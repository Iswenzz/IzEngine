#pragma once

#define NLOHMANN_DEFINE_POLY(Type, ChildClass, ...) \
	virtual void Serialize(nlohmann::json& json) override \
	{ \
		ChildClass::Serialize(json); \
		to_json(json, *this); \
	} \
	virtual void Deserialize(nlohmann::json& json) override \
	{ \
		ChildClass::Deserialize(json); \
		from_json(json, *this); \
	} \
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(Type, __VA_ARGS__);

#define NLOHMANN_DEFINE_POLY_BASE(Type, ...) \
	virtual void Serialize(nlohmann::json& json) \
	{ \
		to_json(json, *this); \
	} \
	virtual void Deserialize(nlohmann::json& json) \
	{ \
		from_json(json, *this); \
	} \
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(Type, __VA_ARGS__);
