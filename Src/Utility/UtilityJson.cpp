#include "pch.h"
#include "UtilityJson.h"

const VECTOR UtilityJson::GetLoadVector3(const nlohmann::json& _data, const VECTOR defaultValue)
{
	VECTOR ret = defaultValue;
	ret.x = _data.value("x", defaultValue.x);
	ret.y = _data.value("y", defaultValue.y);
	ret.z = _data.value("z", defaultValue.z);
	return ret;
}

const VECTOR UtilityJson::GetLoadVector3(const std::string key, const nlohmann::json& _data,const VECTOR defaultValue)
{
	VECTOR ret = defaultValue;
	//座標のキーがなければ処理を抜ける
	if (!_data.contains(key))return ret;

	const auto& posData = _data[key];

	ret.x = posData.value("x", defaultValue.x);
	ret.y = posData.value("y", defaultValue.y);
	ret.z = posData.value("z", defaultValue.z);
	return ret;
}

const Vector2F UtilityJson::GetLoadVector2F(const nlohmann::json& _data, const Vector2F defaultValue)
{
	Vector2F ret = defaultValue;

	ret.x = _data.value("x", defaultValue.x);
	ret.y = _data.value("y", defaultValue.y);
	return ret;
}

const Vector2F UtilityJson::GetLoadVector2F(const std::string key, const nlohmann::json& _data, const Vector2F defaultValue)
{
	Vector2F ret = defaultValue;

	//座標のキーがなければ処理を抜ける
	if (!_data.contains(key))return ret;
	const auto& posData = _data[key];
	
	ret.x = posData.value("x", defaultValue.x);
	ret.y = posData.value("y", defaultValue.y);
	return ret;
}

const Vector2 UtilityJson::GetLoadVector2(const nlohmann::json& _data, const Vector2 defaultValue)
{
	Vector2 ret = defaultValue;

	ret.x = _data.value("x", defaultValue.x);
	ret.y = _data.value("y", defaultValue.y);
	return ret;
}

const Vector2 UtilityJson::GetLoadVector2(const std::string key, const nlohmann::json& _data, const Vector2 defaultValue)
{
	Vector2 ret = defaultValue;

	//座標のキーがなければ処理を抜ける
	if (!_data.contains(key))return ret;
	const auto& posData = _data[key];

	ret.x = posData.value("x", defaultValue.x);
	ret.y = posData.value("y", defaultValue.y);
	return ret;
}

const FLOAT4 UtilityJson::GetLoadColorF(const nlohmann::json& _data, const FLOAT4 defaultValue)
{
	FLOAT4 ret = defaultValue;

	ret.x = _data.value("r", defaultValue.x);
	ret.y = _data.value("g", defaultValue.y);
	ret.z = _data.value("b", defaultValue.z);
	ret.w = _data.value("a", defaultValue.w);
	return ret;
}

const FLOAT4 UtilityJson::GetLoadColorF(const std::string key, const nlohmann::json& _data, const FLOAT4 defaultValue)
{
	FLOAT4 ret = defaultValue;

	//座標のキーがなければ処理を抜ける
	if (!_data.contains(key))return ret;
	const auto& colData = _data[key];

	ret.x = colData.value("r", defaultValue.x);
	ret.y = colData.value("g", defaultValue.y);
	ret.z = colData.value("b", defaultValue.z);
	ret.w = colData.value("a", defaultValue.w);
	return ret;
}

const INT4 UtilityJson::GetLoadColor(const nlohmann::json& _data, const INT4 defaultValue)
{
	INT4 ret = defaultValue;

	ret.x = _data.value("r", defaultValue.x);
	ret.y = _data.value("g", defaultValue.y);
	ret.z = _data.value("b", defaultValue.z);
	ret.w = _data.value("a", defaultValue.w);
	return ret;
}

const INT4 UtilityJson::GetLoadColor(const std::string key, const nlohmann::json& _data, const INT4 defaultValue)
{
	INT4 ret = {};
	//座標のキーがなければ処理を抜ける
	if (!_data.contains(key))return ret;
	const auto& colData = _data[key];

	ret.x = colData.value("r", defaultValue.x);
	ret.y = colData.value("g", defaultValue.y);
	ret.z = colData.value("b", defaultValue.z);
	ret.w = colData.value("a", defaultValue.w);
	return ret;
}
