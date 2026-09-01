#pragma once

#include <nlohmann/json.hpp>
#include <DxLib.h>
#include "Common/Vector2.h"
#include "Common/Vector2F.h"

class UtilityJson
{
public:

	/// @brief Jsonから3D座標を取得する
	/// @param _data 取得したいデータ
	/// @return 3D座標
	static const VECTOR GetLoadVector3(const nlohmann::json& _data, const VECTOR defaultValue = {});
	static const VECTOR GetLoadVector3(const std::string key,const nlohmann::json& _data,const VECTOR defaultValue = {});

	/// @brief Jsonから2D座標(float)を取得する
	/// @param _data 取得したいデータ
	/// @return 2D座標(float)
	static const Vector2F GetLoadVector2F(const nlohmann::json& _data, const Vector2F defaultValue = {});
	static const Vector2F GetLoadVector2F(const std::string key, const nlohmann::json& _data, const Vector2F defaultValue = {});

	/// @brief Jsonから2D座標(int)を取得する
	/// @param _data 取得したいデータ
	/// @return 2D座標(int)
	static const Vector2 GetLoadVector2(const nlohmann::json& _data, const Vector2 defaultValue = {});
	static const Vector2 GetLoadVector2(const std::string key, const nlohmann::json& _data, const Vector2 defaultValue = {});

	/// @brief Jsonからカラー(FLOAT4)を取得する
	/// @param _data 取得したいデータ
	/// @return カラー(FLOAT4)
	static const FLOAT4 GetLoadColorF(const nlohmann::json& _data, const FLOAT4 defaultValue = {});
	static const FLOAT4 GetLoadColorF(const std::string key, const nlohmann::json& _data, const FLOAT4 defaultValue = {});

	/// @brief Jsonからカラー(INT4)を取得する
	/// @param _data 取得したいデータ
	/// @return カラー(INT4)
	static const INT4 GetLoadColor(const nlohmann::json& _data, const INT4 defaultValue = {});
	static const INT4 GetLoadColor(const std::string key, const nlohmann::json& _data, const INT4 defaultValue = {});
};

