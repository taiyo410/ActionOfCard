#include "../Manager/Generic/SceneManager.h"
#include "../Base/ActionBase.h"
#include "../Base/LogicBase.h"
#include "../Utility/Utility3D.h"
#include "./ActionController.h"
#include "../Base/CharacterBase.h"
#include "../../Common/AnimationController.h"

#include "React.h"

React::React(ActionController& _actCntl, CharacterBase& _character):
	ActionBase(_actCntl,_character)
{
	isTurnable_ = false;
}

React::~React(void)
{
}

void React::Load(void)
{
}

void React::Init(void)
{
	bool isLoop = false;
	if (flinchCnt_ > 0.0f)
	{
		isLoop = true;
	}
	anim_.PlayBlend(static_cast<int>(CharacterBase::ANIM_TYPE::REACT), animVar_);
}

void React::Update(void)
{
	//ダウン時間中は一定のアニメーションフレームでループ
	if (flinchCnt_ <= 0.0f)
	{
		flinchCnt_ = 0.0f;
		//アニメーション終了でアイドル状態へ
		if (anim_.IsEnd(static_cast<int>(CharacterBase::ANIM_TYPE::REACT)))
		{
			actionCtrl_.ChangeAction(ActionController::ACTION_TYPE::IDLE);
		}
	}
	else
	{
		//のけぞりカウント
		flinchCnt_ -= SceneManager::GetInstance().GetDeltaTime();

		//のけぞり中はアニメーションループ
		anim_.SetEndLoop(static_cast<int>(CharacterBase::ANIM_TYPE::REACT), LOOP_START, LOOP_END, LOOP_SPD);
	}
}

void React::LoadAnimVar(const ACTION_LOAD_DATA& _data)
{
	if (_data.name != "React")return;

	animVar_ = _data.animVariable;
}
