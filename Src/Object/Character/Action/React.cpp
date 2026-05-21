#include "../Manager/Generic/SceneManager.h"
#include "../Base/ActionBase.h"
#include "../Base/LogicBase.h"
#include "../Utility/Utility3D.h"
#include "../Player/ActionController.h"
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
	anim_.Play(static_cast<int>(CharacterBase::ANIM_TYPE::REACT), isLoop, ANIM_START, LOOP_END);
	character_.PlayCharacterAnim(CharacterBase::ANIM_TYPE::REACT);
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
			actionCntl_.ChangeAction(ActionController::ACTION_TYPE::IDLE);
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
