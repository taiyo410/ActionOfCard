#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Resource/SoundManager.h"
#include "../Base/ActionBase.h"
#include"../Base/LogicBase.h"
#include"../Utility/Utility3D.h"
#include"./ActionController.h"
#include"../Base/CharacterBase.h"
#include"../../Common/AnimationController.h"
#include "Run.h"

Run::Run(ActionController& _actCntl, CharacterBase& _character):
	ActionBase(_actCntl,_character),
	footSESrc_(ResourceManager::SRC::NONE),
	footSeDis_(0.0f),
	footSECnt_(0.0f)
{
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
	anim_.PlayBlend(static_cast<int>(CharacterBase::ANIM_TYPE::RUN),animVar_);
	speed_ = character_.GetStatus().speed;
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
		actionCntl_.DesideCardAction();
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

void Run::LoadAnimVar(const ACTION_LOAD_DATA& _data)
{
	if (_data.name != "Run")return;

	animVar_=_data.animVariable;
	footSESrc_ = resMng_.GetSrcFromString(_data.jsonData.value("footSE", ""));
	footSeDis_ = _data.jsonData.value("footSEDisCount", 0.0f);
}
