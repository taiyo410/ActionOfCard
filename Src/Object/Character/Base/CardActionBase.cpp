#include "pch.h"
#include "Utility/Utility3D.h"
#include "ActionBase.h"
#include "Manager/Generic/SceneManager.h"
#include "Object/Common/AnimationController.h"
#include "Object/Common/EffectController.h"
#include "Object/Card/CardDeck.h"
#include "Object/Card/CardBase.h"
#include "Object/Card/CardUIBase.h"
#include "Object/Card/CardSystem.h"
#include "Object/Character/Action/ActionController.h"
#include "Object/Character/Base/CharacterBase.h"
#include "Object/Character/Base/LogicBase.h"
#include "./CardActionBase.h"

CardActionBase::CardActionBase(ActionController& _actCntl, CharacterBase& _charaObj, CardPresenter& _cardPresent):
	ActionBase(_actCntl,_charaObj),
	cardPresent_(_cardPresent),
	atkPos_(Utility3D::VECTOR_ZERO),
	isCombo_(false),
	isDuelWait_(false)
{
	effect_ = std::make_unique<EffectController>();
}

CardActionBase::~CardActionBase(void)
{
	//atkStatusTable_.clear();
}

void CardActionBase::AttackMotion(const ATK_STATUS& _status, const Collider::TAG& _attackTag, const VECTOR& _localPos)
{
	//攻撃判定処理
	if (anim_.GetAnimStep(atkAnim_) >= _status.colStartStep&&
		anim_.GetAnimStep(atkAnim_) <= _status.colEndStep)
	{
		//攻撃中
		atkPos_ = Utility3D::AddPosRotate(character_.GetTransform().pos, character_.GetTransform().quaRot, _localPos);

		//攻撃判定有効
		isAliveAtkCol_ = true;

		//攻撃の当たり判定消去
		character_.MakeAttackCol(character_.GetCharaTag(), _attackTag, atkPos_,_status.atkRadius);
	}
	else if (anim_.IsEnd(atkAnim_))		//アニメーション終了でアイドル状態変更
	{
		actionCtrl_.ChangeAction(ActionController::ACTION_TYPE::IDLE);
		return;
	}
	else if (anim_.GetAnimStep(atkAnim_) > _status.colEndStep)	//攻撃終了後
	{
		//攻撃判定無効
		character_.DeleteAttackCol(character_.GetCharaTag(),_attackTag);

		//次の攻撃につなげる
		actionCtrl_.ChangeComboCardAttack();
	}
}

bool CardActionBase::IsCardFailure(const Collider::TAG& _attackTag)
{
	//相手のカードに負けたらノックバックする
	if (cardPresent_.IsCardFailure())
	{
		return true;
	}
	return false;
}

void CardActionBase::ReleaseReloadResource(void)
{

}

void CardActionBase::FinishFailureAttack(const Collider::TAG _attackCol)
{	
	//カード負け処理
	cardPresent_.FailureCard();

	//攻撃判定無効
	character_.DeleteAttackCol(character_.GetCharaTag(), _attackCol);

	//ダメージリアクション状態に移行
	actionCtrl_.ChangeAction(ActionController::ACTION_TYPE::REACT);
	actionCtrl_.GetInput().SetIsActioning(false);
}

void CardActionBase::SetAtk(const ATK_STATUS& _atkStatus)
{
	//攻撃ステータスセット
	atk_ = _atkStatus;

	//ダメージの初期化
	atk_.isDamage = false;
}

void CardActionBase::ComboInput(void)
{
	if (actionCtrl_.GetInput().GetIsAct().isCardUse)
	{
		isCombo_ = true;
	}
}



void CardActionBase::LoadAttackStatus(const nlohmann::json& _jsonData, ATK_STATUS& _atkStatus)
{
	//Json空の読み込み
	_atkStatus.colStartStep = _jsonData.value("colStartAnimCnt", 0.0f);
	_atkStatus.colEndStep = _jsonData.value("colEndAnimCnt", 0.0f);
	_atkStatus.bufferFrame = _jsonData.value("bufferFrame", 0.0f);
	_atkStatus.atkRadius = _jsonData.value("attackRadius", 0.0f);
	_atkStatus.atkPoint = _jsonData.value("attackPoint", 0.0f);

	//攻撃ヒット判定はfalseで格納する
	_atkStatus.isDamage = false;
}
