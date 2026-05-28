#include "../Utility/Utility3D.h"
#include "../Common/Easing.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/Camera.h"
#include "../Manager/Resource/ResourceManager.h"
#include "../Object/Common/EffectController.h"
#include "../ActionController.h"
#include "../../../Common/AnimationController.h"
#include "../../../Card/CardDeck.h"
#include "../../../Card/CardBase.h"
#include "../../../Card/CardUIBase.h"
#include "../../../Card/CardSystem.h"
#include "../../Base/CharacterBase.h"
#include "../../Base/LogicBase.h"
#include "PlayerCardAction.h"

PlayerCardAction::PlayerCardAction(ActionController& _actCntl, CharacterBase& _charaObj, CardPresenter& _deck):
	CardActionBase(_actCntl, _charaObj, _deck),
	pushReloadCnt_(),
	cardActType_(CARD_ACTION_TYPE::NONE)
{

	////炎魔法の生成
	//magicFire_ = std::make_unique<PlayerFireMagicAction>(actionCntl_, character_, cardPresent_);

	////通常攻撃
	//attack_ = std::make_unique<PlayerCardAttackAction>(actionCntl_, character_, cardPresent_,*this);
	isTurnable_ = false;
	changeCardAction_ ={
		{ CARD_ACTION_TYPE::ATTACK_ONE_SHORT, [this]() {ChangeShortAttackOne(); }},
		{ CARD_ACTION_TYPE::ATTACK_ONE_MIDDLE, [this]() {ChangeMiddleAttackOne(); }},
		{ CARD_ACTION_TYPE::ATTACK_TWO, [this]() {ChangeAttackTwo(); }},
		{ CARD_ACTION_TYPE::ATTACK_THREE, [this]() {ChangeAttackThree(); }},
		//{ CARD_ACTION_TYPE::MAGIC_FIRE, [this]() {ChangeMagicFire(); }},
		{ CARD_ACTION_TYPE::RELOAD, [this]() {ChangeReload(); }},
	};
	attackActionStr_ = {
		{"Attack_1_Short", CARD_ACTION_TYPE::ATTACK_ONE_SHORT},
		{"Attack_1_Middle", CARD_ACTION_TYPE::ATTACK_ONE_MIDDLE},
		{"Attack_2", CARD_ACTION_TYPE::ATTACK_TWO},
		{"Attack_3", CARD_ACTION_TYPE::ATTACK_THREE},
		//{"FireMagic", CARD_ACTION_TYPE::MAGIC_FIRE},
		{"Reload", CARD_ACTION_TYPE::RELOAD}
	};
	atk_ = {};
	easing_ = std::make_unique<Easing>();
}

PlayerCardAction::~PlayerCardAction(void)
{
	changeCardAction_.clear();

	//カード機能配列の解放
	std::queue<std::function<void(void)>> empty;
	std::swap(cardFuncs_, empty);
}

void PlayerCardAction::Load(void)
{
	//リソースの更新
	resMng_.Load(ResourceManager::SRC::CARD_RELOAD_SE);
	resMng_.Load(ResourceManager::SRC::CARD_RELOAD_FINISH_SE);
	resMng_.Load(ResourceManager::SRC::CARD_PUT_SE);
	////使用エフェクトの追加
	effect_->Add(ResourceManager::GetInstance().Load(ResourceManager::SRC::RELOAD_EFF).handleId_, EffectController::EFF_TYPE::RELOAD);
	effect_->Add(ResourceManager::GetInstance().Load(ResourceManager::SRC::RELOAD_END_EFF).handleId_, EffectController::EFF_TYPE::RELOAD_END);
}

void PlayerCardAction::Init(void)
{
	attackStageNum_ = 0;
	atk_.isDamage = false;
	speed_ = 0.0f;
	isCombo_ = false;

	//キーによる移動はしない
	character_.SetIsCanMoveable(false);

	if (cardPresent_.GetCardType() == CardBase::CARD_TYPE::ATTACK)
	{
		//カードの移動SEを再生
		soundMng_.Play(ResourceManager::SRC::CARD_PUT_SE, SoundManager::PLAYTYPE::BACK);

		//手札に移動
		cardPresent_.PutCard();

		//攻撃処理へ
		DecideAttackOne();
	}
	else if (cardPresent_.GetCardType() == CardBase::CARD_TYPE::FIRE)
	{

	}
	else if (cardPresent_.GetCardType()==CardBase::CARD_TYPE::RELOAD)
	{
		//リロード処理へ
		ChangeCardAction(CARD_ACTION_TYPE::RELOAD);
	}
}

void PlayerCardAction::Update()
{
	//アクション配列の先頭の更新
	cardFuncs_.front()();

	//回避でアクションを中断
	if (actionCntl_.GetInput().GetIsAct().isDodge
		&&cardPresent_.GetCardUIState()!= CardUIBase::CARD_SELECT::DISITION)
	{
		actionCntl_.ChangeAction(ActionController::ACTION_TYPE::DODGE);
	}
}

void PlayerCardAction::Release(void)
{
	//現在使っているカードを捨てる
	cardPresent_.ChangeAction();

	//当たり判定削除
	character_.DeleteAttackCol(Collider::TAG::PLAYER1,Collider::TAG::NML_ATK);
	character_.SetIsCanMoveable(true);

	//リロード中にアクションが中断された場合
	if (cardActType_ == CARD_ACTION_TYPE::RELOAD)
	{
		//リロードSEの停止
		soundMng_.Stop(ResourceManager::SRC::CARD_RELOAD_SE);

		//エフェクトの消去
		effect_->Stop(EffectController::EFF_TYPE::RELOAD, 0);
		effect_->Delete(EffectController::EFF_TYPE::RELOAD, 0);

		//UIの状態をNONEへ
		cardPresent_.ChangeUIState(CardUIBase::CARD_SELECT::NONE);
	}
	
	//コンボフラグリセット
	isCombo_ = false;

	//カード機能配列の解放
	if (!cardFuncs_.empty())
	{
		cardFuncs_.pop();
	}

	cardActType_ = CARD_ACTION_TYPE::NONE;
}

void PlayerCardAction::ReleaseReloadResource(void)
{
	//カードリロード音停止
	soundMng_.Stop(ResourceManager::SRC::CARD_RELOAD_SE);

	//エフェクトの停止
	effect_->Stop(EffectController::EFF_TYPE::RELOAD, 0);
	effect_->Delete(EffectController::EFF_TYPE::RELOAD, 0);
	cardActType_ = CARD_ACTION_TYPE::NONE;
}

const bool PlayerCardAction::IsReloading(void)const
{
	return cardActType_ == CARD_ACTION_TYPE::RELOAD;
}

void PlayerCardAction::LoadAnimVar(const ACTION_LOAD_DATA& _data)
{
	const auto it = attackActionStr_.find(_data.name);
	if (it == attackActionStr_.end())return;

	atkAnimVals_[it->second] = _data.animVariable;

	auto& atk = atkStatusTable_[it->second];
	auto& jsonData = _data.jsonData;
	LoadAttackStatus(jsonData, atk);
}

void PlayerCardAction::ChangeCardAction(const CARD_ACTION_TYPE _type)
{
	if (cardActType_ == _type)return;
	cardActType_ = _type;

	//攻撃ステータスのセット
	atk_ = atkStatusTable_[cardActType_];

	//アニメーション変数のセット
	animVar_ = atkAnimVals_[cardActType_];

	changeCardAction_[cardActType_]();
}

bool PlayerCardAction::IsAttackable(void)
{
	std::vector<CardBase::CARD_TYPE>cardTypes = cardPresent_.GetHandCardType();
	int handCardTypeSize = static_cast<int>(cardPresent_.GetHandCardType().size());
	return handCardTypeSize == 1 && cardTypes[0] == CardBase::CARD_TYPE::ATTACK;
}

bool PlayerCardAction::IsCanComboAttack(void)
{
	return cardPresent_.GetCardUIState()!= CardUIBase::CARD_SELECT::DISITION
		&& actionCntl_.IsCardDecisionControl();
}

void PlayerCardAction::DecideAttackOne(void)
{
	//相手との距離を取得
	const float dis = actionCntl_.GetInput().GetTargetDis();

	//相手との距離が一定以下なら攻撃する
	const float MIDDLE_DIS = 300.0f;

	//距離によって攻撃アクション遷移
	if (dis >= MIDDLE_DIS)
	{
		ChangeCardAction(CARD_ACTION_TYPE::ATTACK_ONE_MIDDLE);
	}
	else
	{
		ChangeCardAction(CARD_ACTION_TYPE::ATTACK_ONE_SHORT);
	}
}

void PlayerCardAction::ChangeActionCardInit(void)
{
	attackStageNum_++;
	cardPresent_.ChangeCard();
	cardFuncs_.pop();
}

void PlayerCardAction::SetUIReloadCnt(void)
{
	float per = pushReloadCnt_ / RELOAD_TIME;
	cardPresent_.SetUIReloadCount(per);
}

void PlayerCardAction::UpdateAttack(void)
{
	//攻撃中にカード負けしたら処理を飛ばす
	if (IsCardFailure(Collider::TAG::NML_ATK))
	{
		cardActType_ = CARD_ACTION_TYPE::NONE;
		return;
	}

	//攻撃の更新
	AttackMotion(atkStatusTable_[cardActType_],Collider::TAG::NML_ATK ,ATK_ONE_LOCAL);
}

void PlayerCardAction::UpdateMiddleAttack(void)
{
	//攻撃中にカード負けしたら処理を飛ばす
	if (IsCardFailure(Collider::TAG::NML_ATK))
	{
		cardActType_ = CARD_ACTION_TYPE::NONE;
		return;
	}
	
	//コンボ先行受付
	ComboInput();

	//キャラ同士で当たったか
	const bool isHitTarget = character_.GetIsHitTarget();
	const float delta = scnMng_.GetDeltaTime();

	//攻撃判定処理
	if (anim_.GetAnimStep(static_cast<int>(CharacterBase::ANIM_TYPE::ATTACK_1_MIDDLE)) >= atk_.colStartStep && midAtkCnt_ > 0.0f) { midAtkCnt_ -= delta; }

	//中距離突きカウントが０以上なら
	if (midAtkCnt_ > 0.0f && !atk_.isDamage)
	{
		//スピード減速
		speed_ = easing_->EaseFunc(ATTACK_ONE_MID_SPD, 0.0f
			, (ATTACK_ONE_MID_TIME - midAtkCnt_) / ATTACK_ONE_MID_TIME
			, Easing::EASING_TYPE::QUAD_OUT);

		//攻撃判定生成
		character_.MakeAttackCol(character_.GetCharaTag(), Collider::TAG::NML_ATK, {}, 0.0f);
	}
	else if (midAtkCnt_ <= 0.0f || character_.GetIsDamage())		//アニメーション終了でアイドル状態変更
	{
		//カウントダウン
		midAtkOverCnt_ -= delta;

		//攻撃判定無効
		speed_ = 0.0f;

		//攻撃判定消去
		character_.DeleteAttackCol(character_.GetCharaTag(), Collider::TAG::NML_ATK);

		//中距離突きが終了したら
		if (midAtkOverCnt_ < 0.0f)
		{
			actionCntl_.ChangeAction(ActionController::ACTION_TYPE::IDLE);
		}
		else
		{
			ChangeComboAction();
		}
	}
}

void PlayerCardAction::UpdateAttackThree(void)
{
}

//void PlayerCardAction::UpdateFireMagic(void)
//{
//	magicFire_->Update();
//}

void PlayerCardAction::UpdateReload(void)
{
	if (actionCntl_.GetInput().GetIsAct().isCardPushKeep)
	{
		//リロードのため時間をカウント
		pushReloadCnt_ += scnMng_.GetDeltaTime();

		//カードUI側にセット
		SetUIReloadCnt();

		//アニメーションループ
		constexpr float LOOP_SPD = 10.0f;
		anim_.SetMidLoop(static_cast<int>(CharacterBase::ANIM_TYPE::CARD_RELOAD), RELOAD_LOOP_START, RELOAD_LOOP_END, LOOP_SPD);
	}
	else
	{
		//アクションをポップし、アイドル状態へ
		cardFuncs_.pop();
		actionCntl_.ChangeAction(ActionController::ACTION_TYPE::IDLE);
	}

	//リロードの溜め終了時、カードをリロードする
	if (pushReloadCnt_ >= RELOAD_TIME)
	{
		cardPresent_.DeckReload();
		cardFuncs_.pop();
		cardActType_ = CARD_ACTION_TYPE::NONE;

		//カードUIのリロードカウントの初期化
		pushReloadCnt_ = 0.0f;
		const float EFF_SCL = 100.0f;

		//UIのリロードカウントを初期化
		SetUIReloadCnt();

		//エフェクトの消去
		effect_->Stop(EffectController::EFF_TYPE::RELOAD, 0);
		effect_->Delete(EffectController::EFF_TYPE::RELOAD, 0);

		//リロード終了エフェクトを再生
		const Transform& trans = character_.GetTransform();
		effect_->Play(EffectController::EFF_TYPE::RELOAD_END, trans.pos, trans.quaRot, { EFF_SCL ,EFF_SCL ,EFF_SCL });

		//カードリロード音停止、完了音再生
		soundMng_.Stop(ResourceManager::SRC::CARD_RELOAD_SE);
		soundMng_.Play(ResourceManager::SRC::CARD_RELOAD_FINISH_SE,SoundManager::PLAYTYPE::BACK);

		//アイドル状態へ
		actionCntl_.ChangeAction(ActionController::ACTION_TYPE::IDLE);
	}
}


void PlayerCardAction::ChangeShortAttackOne(void)
{
	//突きアニメーションへ
	anim_.PlayBlend(static_cast<int>(CharacterBase::ANIM_TYPE::ATTACK_1_SHORT), animVar_);
	atkAnim_ = static_cast<int>(CharacterBase::ANIM_TYPE::ATTACK_1_SHORT);
	cardFuncs_.push([this]() {UpdateAttack(); });
}

void PlayerCardAction::ChangeMiddleAttackOne(void)
{
	//カウントのセット
	midAtkCnt_ = ATTACK_ONE_MID_TIME;

	//近距離攻撃のアニメーションをセット
	anim_.PlayBlend(static_cast<int>(CharacterBase::ANIM_TYPE::ATTACK_1_MIDDLE), animVar_);

	midAtkOverCnt_ = ATTACK_ONE_MID_COMBO_TIME;

	//突き攻撃の方向に向く
	actionCntl_.GetInput().GetLookAtTargetDir();
	isTurnable_ = true;

	cardFuncs_.push([this]() {UpdateMiddleAttack(); });
}

void PlayerCardAction::ChangeAttackTwo(void)
{
	//攻撃2段階目のアニメーションを再生
	anim_.PlayBlend(static_cast<int>(CharacterBase::ANIM_TYPE::ATTACK_2), animVar_);
	atkAnim_ = static_cast<int>(CharacterBase::ANIM_TYPE::ATTACK_2);
	
	//攻撃段階を増やす
	ChangeActionCardInit();


	cardFuncs_.push([this]() {UpdateAttack(); });
}

void PlayerCardAction::ChangeAttackThree(void)
{
	//攻撃3段階目のアニメーションを再生
	anim_.PlayBlend(static_cast<int>(CharacterBase::ANIM_TYPE::ATTACK_3), animVar_);

	//カウントの初期化
	atkThreeEndCnt_ = 0.0f;
	atkAnimLerpCnt_ = 0.0f;

	//カード初期化
	ChangeActionCardInit();

	cardFuncs_.push([this]() {UpdateAttackThree(); });
}

void PlayerCardAction::ChangeReload(void)
{
	if (!cardFuncs_.empty())
	{
		cardFuncs_.pop();
	}

	//現在使っているカードを捨てる
	cardPresent_.FinishCard();

	//UIの状態をリロード待機状態へ
	cardPresent_.ChangeUIState(CardUIBase::CARD_SELECT::RELOAD_WAIT);

	//カードリロードのアニメーション再生(溜めるようなモーション)
	anim_.PlayBlend(static_cast<int>(CharacterBase::ANIM_TYPE::CARD_RELOAD), animVar_);
	atkAnim_ = static_cast<int>(CharacterBase::ANIM_TYPE::CARD_RELOAD);

	//リロード音再生
	soundMng_.Play(ResourceManager::SRC::CARD_RELOAD_SE, SoundManager::PLAYTYPE::LOOP);

	//リロードエフェクトの再生
	const Transform& trans = character_.GetTransform();
	effect_->Play(EffectController::EFF_TYPE::RELOAD, trans.pos, trans.quaRot, { RELOD_EFF_SCL,RELOD_EFF_SCL,RELOD_EFF_SCL }, true);

	cardFuncs_.push([this]() {UpdateReload(); });
}

//void PlayerCardAction::ChangeMagicFire(void)
//{
//	anim_.PlayBlend(static_cast<int>(CharacterBase::ANIM_TYPE::ATTACK_3), animVar_);
//	magicFire_->Init();
//}


void PlayerCardAction::ChangeComboAction(void)
{
	//コンボ入力されていない、もしくはリロードカードならコンボに移行しない
	if (!isCombo_|| cardPresent_.GetCardType() == CardBase::CARD_TYPE::RELOAD)return;

	//コンボフラグをリセット
	isCombo_ = false;

	//攻撃可能かつコンボ攻撃可能なら攻撃段階を上げる
	if (IsAttackable() && IsCanComboAttack())
	{
		//カードを出すときのSEを再生
		soundMng_.Play(ResourceManager::SRC::CARD_PUT_SE, SoundManager::PLAYTYPE::BACK);

		//段階によって遷移する
		if (attackStageNum_ == ATTACK_ONE)
		{
			ChangeCardAction(CARD_ACTION_TYPE::ATTACK_TWO);
		}
		else if (attackStageNum_ == ATTACK_TWO)
		{
			ChangeCardAction(CARD_ACTION_TYPE::ATTACK_THREE);
		}
	}
}

void PlayerCardAction::PushCardFuncs(const std::function<void(void)>& _cardFunc)
{
	cardFuncs_.push(_cardFunc);
}
