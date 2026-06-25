#include "pch.h"
#include "Utility/UtilityCommon.h"
#include "Manager/Generic/SceneManager.h"
#include "Manager/Generic/UIManager.h"
#include "Object/Common/EffectController.h"
#include "Object/Card/CardSystem.h"
#include "Object/CHaracter/Base/CharacterBase.h"
#include "Object/Character/Action/ActionController.h"
#include "Object/Character/Enemy/EnemyLogic.h"
#include "EnemyRevolutionAction.h"

EnemyRevolutionAction::EnemyRevolutionAction(ActionController& _actCntl, CharacterBase& _character):
	ActionBase(_actCntl,_character),
	effectPlayId_(UtilityCommon::INITIAL_HANDLE),
	isRevolutionChange_(false),
	uiMng_(UIManager::GetInstance())
{
}

EnemyRevolutionAction::~EnemyRevolutionAction(void)
{
}

void EnemyRevolutionAction::Load(void)
{
	//エフェクトの追加
	effect_->Add(resMng_.Load(ResourceManager::SRC::REVOLUTION_EFF).handleId_, EffectController::EFF_TYPE::REVOLUTION_EFF);
}

void EnemyRevolutionAction::Init(void)
{
	anim_.PlayBlend(static_cast<int>(CharacterBase::ANIM_TYPE::REVOLUTION), animVar_);
	effectPlayId_ = effect_->Play(EffectController::EFF_TYPE::REVOLUTION_EFF
		, {}, Quaternion(), effectScale_,false, effectPlaySpeed_);
	isRevolutionChange_ = false;
	actionCtrl_.GetLogic().SetIsActioning(true);
	uiMng_.InitIsEndRevolution();
	waitCnt_ = 0.0f;
}

void EnemyRevolutionAction::Update(void)
{
	const float animationStep = anim_.GetAnimStep(static_cast<int>(CharacterBase::ANIM_TYPE::REVOLUTION));
	if (isRevolutionChange_ && uiMng_.GetIsEndRevolutionDirection())
	{
		//ループ設定
		anim_.SetEndMidLoop(static_cast<int>(CharacterBase::ANIM_TYPE::REVOLUTION)
			, animVar_.speed);
		actionCtrl_.ChangeAction(ActionController::ACTION_TYPE::IDLE);
		actionCtrl_.GetLogic().SetIsActioning(false);
		return;
	}
	else if (animationStep > revolutionStartAnimStep_)
	{
		//ループ設定
		anim_.SetMidLoop(static_cast<int>(CharacterBase::ANIM_TYPE::REVOLUTION)
			, animationLoopStartStep_, animationLoopEndStep_, animationLoopSpeed_);

		//勝敗ルールを変える
		if (!isRevolutionChange_)
		{
			uiMng_.StartRevolution();
			isRevolutionChange_ = true;
		}
	}
}

void EnemyRevolutionAction::Release(void)
{
	effect_->Stop(EffectController::EFF_TYPE::REVOLUTION_EFF, effectPlayId_);
			actionCtrl_.GetLogic().SetIsActioning(false);
}

void EnemyRevolutionAction::LoadAnimVar(const ACTION_LOAD_DATA& _data)
{
	if (_data.name != "Revolution")return;

	animVar_ = _data.animVariable;
	revolutionStartAnimStep_ = _data.jsonData.value("startRevolutionAnimStep", 0.0f);
	animationLoopStartStep_ = _data.jsonData.value("animationLoopStartStep", 0.0f);
	animationLoopEndStep_ = _data.jsonData.value("animationLoopEndStep", 0.0f); 
	animationLoopSpeed_ = _data.jsonData.value("animationLoopSpeed", 0.0f);
	float scale = _data.jsonData.value("effectScale", 0.0f);
	effectScale_ = { scale,scale,scale };
	effectPlaySpeed_ = _data.jsonData.value("effectPlaySpeed", 1.0f);
	waitTime_= _data.jsonData.value("waitTIme", 0.0f);
}
