#include "../pch.h"
#include "Object/Common/AnimationController.h"
#include "Object/Common/EffectController.h"
#include "Object/Character/Action/ActionController.h"
#include "Object/Character/Base/CharacterBase.h"
#include "Object/Character/Base/LogicBase.h"
#include "PlayerCardAttackBase.h"

PlayerCardAttackBase::PlayerCardAttackBase(ActionController& _actCntl, CharacterBase& _charaObj, CardPresenter& _deckPresent):
	CardActionBase(_actCntl, _charaObj, _deckPresent)
{
}

PlayerCardAttackBase::~PlayerCardAttackBase(void)
{
}

void PlayerCardAttackBase::Init(void)
{
	atk_.isDamage = false;
	isTurnable_ = false;
	InitAttack();
}

void PlayerCardAttackBase::Update(void)
{
	//攻撃中にカード負けしたら処理を飛ばす
	if (IsCardFailure(Collider::TAG::NML_ATK))
	{
		//攻撃判定無効
		FinishFailureAttack(Collider::TAG::NML_ATK);
		return;
	}

	//回避でアクションを中断
	if (actionCtrl_.GetInput().GetIsAct().isDodge
		&& cardPresent_.GetCardUIState() != CardUIBase::CARD_SELECT::DISITION)
	{
		actionCtrl_.ChangeAction(ActionController::ACTION_TYPE::DODGE);
	}

	//攻撃処理
	AttackUpdate();

	//エフェクト更新
	effect_->Update();
}

void PlayerCardAttackBase::Release(void)
{
	//現在使っているカードを捨てる
	cardPresent_.ChangeAction();

	//念のため、当たり判定削除
	character_.DeleteAttackCol(Collider::TAG::PLAYER1, Collider::TAG::NML_ATK);
	character_.SetIsCanMoveable(true);

	//攻撃アクションの解放
	AttackRelease();
}
