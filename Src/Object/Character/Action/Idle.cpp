#include"../Player/ActionController.h"
#include"../../Common/AnimationController.h"
#include"../Base/CharacterBase.h"
#include"../Utility/Utility3D.h"
#include"../Base/LogicBase.h"
#include "Idle.h"

Idle::Idle(ActionController& _actCntl):
	ActionBase(_actCntl)
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
	anim_.Play(static_cast<int>(CharacterBase::ANIM_TYPE::IDLE));
}

void Idle::Update(void)
{
	//移動
	if (actionCntl_.GetInput().GetIsAct().isRun)
	{
		actionCntl_.ChangeAction(ActionController::ACTION_TYPE::MOVE);
		return;
	}
	//カード使用
	if (actionCntl_.IsCardDecisionControl()&&actionCntl_.GetInput().GetIsAct().isCardUse)
	{
		actionCntl_.ChangeAction(ActionController::ACTION_TYPE::CARD_ACTION);
		return;
	}
	//回避
	if (actionCntl_.GetInput().GetIsAct().isDodge)
	{
		actionCntl_.ChangeAction(ActionController::ACTION_TYPE::DODGE);
		return;
	}
}