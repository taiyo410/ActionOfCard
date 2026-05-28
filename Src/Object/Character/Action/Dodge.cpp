#include "../pch.h"
#include"./ActionController.h"
#include"../../Common/AnimationController.h"
#include"../Base/CharacterBase.h"
#include"../Utility/Utility3D.h"
#include"../Base/LogicBase.h"
#include "Dodge.h"

Dodge::Dodge(ActionController& _actCntl, CharacterBase& _character):
	ActionBase(_actCntl, _character),
	dodgeDir_({})
{
}

Dodge::~Dodge(void)
{
}

void Dodge::Load(void)
{
}

void Dodge::Init(void)
{
	anim_.PlayBlend(static_cast<int>(CharacterBase::ANIM_TYPE::DODGE), animVar_);

	//回避スピードは通常移動スピードに加算する
	speed_ = character_.GetStatus().speed + dodgeSpdAdd_;

	//方向入力されている場合は入力方向、
	//そうでない場合はプレイヤーの前方向に回避
	LogicBase& input = actionCntl_.GetInput();
	if (input.GetIsEnteredDir())
	{
		dodgeDir_ = input.GetInputDir();
	}
	else
	{
		dodgeDir_ = character_.GetTransform().GetForward();
	}

}

void Dodge::Update()
{
	//回避時間が終わったら
	if (anim_.GetAnimStep(static_cast<int>(CharacterBase::ANIM_TYPE::DODGE))> dodgeAnimStep_)
	{
		actionCntl_.ChangeAction(ActionController::ACTION_TYPE::IDLE);
		return;
	}

	//回避中はInputクラスへプレイヤーの前情報をセットする
	actionCntl_.GetInput().SetMoveDir(dodgeDir_);
}

void Dodge::LoadAnimVar(const ACTION_LOAD_DATA& _data)
{
	if (_data.name != "Dodge")return;

	animVar_ = _data.animVariable;
	dodgeAnimStep_ = _data.jsonData.value("endDodgeAnimStep", 0.0f);
	dodgeSpdAdd_ = _data.jsonData.value("addDodgeSpeed", 0.0f);
}
