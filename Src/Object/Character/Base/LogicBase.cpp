#include "./CharacterBase.h"
#include "../Base/CardActionBase.h"
#include "../Manager/Generic/SceneManager.h"
#include "../../../Manager/Generic/Camera.h"
#include"../Utility/Utility3D.h"
#include "LogicBase.h"

LogicBase::LogicBase(Transform& _myTrans):
	myTrans_(_myTrans),
	actCntl_(ACT_CNTL::NONE),
	moveDir_({}),
	isActioning_(false),
	prevMoveDir_(Utility3D::DIR_F),
	attackType_(ENEMY_ATTACK_TYPE::NONE)
{
}

LogicBase::~LogicBase(void)
{
}

VECTOR LogicBase::GetInputDir(void) const
{
	//ƒJƒƒ‰‚ÌŠp“x‚ðŽæ“¾
	auto camera = SceneManager::GetInstance().GetCamera();
	VECTOR cameraRot = camera.lock()->GetAngles();
	Quaternion cameraQuaRot = camera.lock()->GetQuaRotOutX();
	return cameraQuaRot.PosAxis(inputDir_);
}

void LogicBase::SetTargetCharacter(std::shared_ptr<CharacterBase> _target)
{
	targetChara_ = _target;
}

void LogicBase::GetLookAtTargetDir(void)
{
	const VECTOR& targetPos = targetChara_.lock()->GetTransform().pos;
	const VECTOR targetVec = Utility3D::GetMoveVec(myTrans_.pos, targetPos);
	moveDir_ = targetVec;
}

const bool LogicBase::HitTarget(void) const
{
	return false;
}

const Transform LogicBase::GetTargetTransform(void)
{
	return targetChara_.lock()->GetTransform();
}

const int LogicBase::GetJumpCardNum(void) const
{
	return targetChara_.lock()->GetMainAction().GetJumpCardNum();
}

const bool LogicBase::GetIsEnteredDir(void)
{
	return !Utility3D::EqualsVZero(inputDir_);
}

const float LogicBase::GetTargetDis(void) const
{
	const VECTOR targetPos = targetChara_.lock()->GetTransform().pos;
	return static_cast<float>(Utility3D::Distance(myTrans_.pos, targetPos));
}
