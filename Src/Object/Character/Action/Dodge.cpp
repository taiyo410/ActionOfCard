#include "pch.h"
#include "Utility/Utility3D.h"
#include "Object/Common/AnimationController.h"
#include "Object/Character/Base/CharacterBase.h"
#include "Object/Character/Base/LogicBase.h"
#include "./ActionController.h"
#include "./Dodge.h"

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
	LogicBase& input = actionCtrl_.GetLogic();
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
		actionCtrl_.ChangeAction(ActionController::ACTION_TYPE::IDLE);
		return;
	}

	//回避中はInputクラスへプレイヤーの前情報をセットする
	actionCtrl_.GetLogic().SetMoveDir(dodgeDir_);
}

void Dodge::LoadAnimVar(const ACTION_LOAD_DATA& _data)
{
	if (_data.name != "Dodge")return;

	animVar_ = _data.animVariable;
	dodgeAnimStep_ = _data.jsonData.value("endDodgeAnimStep", 0.0f);
	dodgeSpdAdd_ = _data.jsonData.value("addDodgeSpeed", 0.0f);
}
