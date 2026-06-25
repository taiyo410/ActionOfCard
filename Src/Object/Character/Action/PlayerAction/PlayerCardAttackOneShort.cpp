#include "pch.h"
#include "Manager/Generic/SceneManager.h"
#include "Manager/Generic/Camera.h"
#include "Object/Common/AnimationController.h"
#include "Object/Character/Base/CharacterBase.h"
#include "Object/Character/Action/ActionController.h"
#include "PlayerCardAttackOneShort.h"

PlayerCardAttackOneShort::PlayerCardAttackOneShort(ActionController& _actCntl, CharacterBase& _charaObj, CardPresenter& _cardPresenter):
	PlayerCardAttackBase(_actCntl, _charaObj, _cardPresenter)
{
}

PlayerCardAttackOneShort::~PlayerCardAttackOneShort(void)
{
}

void PlayerCardAttackOneShort::Load(void)
{
}

void PlayerCardAttackOneShort::InitAttack(void)
{
	//突きアニメーションへ
	anim_.PlayBlend(static_cast<int>(CharacterBase::ANIM_TYPE::ATTACK_1_SHORT), animVar_);
	atkAnim_ = static_cast<int>(CharacterBase::ANIM_TYPE::ATTACK_1_SHORT);

	//突き攻撃の方向に向く
	character_.LookAtTargetVec();
}

void PlayerCardAttackOneShort::AttackUpdate(void)
{
	//攻撃の更新
	actionCtrl_.ComboInput();

	AttackMotion(atk_, Collider::TAG::NML_ATK, {});
}

void PlayerCardAttackOneShort::LoadAnimVar(const ACTION_LOAD_DATA& _data)
{
	if (_data.name != "Attack_1_Short")return;

	animVar_ = _data.animVariable;
	LoadAttackStatus(_data.jsonData, atk_);
}
