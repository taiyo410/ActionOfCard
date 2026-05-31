#include "../pch.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/Camera.h"
#include "../../Base/CharacterBase.h"
#include "../../../Common/AnimationController.h"
#include "../ActionController.h"
#include "PlayerCardAttackOneShort.h"

PlayerCardAttackOneShort::PlayerCardAttackOneShort(ActionController& _actCntl, CharacterBase& _charaObj, CardPresenter& _cardPresenter):
	CardActionBase(_actCntl, _charaObj, _cardPresenter)
{
}

PlayerCardAttackOneShort::~PlayerCardAttackOneShort(void)
{
}

void PlayerCardAttackOneShort::Load(void)
{
}

void PlayerCardAttackOneShort::Init(void)
{
	//突きアニメーションへ
	anim_.PlayBlend(static_cast<int>(CharacterBase::ANIM_TYPE::ATTACK_1_SHORT), animVar_);
	atkAnim_ = static_cast<int>(CharacterBase::ANIM_TYPE::ATTACK_1_SHORT);
}

void PlayerCardAttackOneShort::Update(void)
{
	//攻撃中にカード負けしたら処理を飛ばす
	if (IsCardFailure(Collider::TAG::NML_ATK))
	{
		return;
	}

	//攻撃の更新
	AttackMotion(atk_, Collider::TAG::NML_ATK, {});
}

void PlayerCardAttackOneShort::Release(void)
{
	//現在使っているカードを捨てる
	cardPresent_.ChangeAction();
}

void PlayerCardAttackOneShort::LoadAnimVar(const ACTION_LOAD_DATA& _data)
{
	if (_data.name != "Attack_1_Short")return;

	animVar_ = _data.animVariable;
	LoadAttackStatus(_data.jsonData, atk_);
}
