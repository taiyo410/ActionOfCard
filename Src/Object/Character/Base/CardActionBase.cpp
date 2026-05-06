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
	ActionBase(_actCntl),
	charaObj_(_charaObj),
	cardPresent_(_deck),
	atkPos_(Utility3D::VECTOR_ZERO),
	isCombo_(false),
	isDuelWait_(false)
{
	effect_ = std::make_unique<EffectController>();
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
	if (anim_.GetAnimStep() >= _status.colEndCnt - _status.bufferFrame)
	{
		ComboInput();
	}

	//攻撃判定処理
	if (anim_.GetAnimStep() >= _status.colStartCnt&&
		anim_.GetAnimStep() <= _status.colEndCnt)
	{
		//攻撃中
		atkPos_ = Utility3D::AddPosRotate(charaObj_.GetTransform().pos, charaObj_.GetTransform().quaRot, _localPos);

		//攻撃判定有効
		isAliveAtkCol_ = true;

		//攻撃の当たり判定消去
		charaObj_.MakeAttackCol(charaObj_.GetCharaTag(), _attackTag, atkPos_,_status.atkRadius);
	}
	else if (anim_.IsEnd())		//アニメーション終了でアイドル状態変更
	{
		actionCntl_.ChangeAction(ActionController::ACTION_TYPE::IDLE);
		return;
	}
	else if (anim_.GetAnimStep() > _status.colEndCnt)	//攻撃終了後
	{
		//攻撃判定無効
		charaObj_.DeleteAttackCol(charaObj_.GetCharaTag(),_attackTag);

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
	charaObj_.DeleteAttackCol(charaObj_.GetCharaTag(), _attackCol);
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

void CardActionBase::LoadAttackStatus(ATK_STATUS& _atk, const std::string _dataName)
{
	nlohmann::json j = resMng_.Load(ResourceManager::SRC::CHARA_DATA).jsonData;

	CHARACTER_TYPE chara = charaObj_.GetCharacterType();
	std::string jsonStr = "";
	chara == CHARACTER_TYPE::PLAYER ? jsonStr = "PlayerAttack" : jsonStr = "EnemyAttack";

	for (const auto& data : j[jsonStr])
	{
		if (data.contains(_dataName))
		{
			auto& atk = data.at(_dataName);
			if(atk.contains("colStartAnimCnt"))
			{
				_atk.colStartCnt = atk.value("colStartAnimCnt", 0.0f);
			}
			if (atk.contains("colEndAnimCnt"))
			{
				_atk.colEndCnt = atk.value("colEndAnimCnt", 0.0f);
			}
			if (atk.contains("bufferFrame"))
			{
				_atk.bufferFrame = atk.value("bufferFrame", 0.0f);
			}
			if (atk.contains("attackRadius"))
			{
				_atk.atkRadius = atk.value("attackRadius", 0.0f);
			}
			if (atk.contains("attackPoint"))
			{
				_atk.atkPoint = atk.value("attackPoint", 0.0f);
			}
		}
	}
	//攻撃ヒット判定はfalseで格納する
	_atk.isDamage = false;
}

void CardActionBase::LoadStatus(void)
{
	for (const auto& atk : atkStatusStrTable_)
	{
		LoadAttackStatus(atkStatusTable_[atk.first], atk.second);
	}
}
