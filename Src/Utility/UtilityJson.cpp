#include "../pch.h"
#include "UtilityJson.h"

const VECTOR UtilityJson::GetLoadVector3(const nlohmann::json& _data)
{
	VECTOR ret = {};
	ret.x = _data.value("x", 0.0f);
	ret.y = _data.value("y", 0.0f);
	ret.z = _data.value("z", 0.0f);
	return ret;
}

const VECTOR UtilityJson::GetLoadVector3(const std::string key, const nlohmann::json& _data)
{
	VECTOR ret = {};
	//座標のキーがなければ処理を抜ける
	if (!_data.contains(key))return ret;

	const auto& posData = _data[key];

	ret.x = posData.value("x", 0.0f);
	ret.y = posData.value("y", 0.0f);
	ret.z = posData.value("z", 0.0f);
	return ret;
}

const Vector2F UtilityJson::GetLoadVector2F(const nlohmann::json& _data)
{
	Vector2F ret = {};

	ret.x = _data.value("x", 0.0f);
	ret.y = _data.value("y", 0.0f);
	return ret;
}

const Vector2F UtilityJson::GetLoadVector2F(const std::string key, const nlohmann::json& _data)
{
	Vector2F ret = {};

	//座標のキーがなければ処理を抜ける
	if (!_data.contains(key))return ret;
	const auto& posData = _data[key];
	
	ret.x = posData.value("x", 0.0f);
	ret.y = posData.value("y", 0.0f);
	return ret;
}

const Vector2 UtilityJson::GetLoadVector2(const nlohmann::json& _data)
{
	Vector2 ret = {};

	ret.x = _data.value("x", 0.0f);
	ret.y = _data.value("y", 0.0f);
	return ret;
}

const Vector2 UtilityJson::GetLoadVector2(const std::string key, const nlohmann::json& _data)
{
	Vector2 ret = {};

	//座標のキーがなければ処理を抜ける
	if (!_data.contains(key))return ret;
	const auto& posData = _data[key];

	ret.x = posData.value("x", 0.0f);
	ret.y = posData.value("y", 0.0f);
	return ret;
}

const FLOAT4 UtilityJson::GetLoadColorF(const nlohmann::json& _data)
{
	FLOAT4 ret = {};

	ret.x = _data.value("r", 0.0f);
	ret.y = _data.value("g", 0.0f);
	ret.z = _data.value("b", 0.0f);
	ret.w = _data.value("a", 0.0f);
	return ret;
}

const FLOAT4 UtilityJson::GetLoadColorF(const std::string key, const nlohmann::json& _data)
{
	FLOAT4 ret = {};

	//座標のキーがなければ処理を抜ける
	if (!_data.contains(key))return ret;
	const auto& colData = _data[key];

	ret.x = colData.value("r", 0.0f);
	ret.y = colData.value("g", 0.0f);
	ret.z = colData.value("b", 0.0f);
	ret.w = colData.value("a", 0.0f);
	return ret;
}

const INT4 UtilityJson::GetLoadColor(const nlohmann::json& _data)
{
	INT4 ret = {};

	ret.x = _data.value("r", 0);
	ret.y = _data.value("g", 0);
	ret.z = _data.value("b", 0);
	ret.w = _data.value("a", 0);
	return ret;
}

const INT4 UtilityJson::GetLoadColor(const std::string key, const nlohmann::json& _data)
{
	INT4 ret = {};
	//座標のキーがなければ処理を抜ける
	if (!_data.contains(key))return ret;
	const auto& colData = _data[key];

	ret.x = colData.value("r", 0);
	ret.y = colData.value("g", 0);
	ret.z = colData.value("b", 0);
	ret.w = colData.value("a", 0);
	return ret;
}
