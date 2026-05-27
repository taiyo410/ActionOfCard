#include "../pch.h"
#include "../Utility/Utility3D.h"
#include "../Common/Easing.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/Camera.h"
#include "../Manager/Resource/ResourceManager.h"
#include "../Object/Common/EffectController.h"
#include "../Player/ActionController.h"
#include "../Action/PlayerFireMagicAction.h"
#include "../../Common/AnimationController.h"
#include "../../Card/CardDeck.h"
#include "../../Card/CardBase.h"
#include "../../Card/CardUIBase.h"
#include "../../Card/CardSystem.h"
#include "../Base/CharacterBase.h"
#include "../Base/LogicBase.h"
#include "PlayerCardAttackAction.h"

PlayerCardAttackAction::PlayerCardAttackAction(ActionController& _actCntl, CharacterBase& _charaObj, CardPresenter& _deck):
	CardActionBase(_actCntl, _charaObj, _deck)
{
	isTurnable_ = false;
	//changeCardAction_ = {
	//	{ CARD_ACTION_TYPE::ATTACK_ONE_SHORT, [this]() {ChangeShortAttackOne(); }},
	//	{ CARD_ACTION_TYPE::ATTACK_ONE_MIDDLE, [this]() {ChangeMiddleAttackOne(); }},
	//	{ CARD_ACTION_TYPE::ATTACK_TWO, [this]() {ChangeAttackTwo(); }},
	//	{ CARD_ACTION_TYPE::ATTACK_THREE, [this]() {ChangeAttackThree(); }},
	//	//{ CARD_ACTION_TYPE::MAGIC_FIRE, [this]() {ChangeMagicFire(); }},
	//	{ CARD_ACTION_TYPE::RELOAD, [this]() {ChangeReload(); }},
	//};
	//attackActionStr_ = {
	//	{"Attack_1_Short", CARD_ACTION_TYPE::ATTACK_ONE_SHORT},
	//	{"Attack_1_Middle", CARD_ACTION_TYPE::ATTACK_ONE_MIDDLE},
	//	{"Attack_2", CARD_ACTION_TYPE::ATTACK_TWO},
	//	{"Attack_3", CARD_ACTION_TYPE::ATTACK_THREE},
	//	//{"FireMagic", CARD_ACTION_TYPE::MAGIC_FIRE},
	//	{"Reload", CARD_ACTION_TYPE::RELOAD}
	//};
	atk_ = {};
	easing_ = std::make_unique<Easing>();

}

PlayerCardAttackAction::~PlayerCardAttackAction(void)
{
}

void PlayerCardAttackAction::Load(void)
{
}

void PlayerCardAttackAction::Init(void)
{
}

void PlayerCardAttackAction::Update(void)
{
}

void PlayerCardAttackAction::Release(void)
{
}

void PlayerCardAttackAction::LoadAnimVar(const ACTION_LOAD_DATA& animVar)
{
}
void PlayerCardAttackAction::UpdateAttack(void)
{
	//攻撃中にカード負けしたら処理を飛ばす
	if (IsCardFailure(Collider::TAG::NML_ATK))
	{
		cardActType_ = CARD_ACTION_TYPE::NONE;
		return;
	}

	//攻撃の更新
	AttackMotion(atkStatusTable_[cardActType_], Collider::TAG::NML_ATK, ATK_ONE_LOCAL);
}

//void PlayerCardAttackAction::ChangeCardAction(const CARD_ACTION_TYPE _type)
//{
//	if (cardActType_ == _type)return;
//	cardActType_ = _type;
//
//	//攻撃ステータスのセット
//	atk_ = atkStatusTable_[cardActType_];
//
//	//アニメーション変数のセット
//	animVar_ = atkAnimVals_[cardActType_];
//
//	changeCardAction_[cardActType_]();
//}

void PlayerCardAttackAction::UpdateMiddleAttack(void)
{
	//攻撃中にカード負けしたら処理を飛ばす
	if (IsCardFailure(Collider::TAG::NML_ATK))
	{
		//cardActType_ = CARD_ACTION_TYPE::NONE;
		return;
	}

	//コンボ先行受付
	ComboInput();

	//キャラ同士で当たったか
	const bool isHitTarget = character_.GetIsHitTarget();
	const float delta = scnMng_.GetDeltaTime();

	//攻撃判定処理
	if (anim_.GetAnimStep(static_cast<int>(CharacterBase::ANIM_TYPE::ATTACK_1_MIDDLE)) >= atk_.colStartCnt && midAtkCnt_ > 0.0f) { midAtkCnt_ -= delta; }

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

void PlayerCardAttackAction::UpdateAttackThree(void)
{
	//攻撃中にカード負けしたら処理を飛ばす
	if (IsCardFailure(Collider::TAG::NML_ATK))
	{
		cardActType_ = CARD_ACTION_TYPE::NONE;
		return;
	}

	//ステップの取得
	const float animStep = anim_.GetAnimStep(static_cast<int>(CharacterBase::ANIM_TYPE::ATTACK_3));							//現在のアニメステップ
	const float atkStartStep = atkStatusTable_[cardActType_].colStartCnt;	//当たり判定スタートカウント
	const float atkEndStep = atkStatusTable_[cardActType_].colEndCnt;		//当たり判定終了カウント

	//攻撃スタートカウント以下なら、アニメーションスピードを遅くする
	if (animStep < atkStartStep)
	{
		anim_.SetAnimSpeed(static_cast<int>(CharacterBase::ANIM_TYPE::ATTACK_3), ATTACK_THREE_ANIM_SPD);
	}

	//攻撃判定処理
	else if (animStep >= atkStartStep &&
		animStep <= atkEndStep)
	{
		//速度を自然に見せるための補完
		atkAnimLerpCnt_ += scnMng_.GetDeltaTime();

		//アニメーション速度補完
		anim_.SetAnimSpeed(static_cast<int>(CharacterBase::ANIM_TYPE::ATTACK_3), CharacterBase::DEFAULT_ANIM_SPEED, true
			, ATTACK_THREE_ANIM_SPD, atkAnimLerpCnt_ / ATTACK_THREE_ANIM_LERP_TIME, Easing::EASING_TYPE::QUAD_IN);

		//攻撃判定有効
		isAliveAtkCol_ = true;
		character_.MakeAttackCol(character_.GetCharaTag(), Collider::TAG::NML_ATK, {}, 0.0f);

	}
	else if (anim_.IsEnd(static_cast<int>(CharacterBase::ANIM_TYPE::ATTACK_3)))		//アニメーション終了でアイドル状態変更
	{
		//攻撃終了時間以上なら、アイドル状態へ
		const float ATK_END_CNT = 0.5f;
		if (atkThreeEndCnt_ > ATK_END_CNT)
		{
			actionCntl_.ChangeAction(ActionController::ACTION_TYPE::IDLE);
			return;
		}

		//攻撃３段階カのウント
		atkThreeEndCnt_ += scnMng_.GetDeltaTime();

		//カメラシェイク
		scnMng_.GetCamera().lock()->SetShakeStatus(atkThreeEndCnt_ / ATK_END_CNT, 50.0f, Easing::EASING_TYPE::ELASTIC_BACK);
		scnMng_.GetCamera().lock()->ChangeSub(Camera::SUB_MODE::ONE_SHAKE);
	}
	else if (animStep > atkEndStep)	//攻撃終了後
	{
		//攻撃判定無効
		character_.DeleteAttackCol(character_.GetCharaTag(), Collider::TAG::NML_ATK);
	}
}

void PlayerCardAttackAction::UpdateReload(void)
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
		soundMng_.Play(ResourceManager::SRC::CARD_RELOAD_FINISH_SE, SoundManager::PLAYTYPE::BACK);

		//アイドル状態へ
		actionCntl_.ChangeAction(ActionController::ACTION_TYPE::IDLE);
	}
}


void PlayerCardAttackAction::ChangeShortAttackOne(void)
{
	//突きアニメーションへ
	anim_.PlayBlend(static_cast<int>(CharacterBase::ANIM_TYPE::ATTACK_1_SHORT), animVar_);
	atkAnim_ = static_cast<int>(CharacterBase::ANIM_TYPE::ATTACK_1_SHORT);
	cardFuncs_.push([this]() {UpdateAttack(); });
}

void PlayerCardAttackAction::ChangeMiddleAttackOne(void)
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

void PlayerCardAttackAction::ChangeAttackTwo(void)
{
	//攻撃2段階目のアニメーションを再生
	anim_.PlayBlend(static_cast<int>(CharacterBase::ANIM_TYPE::ATTACK_2), animVar_);
	atkAnim_ = static_cast<int>(CharacterBase::ANIM_TYPE::ATTACK_2);

	//攻撃段階を増やす
	ChangeActionCardInit();


	cardFuncs_.push([this]() {UpdateAttack(); });
}

void PlayerCardAttackAction::ChangeAttackThree(void)
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

void PlayerCardAttackAction::ChangeReload(void)
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