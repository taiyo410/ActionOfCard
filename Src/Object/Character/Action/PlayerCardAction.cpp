#include "../Utility/Utility3D.h"
#include "../Common/Easing.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/Camera.h"
#include "../Manager/Resource/ResourceManager.h"
#include "../Object/Common/EffectController.h"
#include "../Player/ActionController.h"
#include "../../Common/AnimationController.h"
#include "../../Card/CardDeck.h"
#include "../../Card/CardBase.h"
#include "../../Card/CardUIBase.h"
#include "../../Card/CardSystem.h"
#include "../Base/CharacterBase.h"
#include "../Base/LogicBase.h"
#include "PlayerCardAction.h"

PlayerCardAction::PlayerCardAction(ActionController& _actCntl, CharacterBase& _charaObj, CardPresenter& _deck):
	CardActionBase(_actCntl, _charaObj, _deck),
	pushReloadCnt_()
{
	isTurnable_ = false;
	changeAction_={
		{ CARD_ACT_TYPE::ATTACK_ONE_SHORT, [this]() {ChangeShortAttackOne(); }},
		{ CARD_ACT_TYPE::ATTACK_ONE_MIDDLE, [this]() {ChangeMiddleAttackOne(); }},
		{ CARD_ACT_TYPE::ATTACK_TWO, [this]() {ChangeAttackTwo(); }},
		{ CARD_ACT_TYPE::ATTACK_THREE, [this]() {ChangeAttackThree(); }},
		{ CARD_ACT_TYPE::RELOAD, [this]() {ChangeReload(); }},
	};

	atkStatusStrTable_ = {
		{CARD_ACT_TYPE::ATTACK_ONE_SHORT,"AttackOneShort"},
		{CARD_ACT_TYPE::ATTACK_ONE_MIDDLE,"AttackOneMiddle"},
		{CARD_ACT_TYPE::ATTACK_TWO,"AttackTwo"},
		{CARD_ACT_TYPE::ATTACK_THREE,"AttackThree"}
	};

	atk_ = {};

	easing_ = std::make_unique<Easing>();
}

PlayerCardAction::~PlayerCardAction(void)
{
	changeAction_.clear();

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

	//使用エフェクトの追加
	effect_->Add(ResourceManager::GetInstance().Load(ResourceManager::SRC::RELOAD_EFF).handleId_, EffectController::EFF_TYPE::RELOAD);
	effect_->Add(ResourceManager::GetInstance().Load(ResourceManager::SRC::RELOAD_END_EFF).handleId_, EffectController::EFF_TYPE::RELOAD_END);

	//攻撃ステータスロード
	LoadAttack();
}

void PlayerCardAction::Init(void)
{
	attackStageNum_ = 0;
	atk_.isDamage = false;
	speed_ = 0.0f;
	isCombo_ = false;

	//キーによる移動はしない
	charaObj_.SetIsCanMoveable(false);

	if (cardPresent_.GetCardType() == CardBase::CARD_TYPE::ATTACK)
	{
		//カードの移動SEを再生
		soundMng_.Play(ResourceManager::SRC::CARD_PUT_SE, SoundManager::PLAYTYPE::BACK);

		//手札に移動
		cardPresent_.PutCard();

		//攻撃処理へ
		DecideAttackOne();
	}
	else if (cardPresent_.GetCardType()==CardBase::CARD_TYPE::RELOAD)
	{
		//リロード処理へ
		ChangeCardAction(CARD_ACT_TYPE::RELOAD);
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
	charaObj_.DeleteAttackCol(Collider::TAG::PLAYER1,Collider::TAG::NML_ATK);
	charaObj_.SetIsCanMoveable(true);

	//リロード中にアクションが中断された場合
	if (actType_ == CARD_ACT_TYPE::RELOAD)
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

	actType_ = CARD_ACT_TYPE::NONE;
}

void PlayerCardAction::ReleaseReloadResource(void)
{
	//カードリロード音停止
	soundMng_.Stop(ResourceManager::SRC::CARD_RELOAD_SE);

	//エフェクトの停止
	effect_->Stop(EffectController::EFF_TYPE::RELOAD, 0);
	effect_->Delete(EffectController::EFF_TYPE::RELOAD, 0);
	actType_ = CARD_ACT_TYPE::NONE;
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
		ChangeCardAction(CARD_ACT_TYPE::ATTACK_ONE_MIDDLE);
	}
	else
	{
		ChangeCardAction(CARD_ACT_TYPE::ATTACK_ONE_SHORT);
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

void PlayerCardAction::LoadAttack(void)
{
	for (const auto& atk : atkStatusStrTable_)
	{
		LoadAttackStatus(atkStatusTable_[atk.first], atk.second);
	}
}

void PlayerCardAction::UpdateAttack(void)
{
	//攻撃の更新
	AttackMotion(atkStatusTable_[actType_],Collider::TAG::NML_ATK ,ATK_ONE_LOCAL);
}

void PlayerCardAction::UpdateMiddleAttack(void)
{
	//攻撃中にカード負けしたら処理を飛ばす
	if (IsCardFailure(Collider::TAG::NML_ATK))return;

	//コンボ先行受付
	ComboInput();

	//キャラ同士で当たったか
	const bool isHitTarget = charaObj_.GetIsHitTarget();
	const float delta = scnMng_.GetDeltaTime();

	//攻撃判定処理
	if (anim_.GetAnimStep() >= atk_.colStartCnt && midAtkCnt_ > 0.0f) { midAtkCnt_ -= delta; }

	//中距離突きカウントが０以上なら
	if (midAtkCnt_ > 0.0f && !atk_.isDamage)
	{
		//スピード減速
		speed_ = easing_->EaseFunc(ATTACK_ONE_MID_SPD, 0.0f
			, (ATTACK_ONE_MID_TIME - midAtkCnt_) / ATTACK_ONE_MID_TIME
			, Easing::EASING_TYPE::QUAD_OUT);

		//攻撃判定生成
		charaObj_.MakeAttackCol(charaObj_.GetCharaTag(), Collider::TAG::NML_ATK, {}, 0.0f);
	}
	else if (midAtkCnt_ <= 0.0f || charaObj_.GetIsDamage())		//アニメーション終了でアイドル状態変更
	{
		//カウントダウン
		midAtkOverCnt_ -= delta;

		//攻撃判定無効
		speed_ = 0.0f;

		//攻撃判定消去
		charaObj_.DeleteAttackCol(charaObj_.GetCharaTag(), Collider::TAG::NML_ATK);

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
	//攻撃中にカード負けしたら処理を飛ばす
	if (IsCardFailure(Collider::TAG::NML_ATK))return;

	//ステップの取得
	const float animStep = anim_.GetAnimStep();							//現在のアニメステップ
	const float atkStartStep = atkStatusTable_[actType_].colStartCnt;	//当たり判定スタートカウント
	const float atkEndStep = atkStatusTable_[actType_].colEndCnt;		//当たり判定終了カウント

	//攻撃スタートカウント以下なら、アニメーションスピードを遅くする
	if (animStep < atkStartStep)
	{
		anim_.SetAnimSpeed(ATTACK_THREE_ANIM_SPD);
	}

	//攻撃判定処理
	else if (animStep >= atkStartStep &&
		animStep <= atkEndStep)
	{
		//速度を自然に見せるための補完
		atkAnimLerpCnt_ += scnMng_.GetDeltaTime();

		//アニメーション速度補完
		anim_.SetAnimSpeed(CharacterBase::DEFAULT_ANIM_SPEED, true
			, ATTACK_THREE_ANIM_SPD, atkAnimLerpCnt_ / ATTACK_THREE_ANIM_LERP_TIME,Easing::EASING_TYPE::QUAD_IN);

		//攻撃判定有効
		isAliveAtkCol_ = true;
		charaObj_.MakeAttackCol(charaObj_.GetCharaTag(), Collider::TAG::NML_ATK, {}, 0.0f);

	}
	else if (anim_.IsEnd())		//アニメーション終了でアイドル状態変更
	{
		//攻撃終了時間以上なら、アイドル状態へ
		const float ATK_END_CNT = 0.5f;
		if (atkThreeEndCnt_ > ATK_END_CNT)
		{
			actionCntl_.ChangeAction(ActionController::ACTION_TYPE::IDLE);
			return;
		}

		//攻撃３段階カのウント
		atkThreeEndCnt_+= scnMng_.GetDeltaTime();

		//カメラシェイク
		scnMng_.GetCamera().lock()->SetShakeStatus(atkThreeEndCnt_ / ATK_END_CNT, 50.0f, Easing::EASING_TYPE::ELASTIC_BACK);
		scnMng_.GetCamera().lock()->ChangeSub(Camera::SUB_MODE::ONE_SHAKE);

	}
	else if (animStep > atkEndStep)	//攻撃終了後
	{
		//攻撃判定無効
		charaObj_.DeleteAttackCol(charaObj_.GetCharaTag(), Collider::TAG::NML_ATK);
	}
}

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
		anim_.SetMidLoop(RELOAD_LOOP_START, RELOAD_LOOP_END, LOOP_SPD);
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
		actType_ = CARD_ACT_TYPE::NONE;

		//カードUIのリロードカウントの初期化
		pushReloadCnt_ = 0.0f;
		const float EFF_SCL = 100.0f;

		//UIのリロードカウントを初期化
		SetUIReloadCnt();

		//エフェクトの消去
		effect_->Stop(EffectController::EFF_TYPE::RELOAD, 0);
		effect_->Delete(EffectController::EFF_TYPE::RELOAD, 0);

		//リロード終了エフェクトを再生
		const Transform& trans = charaObj_.GetTransform();
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
	anim_.Play(static_cast<int>(CharacterBase::ANIM_TYPE::ATTACK_1_SHORT), false);

	//攻撃ステータスをセット
	atk_ = atkStatusTable_[CARD_ACT_TYPE::ATTACK_ONE_SHORT];

	cardFuncs_.push([this]() {UpdateAttack(); });
}

void PlayerCardAction::ChangeMiddleAttackOne(void)
{
	//カウントのセット
	midAtkCnt_ = ATTACK_ONE_MID_TIME;

	//近距離攻撃のアニメーションをセット
	anim_.Play(static_cast<int>(CharacterBase::ANIM_TYPE::ATTACK_1_MIDDLE), false
		, ATTACK_ONE_MID_ANIM_START, ATTACK_ONE_MID_ANIM_END,false);

	//攻撃ステータス
	atk_ = atkStatusTable_[CARD_ACT_TYPE::ATTACK_ONE_MIDDLE];
	midAtkOverCnt_ = ATTACK_ONE_MID_COMBO_TIME;

	//突き攻撃の方向に向く
	actionCntl_.GetInput().GetLookAtTargetDir();
	isTurnable_ = true;

	cardFuncs_.push([this]() {UpdateMiddleAttack(); });
}

void PlayerCardAction::ChangeAttackTwo(void)
{
	//攻撃2段階目のアニメーションを再生
	anim_.Play(static_cast<int>(CharacterBase::ANIM_TYPE::ATTACK_2), false);

	//攻撃段階を増やす
	ChangeActionCardInit();

	//攻撃2段階目のステータスをセット
	atk_ = atkStatusTable_[CARD_ACT_TYPE::ATTACK_TWO];

	cardFuncs_.push([this]() {UpdateAttack(); });
}

void PlayerCardAction::ChangeAttackThree(void)
{
	//攻撃3段階目のアニメーションを再生
	anim_.Play(static_cast<int>(CharacterBase::ANIM_TYPE::ATTACK_3), false, ATTACK_THREE_ANIM_START, ATTACK_THREE_ANIM_GOAL);

	//カウントの初期化
	atkThreeEndCnt_ = 0.0f;
	atkAnimLerpCnt_ = 0.0f;

	//攻撃3段階目のステータスをセット
	atk_ = atkStatusTable_[CARD_ACT_TYPE::ATTACK_THREE];

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
	anim_.Play(static_cast<int>(CharacterBase::ANIM_TYPE::CARD_RELOAD), true, RELOAD_START_STEP, RELOAD_END_STEP);

	//リロード音再生
	soundMng_.Play(ResourceManager::SRC::CARD_RELOAD_SE, SoundManager::PLAYTYPE::LOOP);

	//リロードエフェクトの再生
	const Transform& trans = charaObj_.GetTransform();
	effect_->Play(EffectController::EFF_TYPE::RELOAD, trans.pos, trans.quaRot, { RELOD_EFF_SCL,RELOD_EFF_SCL,RELOD_EFF_SCL }, true);

	cardFuncs_.push([this]() {UpdateReload(); });
}

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
			ChangeCardAction(CARD_ACT_TYPE::ATTACK_TWO);
		}
		else if (attackStageNum_ == ATTACK_TWO)
		{
			ChangeCardAction(CARD_ACT_TYPE::ATTACK_THREE);
		}
	}
}
