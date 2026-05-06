#include "../pch.h"
#include"../Player/ActionController.h"
#include"../../Common/AnimationController.h"
#include"../Base/CharacterBase.h"
#include"../Utility/Utility3D.h"
#include"../Base/LogicBase.h"
#include "Dodge.h"

Dodge::Dodge(ActionController& _actCntl, Transform& _trans, float _spd):
	ActionBase(_actCntl),
	dodgeSpd_(_spd),
	trans_(_trans),
	dodgeDir_({})
{
	speed_ = dodgeSpd_+ ADD_DODGE_SPEED;
}

Dodge::~Dodge(void)
{
}

void Dodge::Load(void)
{
}

void Dodge::Init(void)
{
	anim_.Play(static_cast<int>(CharacterBase::ANIM_TYPE::DODGE),false);

	//方向入力されている場合は入力方向、
	//そうでない場合はプレイヤーの前方向に回避
	LogicBase& input = actionCntl_.GetInput();
	if (input.GetIsEnteredDir())
	{
		dodgeDir_ = input.GetInputDir();
	}
	else
	{
		dodgeDir_ = trans_.GetForward();
	}

}

void Dodge::Update()
{
	//回避時間が終わったら
	if (anim_.GetAnimStep()> END_DODGE_ANIM_STEP)
	{
		actionCntl_.ChangeAction(ActionController::ACTION_TYPE::IDLE);
		return;
	}

	//回避中はInputクラスへプレイヤーの前情報をセットする
	actionCntl_.GetInput().SetMoveDir(dodgeDir_);
}
