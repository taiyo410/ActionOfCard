#include"./ActionController.h"
#include"../../Common/AnimationController.h"
#include"../Base/CharacterBase.h"
#include"../Utility/Utility3D.h"
#include"../Base/LogicBase.h"
#include "Idle.h"

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
		actionCtrl_.ChangeAction(ActionController::ACTION_TYPE::MOVE);
		return;
	}
	//カード使用
	if (actionCtrl_.IsCardDecisionControl()&&actionCtrl_.GetInput().GetIsAct().isCardUse)
	{
		actionCtrl_.DesideCardAction();
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
