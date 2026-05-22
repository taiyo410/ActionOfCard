#include "../pch.h"
#include "../Utility/Utility3D.h"
#include "ActionBase.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Player/ActionController.h"
#include "../../Common/AnimationController.h"
#include "../Object/Common/EffectController.h"
#include "../../Card/CardDeck.h"
#include "../../Card/CardBase.h"
#include "../../Card/CardUIBase.h"
#include "../../Card/CardSystem.h"
#include "../Base/CharacterBase.h"
#include "../Base/LogicBase.h"
#include "CardActionBase.h"

CardActionBase::CardActionBase(ActionController& _actCntl, CharacterBase& _charaObj, CardPresenter& _deck):
	ActionBase(_actCntl,_charaObj),
	cardPresent_(_deck),
	atkPos_(Utility3D::VECTOR_ZERO),
	isCombo_(false),
	isDuelWait_(false)
{
	effect_ = std::make_unique<EffectController>();

	//攻撃アクションの文字列との対応
	attackActionStr_=
	{
		{"Attack_1_Short", CARD_ACT_TYPE::ATTACK_ONE_SHORT},
		{"Attack_1_Middle", CARD_ACT_TYPE::ATTACK_ONE_MIDDLE},
		{"Attack_2", CARD_ACT_TYPE::ATTACK_TWO},
		{"Attack_3", CARD_ACT_TYPE::ATTACK_THREE},
		{"StompAttack", CARD_ACT_TYPE::STOMP_ATK},
		{"JumpAttack", CARD_ACT_TYPE::JUMP_ATK}
	};
}

CardActionBase::~CardActionBase(void)
{
	atkStatusTable_.clear();
}

void CardActionBase::ChangeCardAction(const CARD_ACT_TYPE& _type)
{
	//同じ状態が入ってきたら抜ける
	if (actType_ == _type)return;
	actType_ = _type;
	changeAction_[actType_]();

	//当たり判定情報を変える
	atkStatusTable_[actType_];
}

void CardActionBase::AttackMotion(const ATK_STATUS& _status, const Collider::TAG& _attackTag, const VECTOR& _localPos)
{
	//攻撃中にカード負けしたら処理を飛ばす
	if (IsCardFailure(_attackTag))return;

	//コンボ入力受付
	if (anim_.GetAnimStep(atkAnim_) >= _status.colEndCnt - _status.bufferFrame)
	{
		ComboInput();
	}

	//攻撃判定処理
	if (anim_.GetAnimStep(atkAnim_) >= _status.colStartCnt&&
		anim_.GetAnimStep(atkAnim_) <= _status.colEndCnt)
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
		actionCntl_.ChangeAction(ActionController::ACTION_TYPE::IDLE);
		return;
	}
	else if (anim_.GetAnimStep(atkAnim_) > _status.colEndCnt)	//攻撃終了後
	{
		//攻撃判定無効
		character_.DeleteAttackCol(character_.GetCharaTag(),_attackTag);

		//次の攻撃につなげる
		ChangeComboAction();
	}
}

bool CardActionBase::IsCardFailure(const Collider::TAG& _attackTag)
{
	//相手のカードに負けたらノックバックする
	if (cardPresent_.IsCardFailure())
	{
		//攻撃判定無効
		FinishFailureAttack(_attackTag);
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
	actType_ = CARD_ACT_TYPE::NONE;
	cardFuncs_.pop();

	//ダメージリアクション状態に移行
	actionCntl_.ChangeAction(ActionController::ACTION_TYPE::REACT);
	actionCntl_.GetInput().IsActioningSet();
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
	if (actionCntl_.GetInput().GetIsAct().isCardUse)
	{
		isCombo_ = true;
	}
}

void CardActionBase::LoadAttackStatus(void)
{
	nlohmann::json j = resMng_.Load(ResourceManager::SRC::ACTION_DATA).jsonData;

	CHARACTER_TYPE chara = character_.GetCharacterType();
	std::string jsonStr = "";
	chara == CHARACTER_TYPE::PLAYER ? jsonStr = "Player" : jsonStr = "Enemy";

	for (const auto& [name, data] : j[jsonStr].items())
	{
		auto it = attackActionStr_.find(name);
		if (it == attackActionStr_.end()) continue;

		std::string dataName = it->first;
		//取得データ
		auto& atkData = data[name];

		auto& atk = atkStatusTable_[it->second];
		if (data.contains("colStartAnimCnt"))
		{
			atk.colStartCnt = data.value("colStartAnimCnt", 0.0f);
		}
		if (data.contains("colEndAnimCnt"))
		{
			atk.colEndCnt = data.value("colEndAnimCnt", 0.0f);
		}
		if (data.contains("bufferFrame"))
		{
			atk.bufferFrame = data.value("bufferFrame", 0.0f);
		}
		if (data.contains("attackRadius"))
		{
			atk.atkRadius = data.value("attackRadius", 0.0f);
		}
		if (data.contains("attackPoint"))
		{
			atk.atkPoint = data.value("attackPoint", 0.0f);
		}
		//攻撃ヒット判定はfalseで格納する
		atk.isDamage = false;

	}

}

void CardActionBase::LoadStatus(void)
{
	LoadAttackStatus();
}
