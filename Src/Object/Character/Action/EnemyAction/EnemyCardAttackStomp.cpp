#include "../pch.h"
#include "Utility/Utility3D.h"
#include "Manager/Generic/Camera.h"
#include "Manager/Generic/SceneManager.h"
#include "Manager/Resource/ResourceManager.h"
#include "Manager/Resource/SoundManager.h"
#include "Object/Common/EffectController.h"
#include "Object/Common/AnimationController.h"
#include "Object/CHaracter/Action/ActionController.h"
#include "Object/CHaracter/Base/CharacterBase.h"
#include "Object/Character/Enemy/EnemyRock.h"
#include "./EnemyCardAttackStomp.h"

EnemyCardAttackStomp::EnemyCardAttackStomp(ActionController& _actCntl, CharacterBase& _charaObj, CardPresenter& _cardPresent):
	EnemyCardAttackBase(_actCntl, _charaObj, _cardPresent)
{

}

EnemyCardAttackStomp::~EnemyCardAttackStomp(void)
{
}

void EnemyCardAttackStomp::Load(void)
{
	resMng_.Load(ResourceManager::SRC::ENEMY_STOMP_SE);

	//敵の岩生成
	for(const auto& rock : rock_)
	{
		rock->Load();
	}
}

void EnemyCardAttackStomp::InitAttack(void)
{
	//岩生成フラグの初期化
	isGenerateRock_ = false;

	//スタンプアニメーション再生
	anim_.PlayBlend(static_cast<int>(CharacterBase::ANIM_TYPE::STOMP_ATK), animVar_);
}

void EnemyCardAttackStomp::AttackUpdate(void)
{
	//アニメーションで足をつく動作をしたら
	if (anim_.GetAnimStep(static_cast<int>(CharacterBase::ANIM_TYPE::STOMP_ATK)) > atk_.colStartStep)
	{
		//キャラ情報
		const Transform& charaTrans = character_.GetTransform();

		//攻撃中
		atk_.pos = Utility3D::AddPosRotate(charaTrans.pos, charaTrans.quaRot, { 0.0f,0.0f,0.0f });

		//溜めのカメラシェイク
		atkCnt_ += SceneManager::GetInstance().GetDeltaTime();
		scnMng_.GetCamera().lock()->SetShakeStatus(atkCnt_ / cameraShakeTime_, cameraShakeLimit_);
		scnMng_.GetCamera().lock()->ChangeSub(Camera::SUB_MODE::SHAKE);

		//地響き音再生
		const bool isPlayStompSE = soundMng_.IsPlay(ResourceManager::SRC::ENEMY_STOMP_SE);
		if (!isPlayStompSE)
		{
			soundMng_.Play(ResourceManager::SRC::ENEMY_STOMP_SE, SoundManager::PLAYTYPE::BACK);
		}

		//地面から岩の玉を生成してあらゆる方向に飛ばす
		if (!isGenerateRock_)
		{
			//character_.SetIsAliveEnemyRock(true);
			for (auto& rock : rock_)
			{
				rock->SetIsAlive(true);
				rock->Init();

				//キャラクター側に岩の描画登録
				character_.RegisterDrawableRocks(rock);
			}
			isGenerateRock_ = true;
		}
		else
		{
			for (auto& rock : rock_)
			{
				rock->Update();
			}
		}

		//ため時間が終わったら
		if (atkCnt_ > cameraShakeTime_)
		{
			atkCnt_ = 0.0f;
			atk_.atkRadius = 0.0f;

			//アニメーションループ終了
			anim_.SetEndMidLoop(static_cast<int>(CharacterBase::ANIM_TYPE::STOMP_ATK), CharacterBase::DEFAULT_ANIM_SPEED);

			//敵の岩の攻撃判定終了
			character_.DeleteAttackCol(Collider::TAG::ENEMY1, Collider::TAG::NML_ATK);

			//エフェクトの終了
			effect_->Stop(EffectController::EFF_TYPE::JUMP, 0);
			effect_->Delete(EffectController::EFF_TYPE::JUMP, 0);

			//カード終了処理
			cardPresent_.FinishCard();

			//アイドル状態へ
			actionCtrl_.ChangeAction(ActionController::ACTION_TYPE::IDLE);
		}
	}
}

void EnemyCardAttackStomp::AttackRelease(void)
{
	scnMng_.GetCamera().lock()->ChangeSub(Camera::SUB_MODE::NONE);
	//character_.DeleteEnemyRockCol();
	//敵の岩の攻撃判定終了
	for (auto& rock : rock_)
	{
		rock->DeleteRockCollider();
	}

	//キャラクター側に岩の描画登録
	character_.UnRegisterDrawableRocks();
}

void EnemyCardAttackStomp::LoadAnimVar(const ACTION_LOAD_DATA& _data)
{
	if (_data.name != "StompAttack")return;

	animVar_ = _data.animVariable;

	const auto& data = _data.jsonData;
	LoadAttackStatus(data, atk_);
	cameraShakeTime_ = data.value("cameraShakeTime", 0.0f);
	rockNum_ = data.value("rockNum", 0);
	cameraShakeLimit_ = data.value("cameraShakeLimit", 0.0f);

	//岩の生成
	for (int i = 0; i < rockNum_; i++)
	{
		std::shared_ptr<EnemyRock> rock = std::make_shared<EnemyRock>(i, atk_.pos);
		rock_.emplace_back(rock);
	}
}
