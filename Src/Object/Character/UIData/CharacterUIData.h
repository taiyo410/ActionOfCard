#pragma once
#include "../Object/Common/AnimationController.h"
#include <nlohmann/json.hpp>
enum class CHARACTER_TYPE
{
	PLAYER,
	ENEMY,
	MAX
};
struct HP_DATA
{
	float hpPer = 1.0f;
	float preHpPer = 1.0f;
};
struct ACTION_LOAD_DATA
{
	std::string name = "";									//アクション名
	AnimationController::ANIMATION_VARIABLE animVariable = {};	//アニメーションの可変パラメータ
	const nlohmann::json jsonData;								//アクションのデータ
};

