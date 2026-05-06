#include "../Utility/Utility3D.h"
#include "../Common/Easing.h"
#include"../Player/ActionController.h"
#include "../Base/CharacterBase.h"
#include"../Enemy/EnemyLogic.h"
#include"../Enemy/EnemyRock.h"
#include "../../Common/AnimationController.h"
#include "../Object/Common/EffectController.h"
#include "../Manager/Generic/Camera.h"
#include "../Manager/Resource/ResourceManager.h"
#include "../Manager/Generic/SceneManager.h"
#include "../../Card/CardDeck.h"
#include "../../Card/CardBase.h"
#include "../../Card/CardUIBase.h"
#include "../../Card/CardSystem.h"
#include "EnemyCardAction.h"

EnemyCardAction::EnemyCardAction(ActionController& _actCntl, CharacterBase& _charaObj, CardPresenter& _deck):
CardActionBase(_actCntl, _charaObj, _deck),
atkCnt_(),
jampCardNum_()
{
	isTurnable_ = false;
	changeAction_ = {
		{ CARD_ACT_TYPE::STOMP_ATK, [this]() {ChangeStomp(); }},
		{ CARD_ACT_TYPE::JUMP_ATK, [this]() {ChangeJumpAtk(); }},
		{ CARD_ACT_TYPE::RELOAD, [this]() {ChangeReload(); }},
	};

	changeCardAction_ = {
		{LogicBase::ENEMY_ATTACK_TYPE::STOMP,[this]() {ChangeCardAction(CARD_ACT_TYPE::STOMP_ATK); }},
		{LogicBase::ENEMY_ATTACK_TYPE::JUMP,[this]() {ChangeCardAction(CARD_ACT_TYPE::JUMP_ATK); }},
	};

	atkStatusStrTable_ = {
	{CARD_ACT_TYPE::JUMP_ATK,"JumpAttack"},
	{CARD_ACT_TYPE::STOMP_ATK,"StompAttack"}
	};

	//岩の生成
	charaObj_.AddEnemyRock(STOMP_ATK_ROCK_NUM, atk_.pos);
}

EnemyCardAction::~EnemyCardAction(void)
{
}

void EnemyCardAction::Load(void)
{
	//リソースのロード
	resMng_.Load(ResourceManager::SRC::ENEMY_JUMP_LAND_SE);
	resMng_.Load(ResourceManager::SRC::ENEMY_CHARGE_SE);
	resMng_.Load(ResourceManager::SRC::ENEMY_STOMP_SE);

	//エフェクトの追加
	effect_->Add(ResourceManager::GetInstance().Load(ResourceManager::SRC::BLAST).handleId_,EffectController::EFF_TYPE::BLAST);
	effect_->Add(ResourceManager::GetInstance().Load(ResourceManager::SRC::E_JUMP_CHARGE_EFF).handleId_,EffectController::EFF_TYPE::E_JUMP_CHARGE);

	//敵の岩生成
	charaObj_.LoadEnemyRock();

	//攻撃ステータスロード
	LoadStatus();

}

void EnemyCardAction::Init(void)
{
	actType_ = CARD_ACT_TYPE::NONE;
	easing_ = std::make_unique<Easing>();

	atkCnt_ = 0.0f;
	speed_ = 0.0f;
	jampCardNum_ = 0;
	atk_ = {};
	atk_.isDamage = false;

	//カード種類を判断して遷移するカードアクションを決める
	if (cardPresent_.GetCardType() == CardBase::CARD_TYPE::ATTACK)
	{
		cardPresent_.PutCard();
		LogicBase::ENEMY_ATTACK_TYPE attackType = actionCntl_.GetInput().GetAttackType();
		changeCardAction_[attackType]();
	}
	else if (cardPresent_.GetCardType() == CardBase::CARD_TYPE::RELOAD)
	{
		ChangeCardAction(CARD_ACT_TYPE::RELOAD);
	}
}

void EnemyCardAction::Update(void)
{
	cardFuncs_.front()();
}

void EnemyCardAction::Release(void)
{
	//当たり判定の消去
	charaObj_.DeleteAttackCol(Collider::TAG::ENEMY1, Collider::TAG::NML_ATK);
	charaObj_.DeleteAttackCol(Collider::TAG::ENEMY1, Collider::TAG::JUMP_ATK);
	
	//カメラシェイクを元に戻す
	scnMng_.GetCamera().lock()->ChangeSub(Camera::SUB_MODE::NONE);

	actionCntl_.GetInput().IsActioningSet();
	//カード機能配列の解放
	if (!cardFuncs_.empty())
	{
		cardFuncs_.pop();
	}

	//現在使っているカードを捨てる
	cardPresent_.FinishCard();

	//硬直時間セット
	actionCntl_.GetInput().SetFreezeCntByAttackType();
}

const bool EnemyCardAction::IsJumpAtkCharge(void) const
{
	return jumpChargeCnt_ < JUMP_CHARGE_TIME; 
}

void EnemyCardAction::ChangeStomp(void)
{
	//岩生成フラグの初期化
	isGenerateRock_ = false;

	//ジャンプ攻撃処理
	atk_ = atkStatusTable_[CARD_ACT_TYPE::STOMP_ATK];

	//スタンプアニメーション再生
	anim_.Play(static_cast<int>(CharacterBase::ANIM_TYPE::STOMP_ATK), false);

	cardFuncs_.push([this]() {UpdateStomp(); });
}

void EnemyCardAction::ChangeJumpAtk(void)
{
	//ジャンプアニメーション再生
	anim_.Play(static_cast<int>(CharacterBase::ANIM_TYPE::JUMP_ATK), false);
	jumpChargeCnt_ = 0.0f;

	//溜めジャンプSE再生
	soundMng_.Play(ResourceManager::SRC::ENEMY_CHARGE_SE, SoundManager::PLAYTYPE::LOOP);

	//ジャンプ攻撃処理
	atk_ = atkStatusTable_[CARD_ACT_TYPE::JUMP_ATK];

	//溜めジャンプエフェクト再生
	effect_->Play(EffectController::EFF_TYPE::E_JUMP_CHARGE,
		charaObj_.GetTransform().pos,
		charaObj_.GetTransform().quaRot,
		{ JUMP_CHARGE_EFF_SCL,
		JUMP_CHARGE_EFF_SCL ,
		JUMP_CHARGE_EFF_SCL });

	cardFuncs_.push([this]() {UpdateJumpAtk(); });
}

void EnemyCardAction::ChangeReload(void)
{
	//カードアクション配列初期化
	if (!cardFuncs_.empty())
	{
		cardFuncs_.pop();
	}
	//現在使っているカードを捨てる
	cardPresent_.EnemyCardReload();

	//アニメーションの再生
	anim_.Play(static_cast<int>(CharacterBase::ANIM_TYPE::RUSH_ATK), true);

	cardFuncs_.push([this]() {UpdateReload(); });
}

void EnemyCardAction::UpdateStomp(void)
{
	//負けたら、岩のモデルや当たり判定を消去
	if (IsCardFailure(Collider::TAG::NML_ATK))
	{
		scnMng_.GetCamera().lock()->ChangeSub(Camera::SUB_MODE::NONE);
		charaObj_.DeleteEnemyRockCol();
		charaObj_.SetIsAliveEnemyRock(false);
		return;
	}

	//アニメーションで足をつく動作をしたら
	if (anim_.GetAnimStep() > atk_.colStartCnt)
	{
		//キャラ情報
		const Transform& charaTrans = charaObj_.GetTransform();

		//攻撃中
		atk_.pos = Utility3D::AddPosRotate(charaTrans.pos, charaTrans.quaRot, { 0.0f,0.0f,0.0f });

		//カメラシェイク制限値
		constexpr float CAMERA_SHAKE_LIMIT = 30.0f;

		//溜めのカメラシェイク
		atkCnt_ += SceneManager::GetInstance().GetDeltaTime();
		scnMng_.GetCamera().lock()->SetShakeStatus(atkCnt_ / STOMP_ATK_SHAKE_CNT, CAMERA_SHAKE_LIMIT);
		scnMng_.GetCamera().lock()->ChangeSub(Camera::SUB_MODE::SHAKE);

		//地響き音再生
		const bool isPlayStompSE_ = soundMng_.IsPlay(ResourceManager::SRC::ENEMY_STOMP_SE);
		if (!isPlayStompSE_)
		{
			soundMng_.Play(ResourceManager::SRC::ENEMY_STOMP_SE, SoundManager::PLAYTYPE::BACK);
		}

		//地面から岩の玉を生成してあらゆる方向に飛ばす
		if (!isGenerateRock_)
		{
			charaObj_.SetIsAliveEnemyRock(true);
			isGenerateRock_ = true;
		}
		else
		{
			charaObj_.EnemyRockUpdate();
		}

		//ため時間が終わったら
		if (atkCnt_ > STOMP_ATK_SHAKE_CNT)
		{
			atkCnt_ = 0.0f;
			atk_.atkRadius = JUMP_ATK_START_RADIUS;

			//アニメーションループ終了
			anim_.SetEndMidLoop(CharacterBase::DEFAULT_ANIM_SPEED);
			charaObj_.DeleteEnemyRockCol();
			charaObj_.SetIsAliveEnemyRock(false);

			//敵の岩の攻撃判定終了
			charaObj_.DeleteEnemyRockCol();
			charaObj_.DeleteAttackCol(Collider::TAG::ENEMY1, Collider::TAG::NML_ATK);

			//エフェクトの終了
			effect_->Stop(EffectController::EFF_TYPE::JUMP, 0);
			effect_->Delete(EffectController::EFF_TYPE::JUMP, 0);

			//カード終了処理
			cardPresent_.FinishCard();

			//アイドル状態へ
			actionCntl_.ChangeAction(ActionController::ACTION_TYPE::IDLE);
		}
	}
}

void EnemyCardAction::UpdateJumpAtk(void)
{
	////負けたら終了
	if (IsCardFailure(Collider::TAG::NML_ATK))
	{
		//カメラシェイクを止める
		scnMng_.GetCamera().lock()->ChangeSub(Camera::SUB_MODE::NONE);

		//音を止める
		soundMng_.Stop(ResourceManager::SRC::ENEMY_CHARGE_SE);

		//エフェクトを消去
		const int JUMP_CHARGE_EFF_ARRAY = 0;
		effect_->Stop(EffectController::EFF_TYPE::E_JUMP_CHARGE, JUMP_CHARGE_EFF_ARRAY);
		effect_->Delete(EffectController::EFF_TYPE::E_JUMP_CHARGE, JUMP_CHARGE_EFF_ARRAY);
		return;
	}

	//ジャンプチャージ
	if (jumpChargeCnt_ < JUMP_CHARGE_TIME)
	{
		//攻撃カウント
		jumpChargeCnt_ += SceneManager::GetInstance().GetDeltaTime();

		//アニメーションループ
		anim_.SetMidLoop(JUMP_CHARGE_ANIM_LOOP_START, JUMP_CHARGE_ANIM_LOOP_END, JUMP_ATK_ANIM_LOOP_SPEED);

		//溜めのカメラシェイク
		scnMng_.GetCamera().lock()->SetShakeStatus(jumpChargeCnt_ / JUMP_CHARGE_TIME, JUMP_CHARGE_CAMERA_SHAKE_LIMIT);
		scnMng_.GetCamera().lock()->ChangeSub(Camera::SUB_MODE::SHAKE);

		//ジャンプチャージが終わったら
		if (jumpChargeCnt_ >= JUMP_CHARGE_TIME)
		{
			//アニメーションループ終了
			anim_.SetEndMidLoop(CharacterBase::DEFAULT_ANIM_SPEED);
			soundMng_.Stop(ResourceManager::SRC::ENEMY_CHARGE_SE);

			//チャージエフェクトの消去
			const int JUMP_CHARGE_EFF_ARRAY = 0;
			effect_->Stop(EffectController::EFF_TYPE::E_JUMP_CHARGE, JUMP_CHARGE_EFF_ARRAY);
			effect_->Delete(EffectController::EFF_TYPE::E_JUMP_CHARGE, JUMP_CHARGE_EFF_ARRAY);

			//カードを終了状態にし、プレイヤーの攻撃で中断できないようにする
			cardPresent_.FinishCard();
		}
	}

	//ジャンプアニメーションが終わったらドーム型の攻撃をする
	else if (anim_.GetAnimStep() > JUMP_ANIM_END)
	{
		const Transform& charaTrans = charaObj_.GetTransform();

		//攻撃カウント
		atkCnt_ += SceneManager::GetInstance().GetDeltaTime();

		//攻撃中
		atk_.pos = Utility3D::AddPosRotate(charaTrans.pos, charaTrans.quaRot, { 0.0f,0.0f,0.0f });

		//攻撃判定有効
		isAliveAtkCol_ = true;
		charaObj_.MakeAttackCol(charaObj_.GetCharaTag(), Collider::TAG::NML_ATK, atk_.pos, atk_.atkRadius);

		//攻撃範囲拡大
		atk_.atkRadius = easing_->EaseFunc(JUMP_ATK_START_RADIUS, JUMP_ATK_GOAL_RADIUS, atkCnt_ / JUMP_ATK_CNT_MAX, Easing::EASING_TYPE::QUAD_IN);
		charaObj_.UpdateAttackCol(atk_.atkRadius);

		//溜めのカメラシェイク()
		scnMng_.GetCamera().lock()->SetShakeStatus(atkCnt_ / JUMP_ATK_CNT_MAX, JUMP_ATTACK_CAMERA_SHAKE_LIMIT);
		scnMng_.GetCamera().lock()->ChangeSub(Camera::SUB_MODE::SHAKE);

		//サウンド再生
		if (!soundMng_.IsPlay(ResourceManager::SRC::ENEMY_JUMP_LAND_SE))
		{
			soundMng_.Play(ResourceManager::SRC::ENEMY_JUMP_LAND_SE, SoundManager::PLAYTYPE::BACK);
			effect_->Play(EffectController::EFF_TYPE::BLAST, atk_.pos, {},{ atk_.atkRadius,atk_.atkRadius,atk_.atkRadius},true);
		}

		//エフェクトの大きさを大きくしていく
		effect_->SetScale(EffectController::EFF_TYPE::BLAST, static_cast<int>(EFF_TYPE::BLAST),
			{ atk_.atkRadius * BLAST_EFF_SCL,atk_.atkRadius * BLAST_EFF_SCL,atk_.atkRadius * BLAST_EFF_SCL });

		//攻撃アニメーションループ
		anim_.SetMidLoop(JUMP_ATK_ANIM_LOOP_START, JUMP_ATK_ANIM_LOOP_END, JUMP_ATK_ANIM_ATTACK_LOOP_SPEED);

		//攻撃時間が終わったら
		if (atkCnt_ > JUMP_ATK_CNT_MAX)
		{
			atkCnt_ = 0.0f;
			atk_.atkRadius = JUMP_ATK_START_RADIUS;

			//アニメーションループ終了
			anim_.SetEndMidLoop(CharacterBase::DEFAULT_ANIM_SPEED);

			//当たり判定の消去
			charaObj_.DeleteAttackCol(Collider::TAG::ENEMY1, Collider::TAG::NML_ATK);

			//エフェクトの消去
			effect_->Delete(EffectController::EFF_TYPE::BLAST,static_cast<int>(EFF_TYPE::BLAST));

			//アイドル状態へ移行
			actionCntl_.ChangeAction(ActionController::ACTION_TYPE::IDLE);
		}
	}
}

void EnemyCardAction::UpdateReload(void)
{
	cardPresent_.EnemyCardReload();
	actType_ = CARD_ACT_TYPE::NONE;
	actionCntl_.ChangeAction(ActionController::ACTION_TYPE::IDLE);
}


