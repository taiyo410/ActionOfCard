#include "Utility/UtilityCommon.h"
#include "Utility/Utility3D.h"
#include "Manager/Generic/InputManager.h"
#include "Manager/Generic/SceneManager.h"
#include "Object/Character/Base/CharacterBase.h"
#include "./EnemyLogic.h"

EnemyLogic::EnemyLogic(Transform& _myTrans):
	LogicBase(_myTrans),
	scnMng_(SceneManager::GetInstance()),
	cardCoolCnt_(0.0f),
	freezeCnt_(0.0f),
	atkRange_(0.0f),
	prevAttackType_(ENEMY_ATTACK_TYPE::NONE)
{
}

EnemyLogic::~EnemyLogic(void)
{
}

void EnemyLogic::Init(void)
{
	cardCoolCnt_ = CARD_COOL_TIME;
	attackType_ = ENEMY_ATTACK_TYPE::NONE;

	freezeCnt_ = INIT_FREEZE_TIME;
}

void EnemyLogic::Update(void)
{
	//すべての行動フラグをリセット
	isAct_ = {};
	moveDir_ = Utility3D::VECTOR_ZERO;
	VECTOR playerPos = targetChara_.lock()->GetTransform().pos;
	VECTOR targetVec = Utility3D::GetMoveVec(myTrans_.pos,playerPos);

	if(freezeCnt_ > 0.0f)
	{
		freezeCnt_ -= scnMng_.GetDeltaTime();
		return;
	}

	//y座標までは追従しない
	targetVec.y = 0.0f;

	cardCoolCnt_ -= scnMng_.GetDeltaTime();

	float distance = GetTargetDis();

	if (distance > ATK_DISTANCE &&!isActioning_)
	{
		//距離が遠いときは近づく
		isAct_.isRun = true;
		moveDir_ = targetVec;
		return;
	}

	//硬直がなかったらカード使用可能
	//if (freezeCnt_ <= 0.0f && !isActioning_)
	if (InputManager::GetInstance().IsTrgDown(KEY_INPUT_Z))
	{
		isAct_.isCardUse = true;
		cardCoolCnt_ = CARD_COOL_TIME;
	}

#ifdef _DEBUG
	//DebugUpdate();
#endif // _DEBUG
}

#ifdef _DEBUG
void EnemyLogic::DebugUpdate(void)
{
	//移動角度を決める
	if (InputManager::GetInstance().IsNew(KEY_INPUT_UP))
	{
		moveDir_ = Utility3D::DIR_F;
	}
	else if (InputManager::GetInstance().IsNew(KEY_INPUT_LEFT))
	{
		moveDir_ = Utility3D::DIR_L;
	}
	else if (InputManager::GetInstance().IsNew(KEY_INPUT_DOWN))
	{
		moveDir_ = Utility3D::DIR_B;
	}
	else if (InputManager::GetInstance().IsNew(KEY_INPUT_RIGHT))
	{
		moveDir_ = Utility3D::DIR_R;
	}
}
#endif // _DEBUG

#ifdef _DEBUG
void EnemyLogic::DebugDraw(void)
{

}
#endif // _DEBUG


void EnemyLogic::SetFreezeCntByAttackType(void)
{
	switch (prevAttackType_)
	{
	case LogicBase::ENEMY_ATTACK_TYPE::NONE:
		break;
	case LogicBase::ENEMY_ATTACK_TYPE::STOMP:
		freezeCnt_ = NORMAL_FREEZE_TIME;
		break;
	case LogicBase::ENEMY_ATTACK_TYPE::JUMP:
		freezeCnt_ = JUMP_FREEZE_TIME;
		break;
	}
}