#include "../pch.h"
#include "../pch.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/Camera.h"
#include "../../Base/CharacterBase.h"
#include "../../../Common/AnimationController.h"
#include "../ActionController.h"
#include "PlayerCardAttackOneShort.h"
#include "PlayerCardAttackTwo.h"

PlayerCardAttackTwo::PlayerCardAttackTwo(ActionController& _actCntl, CharacterBase& _charaObj, CardPresenter& _cardPresenter):
	PlayerCardAttackBase(_actCntl,_charaObj,_cardPresenter)
{
}

PlayerCardAttackTwo::~PlayerCardAttackTwo(void)
{
}

void PlayerCardAttackTwo::InitAttack(void)
{
	//攻撃2段階目のアニメーションを再生
	anim_.PlayBlend(static_cast<int>(CharacterBase::ANIM_TYPE::ATTACK_2), animVar_);
	atkAnim_ = static_cast<int>(CharacterBase::ANIM_TYPE::ATTACK_2);
}

void PlayerCardAttackTwo::AttackUpdate(void)
{
	//コンボ入力受付
	actionCtrl_.ComboInput();

	//攻撃の更新
	AttackMotion(atk_, Collider::TAG::NML_ATK, {});
}

void PlayerCardAttackTwo::LoadAnimVar(const ACTION_LOAD_DATA& _data)
{
	if (_data.name != "Attack_2")return;

	animVar_ = _data.animVariable;
	LoadAttackStatus(_data.jsonData, atk_);
}
