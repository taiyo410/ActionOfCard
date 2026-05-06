#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Resource/SoundManager.h"
#include "../Base/ActionBase.h"
#include"../Base/LogicBase.h"
#include"../Utility/Utility3D.h"
#include"../Player/ActionController.h"
#include"../Base/CharacterBase.h"
#include"../../Common/AnimationController.h"
#include "Run.h"

Run::Run(ActionController& _actCntl, const float _speed, ResourceManager::SRC _src, const float  _footSeDis):
	ActionBase(_actCntl),
	moveSpd_(_speed),
	footSESrc_(_src),
	footSeDis_(_footSeDis),
	footSECnt_()
{
	speed_ = moveSpd_;
}

Run::~Run(void)
{
}

void Run::Load(void)
{
	resMng_.Load(footSESrc_);
}

void Run::Init(void)
{
	anim_.Play(static_cast<int>(CharacterBase::ANIM_TYPE::RUN));
	footSECnt_ = 0.0f;

}

void Run::Update(void)
{
	//入力方向が0だった場合、アイドル状態移行
	if (!actionCntl_.GetInput().GetIsAct().isRun)
	{
		actionCntl_.ChangeAction(ActionController::ACTION_TYPE::IDLE);
		return;
	}

	//カードを場に出す処理
	if (actionCntl_.IsCardDecisionControl() && actionCntl_.GetInput().GetIsAct().isCardUse)
	{
		actionCntl_.ChangeAction(ActionController::ACTION_TYPE::CARD_ACTION);
		return;
	}

	//ジャンプ入力があった場合、ジャンプ状態へ移行
	if (actionCntl_.GetInput().GetIsAct().isJump)
	{
		actionCntl_.ChangeAction(ActionController::ACTION_TYPE::JUMP);
		return;
	}

	//回避状態移行
	if (actionCntl_.GetInput().GetIsAct().isDodge)
	{
		actionCntl_.ChangeAction(ActionController::ACTION_TYPE::DODGE);
		return;
	}

	if (footSECnt_ <= 0.0f)
	{
		SoundManager::GetInstance().Play(footSESrc_, SoundManager::PLAYTYPE::BACK);
		footSECnt_ = footSeDis_;
	}
	else
	{
		footSECnt_ -= scnMng_.GetDeltaTime();
	}
}
