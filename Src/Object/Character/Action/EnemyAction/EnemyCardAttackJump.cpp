#include "../pch.h"
#include "../../../Common/AnimationController.h"
#include "../Object/Common/EffectController.h"
#include "../Utility/Utility3D.h"
#include "../Manager/Generic/Camera.h"
#include "../Manager/Resource/ResourceManager.h"
#include "../Manager/Resource/SoundManager.h"
#include "../Manager/Generic/SceneManager.h"
#include "../ActionController.h"
#include "../../Base/CharacterBase.h"
#include "EnemyCardAttackJump.h"

EnemyCardAttackJump::EnemyCardAttackJump(ActionController& _actCntl, CharacterBase& _charaObj, CardPresenter& _cardPresent):
	EnemyCardAttackBase(_actCntl, _charaObj, _cardPresent)
{
	easing_ = std::make_unique<Easing>();
}

EnemyCardAttackJump::~EnemyCardAttackJump(void)
{
}

void EnemyCardAttackJump::Load(void)
{
	//リソースのロード
	resMng_.Load(ResourceManager::SRC::ENEMY_JUMP_LAND_SE);
	resMng_.Load(ResourceManager::SRC::ENEMY_CHARGE_SE);

	//エフェクトの追加
	effect_->Add(ResourceManager::GetInstance().Load(ResourceManager::SRC::BLAST).handleId_, EffectController::EFF_TYPE::BLAST);
	effect_->Add(resMng_.Load(ResourceManager::SRC::E_JUMP_CHARGE_EFF).handleId_, EffectController::EFF_TYPE::E_JUMP_CHARGE);
}

void EnemyCardAttackJump::InitAttack(void)
{
	//ジャンプアニメーション再生
	anim_.PlayBlend(static_cast<int>(CharacterBase::ANIM_TYPE::JUMP_ATK), animVar_);
	jumpChargeCnt_ = 0.0f;

	//溜めジャンプSE再生
	soundMng_.Play(ResourceManager::SRC::ENEMY_CHARGE_SE, SoundManager::PLAYTYPE::LOOP);

	//溜めジャンプエフェクト再生
	effect_->Play(EffectController::EFF_TYPE::E_JUMP_CHARGE,
		character_.GetTransform().pos,
		character_.GetTransform().quaRot,
		{ JUMP_CHARGE_EFF_SCL,
		JUMP_CHARGE_EFF_SCL ,
		JUMP_CHARGE_EFF_SCL });
}

void EnemyCardAttackJump::AttackUpdate(void)
{
	//ジャンプチャージ
	if (jumpChargeCnt_ < JUMP_CHARGE_TIME)
	{
		//攻撃カウント
		jumpChargeCnt_ += SceneManager::GetInstance().GetDeltaTime();

		//アニメーションループ
		anim_.SetMidLoop(static_cast<int>(CharacterBase::ANIM_TYPE::JUMP_ATK), JUMP_CHARGE_ANIM_LOOP_START, JUMP_CHARGE_ANIM_LOOP_END, JUMP_ATK_ANIM_LOOP_SPEED);

		//溜めのカメラシェイク
		scnMng_.GetCamera().lock()->SetShakeStatus(jumpChargeCnt_ / JUMP_CHARGE_TIME, JUMP_CHARGE_CAMERA_SHAKE_LIMIT);
		scnMng_.GetCamera().lock()->ChangeSub(Camera::SUB_MODE::SHAKE);

		//ジャンプチャージが終わったら
		if (jumpChargeCnt_ >= JUMP_CHARGE_TIME)
		{
			//アニメーションループ終了
			anim_.SetEndMidLoop(static_cast<int>(CharacterBase::ANIM_TYPE::JUMP_ATK), CharacterBase::DEFAULT_ANIM_SPEED);
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
	else if (anim_.GetAnimStep(static_cast<int>(CharacterBase::ANIM_TYPE::JUMP_ATK)) > JUMP_ANIM_END)
	{
		const Transform& charaTrans = character_.GetTransform();

		//攻撃カウント
		atkCnt_ += SceneManager::GetInstance().GetDeltaTime();

		//攻撃中
		atk_.pos = Utility3D::AddPosRotate(charaTrans.pos, charaTrans.quaRot, { 0.0f,0.0f,0.0f });

		//攻撃判定有効
		isAliveAtkCol_ = true;
		character_.MakeAttackCol(character_.GetCharaTag(), Collider::TAG::NML_ATK, atk_.pos, atk_.atkRadius);

		//攻撃範囲拡大
		atk_.atkRadius = easing_->EaseFunc(JUMP_ATK_START_RADIUS, JUMP_ATK_GOAL_RADIUS, atkCnt_ / JUMP_ATK_CNT_MAX, Easing::EASING_TYPE::QUAD_IN);
		character_.UpdateAttackCol(atk_.atkRadius);

		//溜めのカメラシェイク()
		scnMng_.GetCamera().lock()->SetShakeStatus(atkCnt_ / JUMP_ATK_CNT_MAX, JUMP_ATTACK_CAMERA_SHAKE_LIMIT);
		scnMng_.GetCamera().lock()->ChangeSub(Camera::SUB_MODE::SHAKE);

		//サウンド再生
		if (!soundMng_.IsPlay(ResourceManager::SRC::ENEMY_JUMP_LAND_SE))
		{
			soundMng_.Play(ResourceManager::SRC::ENEMY_JUMP_LAND_SE, SoundManager::PLAYTYPE::BACK);
			effect_->Play(EffectController::EFF_TYPE::BLAST, atk_.pos, {}, { atk_.atkRadius,atk_.atkRadius,atk_.atkRadius }, true);
		}

		//エフェクトの大きさを大きくしていく
		effect_->SetScale(EffectController::EFF_TYPE::BLAST, 0,
			{ atk_.atkRadius * BLAST_EFF_SCL,atk_.atkRadius * BLAST_EFF_SCL,atk_.atkRadius * BLAST_EFF_SCL });

		//攻撃アニメーションループ
		anim_.SetMidLoop(static_cast<int>(CharacterBase::ANIM_TYPE::JUMP_ATK), JUMP_ATK_ANIM_LOOP_START, JUMP_ATK_ANIM_LOOP_END, JUMP_ATK_ANIM_ATTACK_LOOP_SPEED);

		//攻撃時間が終わったら
		if (atkCnt_ > JUMP_ATK_CNT_MAX)
		{
			atkCnt_ = 0.0f;
			atk_.atkRadius = JUMP_ATK_START_RADIUS;

			//アニメーションループ終了
			anim_.SetEndMidLoop(static_cast<int>(CharacterBase::ANIM_TYPE::JUMP_ATK), CharacterBase::DEFAULT_ANIM_SPEED);

			//当たり判定の消去
			character_.DeleteAttackCol(Collider::TAG::ENEMY1, Collider::TAG::NML_ATK);

			//エフェクトの消去
			effect_->Delete(EffectController::EFF_TYPE::BLAST, 0);

			//アイドル状態へ移行
			actionCntl_.ChangeAction(ActionController::ACTION_TYPE::IDLE);
		}
	}
}

void EnemyCardAttackJump::AttackRelease(void)
{
	//カメラシェイクを止める
	scnMng_.GetCamera().lock()->ChangeSub(Camera::SUB_MODE::NONE);

	//音を止める
	soundMng_.Stop(ResourceManager::SRC::ENEMY_CHARGE_SE);

	//エフェクトを消去
	const int JUMP_CHARGE_EFF_ARRAY = 0;
	effect_->Stop(EffectController::EFF_TYPE::E_JUMP_CHARGE, JUMP_CHARGE_EFF_ARRAY);
	effect_->Delete(EffectController::EFF_TYPE::E_JUMP_CHARGE, JUMP_CHARGE_EFF_ARRAY);
}

void EnemyCardAttackJump::LoadAnimVar(const ACTION_LOAD_DATA& _data)
{
	if (_data.name != "JumpAttack")return;

	animVar_ = _data.animVariable;
	LoadAttackStatus(_data.jsonData, atk_);
}
