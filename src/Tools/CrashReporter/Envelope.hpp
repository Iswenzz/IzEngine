#pragma once
#include <nlohmann/json.hpp>

#include <string>
#include <vector>

namespace IzEngine
{
	// One item of a Sentry envelope: a JSON header line and the payload bytes that follow it.
	struct EnvelopeItem
	{
		nlohmann::json Headers;
		std::string Payload;
	};

	// The parts of a Sentry DSN needed to reach the ingest endpoint.
	struct DSN
	{
		bool Valid = false;
		std::string Scheme;
		std::string Host;
		int Port = 0;
		std::string Path;
		std::string PublicKey;
		std::string ProjectId;
	};

	class Envelope
	{
	public:
		nlohmann::json Headers;
		std::vector<EnvelopeItem> Items;

		bool Parse(const std::string& data);
		std::string Serialize() const;
		EnvelopeItem* Find(const std::string& type, const std::string& attachmentType = "");

		static DSN ParseDSN(const std::string& text);
		static std::string EnvelopeURL(const DSN& dsn);
		static std::string AuthHeader(const DSN& dsn, const std::string& client);
	};
}
