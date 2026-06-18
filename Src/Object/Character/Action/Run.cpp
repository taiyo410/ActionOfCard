#include "Utility/Utility3D.h"
#include "Manager/Generic/SceneManager.h"
#include "Manager/Resource/SoundManager.h"
#include "Object/Common/AnimationController.h"
#include "Object/Character/Base/ActionBase.h"
#include "Object/Character/Base/LogicBase.h"
#include "Object/Character/Base/CharacterBase.h"
#include "./ActionController.h"
#include "./Run.h"

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
	//カードを場に出す処理
	if (actionCtrl_.GetInput().GetIsAct().isCardUse)
	{
		if (actionCtrl_.IsCardDecisionControl())
		{
			actionCtrl_.DecideCardAction();
			return;
		}
	}

	//入力方向が0だった場合、アイドル状態移行
	if (!actionCtrl_.GetInput().GetIsAct().isRun)
	{
		actionCtrl_.ChangeAction(ActionController::ACTION_TYPE::IDLE);
		return;
	}

	//回避状態移行
	if (actionCtrl_.GetInput().GetIsAct().isDodge)
	{
		actionCtrl_.ChangeAction(ActionController::ACTION_TYPE::DODGE);
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
