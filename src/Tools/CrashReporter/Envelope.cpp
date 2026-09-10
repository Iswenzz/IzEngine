#include "Envelope.hpp"

#include <format>

namespace IzEngine
{
	static nlohmann::json ParseLine(const std::string& data, size_t& pos)
	{
		const size_t end = data.find('\n', pos);
		const std::string line = data.substr(pos, end == std::string::npos ? std::string::npos : end - pos);
		pos = end == std::string::npos ? data.size() : end + 1;
		return nlohmann::json::parse(line, nullptr, false);
	}

	static std::string Dump(const nlohmann::json& json)
	{
		return json.dump(-1, ' ', false, nlohmann::json::error_handler_t::replace);
	}

	// A header line, then items made of a header line and a payload that is either `length` bytes
	// or, without a length, the rest of the line. Crashpad appends its items to the file sentry
	// wrote, so the header line can be missing when sentry never got to write one.
	bool Envelope::Parse(const std::string& data)
	{
		size_t pos = 0;
		Headers = ParseLine(data, pos);
		if (!Headers.is_object())
			Headers = nlohmann::json::object();

		while (pos < data.size())
		{
			if (data[pos] == '\n')
			{
				pos++;
				continue;
			}

			EnvelopeItem item;
			item.Headers = ParseLine(data, pos);
			if (!item.Headers.is_object())
				return false;

			if (item.Headers.contains("length") && item.Headers["length"].is_number_unsigned())
			{
				const size_t length = item.Headers["length"].get<size_t>();
				if (pos + length > data.size())
					return false;
				item.Payload = data.substr(pos, length);
				pos += length;
			}
			else
			{
				const size_t end = data.find('\n', pos);
				item.Payload = data.substr(pos, end == std::string::npos ? std::string::npos : end - pos);
				pos = end == std::string::npos ? data.size() : end + 1;
			}
			Items.push_back(std::move(item));
		}
		return true;
	}

	std::string Envelope::Serialize() const
	{
		std::string out = Dump(Headers);
		for (const auto& item : Items)
		{
			nlohmann::json headers = item.Headers;
			headers["length"] = item.Payload.size();

			out += '\n';
			out += Dump(headers);
			out += '\n';
			out += item.Payload;
		}
		return out;
	}

	EnvelopeItem* Envelope::Find(const std::string& type, const std::string& attachmentType)
	{
		for (auto& item : Items)
		{
			if (item.Headers.value("type", "") != type)
				continue;
			if (!attachmentType.empty() && item.Headers.value("attachment_type", "") != attachmentType)
				continue;
			return &item;
		}
		return nullptr;
	}

	// scheme://publickey[:secret]@host[:port]/[path/]projectid
	DSN Envelope::ParseDSN(const std::string& text)
	{
		DSN dsn;
		const size_t scheme = text.find("://");
		const size_t at = text.find('@');
		if (scheme == std::string::npos || at == std::string::npos || at < scheme)
			return dsn;

		dsn.Scheme = text.substr(0, scheme);
		const std::string credentials = text.substr(scheme + 3, at - scheme - 3);
		dsn.PublicKey = credentials.substr(0, credentials.find(':'));

		const size_t slash = text.find('/', at);
		if (slash == std::string::npos)
			return dsn;

		const std::string authority = text.substr(at + 1, slash - at - 1);
		const size_t colon = authority.find(':');
		dsn.Host = authority.substr(0, colon);
		dsn.Port = colon == std::string::npos ? (dsn.Scheme == "https" ? 443 : 80)
			: static_cast<int>(strtol(authority.c_str() + colon + 1, nullptr, 10));

		const std::string rest = text.substr(slash);
		const size_t last = rest.find_last_of('/');
		dsn.Path = rest.substr(0, last);
		dsn.ProjectId = rest.substr(last + 1);

		dsn.Valid = !dsn.Host.empty() && !dsn.PublicKey.empty() && !dsn.ProjectId.empty() && dsn.Port > 0;
		return dsn;
	}

	std::string Envelope::EnvelopeURL(const DSN& dsn)
	{
		return std::format("{}://{}:{}{}/api/{}/envelope/", dsn.Scheme, dsn.Host, dsn.Port, dsn.Path, dsn.ProjectId);
	}

	std::string Envelope::AuthHeader(const DSN& dsn, const std::string& client)
	{
		return std::format("Sentry sentry_key={}, sentry_version=7, sentry_client={}", dsn.PublicKey, client);
	}
}
