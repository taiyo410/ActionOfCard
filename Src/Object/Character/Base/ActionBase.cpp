#include"../Utility/Utility3D.h"
#include"../Utility/UtilityCommon.h"
#include"../Manager/Generic/SceneManager.h"
#include"../../../Manager/Generic/Camera.h"
#include"../Player/ActionController.h"
#include "../../../Object/Common/AnimationController.h"
#include "../Base/CharacterBase.h"
#include"../Base/LogicBase.h"
#include "ActionBase.h"

ActionBase::ActionBase(ActionController& _actCntl):
	scnMng_(SceneManager::GetInstance()),
	actionCntl_(_actCntl),
	anim_(_actCntl.GetAnimation()),
	soundMng_(SoundManager::GetInstance()),
	resMng_(ResourceManager::GetInstance()),
	isTurnable_(true),
	isAliveAtkCol_(false),
	speed_(0.0f),
	flinchCnt_(0.0f),
	actType_(CARD_ACT_TYPE::NONE),
	isTargetTurn_(false)
{
}

ActionBase::~ActionBase()
{
}

void ActionBase::Release(void)
{
}