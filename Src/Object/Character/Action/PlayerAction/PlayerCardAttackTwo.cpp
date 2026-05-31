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
	CardActionBase(_actCntl,_charaObj,_cardPresenter)
{
}

PlayerCardAttackTwo::~PlayerCardAttackTwo(void)
{
}

void PlayerCardAttackTwo::Load(void)
{
}

void PlayerCardAttackTwo::Init(void)
{
	//攻撃2段階目のアニメーションを再生
	anim_.PlayBlend(static_cast<int>(CharacterBase::ANIM_TYPE::ATTACK_2), animVar_);
	atkAnim_ = static_cast<int>(CharacterBase::ANIM_TYPE::ATTACK_2);

	//攻撃段階を増やす
	//ChangeActionCardInit();
}

void PlayerCardAttackTwo::Update(void)
{
	//攻撃中にカード負けしたら処理を飛ばす
	if (IsCardFailure(Collider::TAG::NML_ATK))
	{
		return;
	}

	//攻撃の更新
	AttackMotion(atk_, Collider::TAG::NML_ATK, {});
}

void PlayerCardAttackTwo::Release(void)
{
	//現在使っているカードを捨てる
	cardPresent_.ChangeAction();
}

void PlayerCardAttackTwo::LoadAnimVar(const ACTION_LOAD_DATA& _data)
{
	if (_data.name != "Attack_2")return;

	animVar_ = _data.animVariable;
	LoadAttackStatus(_data.jsonData, atk_);
}
