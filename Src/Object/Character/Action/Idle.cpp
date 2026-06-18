#include "Utility/Utility3D.h"
#include "Object/Common/AnimationController.h"
#include "Object/Character/Base/CharacterBase.h"
#include "Object/Character/Base/LogicBase.h"
#include "./ActionController.h"
#include "./Idle.h"

Idle::Idle(ActionController& _actCntl,CharacterBase& _character):
	ActionBase(_actCntl,_character)
{
}

Idle::~Idle(void)
{
}

void Idle::Load(void)
{
}

void Idle::Init(void)
{
	//anim_.Play(static_cast<int>(CharacterBase::ANIM_TYPE::IDLE));
	anim_.PlayBlend(static_cast<int>(CharacterBase::ANIM_TYPE::IDLE), animVar_);
}

void Idle::Update(void)
{
	//移動
	if (actionCtrl_.GetInput().GetIsAct().isRun)
	{
		actionCtrl_.ChangeAction(ActionController::ACTION_TYPE::RUN);
		return;
	}
	//カード使用
	if (actionCtrl_.IsCardDecisionControl()&&actionCtrl_.GetInput().GetIsAct().isCardUse)
	{
		actionCtrl_.DecideCardAction();
		return;
	}
	//回避
	if (actionCtrl_.GetInput().GetIsAct().isDodge)
	{
		actionCtrl_.ChangeAction(ActionController::ACTION_TYPE::DODGE);
		return;
	}
}

void Idle::LoadAnimVar(const ACTION_LOAD_DATA& _data)
{
	if(_data.name!="Idle")return;

	animVar_ = _data.animVariable;
}
