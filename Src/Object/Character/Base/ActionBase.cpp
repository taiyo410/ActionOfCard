#include "Utility/Utility3D.h"
#include "Utility/UtilityCommon.h"
#include "Manager/Generic/SceneManager.h"
#include "Manager/Generic/Camera.h"
#include "Object/Common/AnimationController.h"
#include "Object/Character/Action/ActionController.h"
#include "Object/Character/Base/CharacterBase.h"
#include "Object/Character/Base/LogicBase.h"
#include "./ActionBase.h"

ActionBase::ActionBase(ActionController& _actCntl,CharacterBase& _character):
	scnMng_(SceneManager::GetInstance()),	
	actionCtrl_(_actCntl),
	anim_(_actCntl.GetAnimation()),
	soundMng_(SoundManager::GetInstance()),
	resMng_(ResourceManager::GetInstance()),
	character_(_character),
	isTurnable_(true),
	isAliveAtkCol_(false),
	speed_(0.0f),
	flinchCnt_(0.0f),
	isTargetTurn_(false)
{
}

ActionBase::~ActionBase()
{
}

void ActionBase::Release(void)
{
}