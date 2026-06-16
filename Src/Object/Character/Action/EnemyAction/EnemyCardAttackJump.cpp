#include "../pch.h"
#include "Utility/Utility3D.h"
#include "Manager/Generic/Camera.h"
#include "Manager/Resource/ResourceManager.h"
#include "Manager/Resource/SoundManager.h"
#include "Manager/Generic/SceneManager.h"
#include "Object/Common/AnimationController.h"
#include "Object/Common/EffectController.h"
#include "Object/Character/Action/ActionController.h"
#include "Object/Character/Base/CharacterBase.h"
#include "./EnemyCardAttackJump.h"

EnemyCardAttackJump::EnemyCardAttackJump(ActionController& _actCntl, CharacterBase& _charaObj, CardPresenter& _cardPresent):
	EnemyCardAttackBase(_actCntl, _charaObj, _cardPresent),
	jumpChargeEffNum_(-1),
	blastEffNum_(-1)
{
	easing_ = std::make_unique<Easing>();
}

EnemyCardAttackJump::~EnemyCardAttackJump(void)
{
	effect_->AllStop();
	//リソースの解放
	effect_->AllDelete();
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
	jumpChargeEffNum_ = effect_->Play(EffectController::EFF_TYPE::E_JUMP_CHARGE,
		character_.GetTransform().pos,
		character_.GetTransform().quaRot,
		{ chargeEffScale_,
		chargeEffScale_ ,
		chargeEffScale_ });
}

void EnemyCardAttackJump::AttackUpdate(void)
{
	//ジャンプチャージ
	if (jumpChargeCnt_ < chargeTime_)
	{
		//攻撃カウント
		jumpChargeCnt_ += SceneManager::GetInstance().GetDeltaTime();

		//アニメーションループ
		anim_.SetMidLoop(static_cast<int>(CharacterBase::ANIM_TYPE::JUMP_ATK), chargeAnimLoopStartStep_, chargeAnimLoopEndStep_, chargeAnimLoopSpd_);

		//溜めのカメラシェイク
		scnMng_.GetCamera().lock()->SetShakeStatus(jumpChargeCnt_ / chargeTime_, chargeCameraShakeLimit_);
		scnMng_.GetCamera().lock()->ChangeSub(Camera::SUB_MODE::SHAKE);

		//ジャンプチャージが終わったら
		if (jumpChargeCnt_ >= chargeTime_)
		{
			//アニメーションループ終了
			anim_.SetEndMidLoop(static_cast<int>(CharacterBase::ANIM_TYPE::JUMP_ATK), CharacterBase::DEFAULT_ANIM_SPEED);
			soundMng_.Stop(ResourceManager::SRC::ENEMY_CHARGE_SE);

			//チャージエフェクトの消去
			effect_->Stop(EffectController::EFF_TYPE::E_JUMP_CHARGE, jumpChargeEffNum_);
			effect_->Delete(EffectController::EFF_TYPE::E_JUMP_CHARGE, jumpChargeEffNum_);

			//カードを終了状態にし、プレイヤーの攻撃で中断できないようにする
			cardPresent_.FinishCard();
		}
	}

	//ジャンプアニメーションが終わったらドーム型の攻撃をする
	else if (anim_.GetAnimStep(static_cast<int>(CharacterBase::ANIM_TYPE::JUMP_ATK)) > jumpAnimEndStep_)
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
		atk_.atkRadius = easing_->EaseFunc(atkStartRadius_, atkEndRadius_, atkCnt_ / atkExpandTime_, Easing::EASING_TYPE::QUAD_IN);
		character_.UpdateAttackCol(atk_.atkRadius);

		//溜めのカメラシェイク()
		scnMng_.GetCamera().lock()->SetShakeStatus(atkCnt_ / atkExpandTime_, atkCameraShakeLimit_);
		scnMng_.GetCamera().lock()->ChangeSub(Camera::SUB_MODE::SHAKE);

		//サウンド再生
		if (!soundMng_.IsPlay(ResourceManager::SRC::ENEMY_JUMP_LAND_SE))
		{
			soundMng_.Play(ResourceManager::SRC::ENEMY_JUMP_LAND_SE, SoundManager::PLAYTYPE::BACK);
			blastEffNum_ = effect_->Play(EffectController::EFF_TYPE::BLAST, atk_.pos, {}, { atk_.atkRadius,atk_.atkRadius,atk_.atkRadius }, true);
		}

		//エフェクトの大きさを大きくしていく
		effect_->SetScale(EffectController::EFF_TYPE::BLAST, blastEffNum_,
			{ atk_.atkRadius * atkEffScale_,atk_.atkRadius * atkEffScale_,atk_.atkRadius * atkEffScale_ });

		//攻撃アニメーションループ
		anim_.SetMidLoop(static_cast<int>(CharacterBase::ANIM_TYPE::JUMP_ATK), atkAnimLoopStartStep_, atkAnimLoopEndStep_, atkAnimLoopSpd_);

		//攻撃時間が終わったら
		if (atkCnt_ > atkExpandTime_)
		{
			atkCnt_ = 0.0f;
			atk_.atkRadius = atkStartRadius_;

			//アニメーションループ終了
			anim_.SetEndMidLoop(static_cast<int>(CharacterBase::ANIM_TYPE::JUMP_ATK), CharacterBase::DEFAULT_ANIM_SPEED);

			//当たり判定の消去
			character_.DeleteAttackCol(Collider::TAG::ENEMY1, Collider::TAG::NML_ATK);

			//アイドル状態へ移行
			actionCtrl_.ChangeAction(ActionController::ACTION_TYPE::IDLE);
		}
	}
}

void EnemyCardAttackJump::AttackRelease(void)
{
	//カメラシェイクを止める
	scnMng_.GetCamera().lock()->ChangeSub(Camera::SUB_MODE::NONE);

	//音を止める
	soundMng_.Stop(ResourceManager::SRC::ENEMY_CHARGE_SE);

	//チャージエフェクトの消去
	effect_->Stop(EffectController::EFF_TYPE::E_JUMP_CHARGE, jumpChargeEffNum_);
	effect_->Delete(EffectController::EFF_TYPE::E_JUMP_CHARGE, jumpChargeEffNum_);

	effect_->Stop(EffectController::EFF_TYPE::BLAST, blastEffNum_);
	effect_->Delete(EffectController::EFF_TYPE::BLAST, blastEffNum_);
}

void EnemyCardAttackJump::LoadAnimVar(const ACTION_LOAD_DATA& _data)
{
	if (_data.name != "JumpAttack")return;
	animVar_ = _data.animVariable;

	const auto& data = _data.jsonData;
	LoadAttackStatus(data, atk_);
	chargeEffScale_ = data.value("chargeEffectScale", 0.0f);
	chargeTime_ = data.value("chargeTime", 0.0f);
	chargeAnimLoopStartStep_ = data.value("chargeAnimLoopStartStep", 0.0f);
	chargeAnimLoopEndStep_ = data.value("chargeAnimLoopEndStep", 0.0f);
	chargeAnimLoopSpd_ = data.value("chargeAnimLoopSpeed", 0.0f);
	chargeCameraShakeLimit_ = data.value("chargeCameraShakeLimit", 0.0f);
	jumpAnimEndStep_ = data.value("jumpEndAnimStep", 0.0f);

	//ローカル座標の取得
 	if (data.contains("attackLocalPos"))
	{
		atkLocalPos_.x = data["attackLocalPos"].value("x", 0.0f);
		atkLocalPos_.y = data["attackLocalPos"].value("y", 0.0f);
		atkLocalPos_.z = data["attackLocalPos"].value("z", 0.0f);
	}
	atkStartRadius_ = data.value("attackStartRadius", 0.0f);
	atkEndRadius_ = data.value("attackEndRadius", 0.0f);
	atkExpandSpd_ = data.value("attackColExpandSpeed", 0.0f);
	atkExpandTime_ = data.value("attackColExpandTime", 0.0f);
	atkAnimLoopStartStep_ = data.value("attackAnimLoopStartStep", 0.0f);
	atkAnimLoopEndStep_ = data.value("attackAnimLoopEndStep", 0.0f);
	atkAnimLoopSpd_ = data.value("attackAnimLoopSpeed", 0.0f);
	atkCameraShakeLimit_ = data.value("attackCameraShakeLimit", 0.0f);
	atkEffScale_ = data.value("attackEffectScale", 0.0f);


}
