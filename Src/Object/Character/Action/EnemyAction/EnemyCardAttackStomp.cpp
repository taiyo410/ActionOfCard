#include "../pch.h"
#include "../../../Common/AnimationController.h"
#include "../Utility/Utility3D.h"
#include "../Manager/Generic/Camera.h"
#include "../Manager/Resource/ResourceManager.h"
#include "../Manager/Resource/SoundManager.h"
#include "../Manager/Generic/SceneManager.h"
#include "../ActionController.h"
#include "../../Base/CharacterBase.h"
#include "../Object/Common/EffectController.h"
#include "../Object/Character/Enemy/EnemyRock.h"
#include "EnemyCardAttackStomp.h"

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
	//character_.LoadEnemyRock();
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

		//カメラシェイク制限値
		constexpr float CAMERA_SHAKE_LIMIT = 30.0f;

		//溜めのカメラシェイク
		atkCnt_ += SceneManager::GetInstance().GetDeltaTime();
		scnMng_.GetCamera().lock()->SetShakeStatus(atkCnt_ / STOMP_ATK_SHAKE_CNT, CAMERA_SHAKE_LIMIT);
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
			//character_.EnemyRockUpdate();
			for (auto& rock : rock_)
			{
				rock->Update();
			}
		}

		//ため時間が終わったら
		if (atkCnt_ > STOMP_ATK_SHAKE_CNT)
		{
			atkCnt_ = 0.0f;
			atk_.atkRadius = 0.0f;

			//アニメーションループ終了
			anim_.SetEndMidLoop(static_cast<int>(CharacterBase::ANIM_TYPE::STOMP_ATK), CharacterBase::DEFAULT_ANIM_SPEED);
			//character_.DeleteEnemyRockCol();
			//character_.SetIsAliveEnemyRock(false);

			//敵の岩の攻撃判定終了
			//character_.DeleteEnemyRockCol();
			character_.DeleteAttackCol(Collider::TAG::ENEMY1, Collider::TAG::NML_ATK);

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

void EnemyCardAttackStomp::AttackRelease(void)
{
	scnMng_.GetCamera().lock()->ChangeSub(Camera::SUB_MODE::NONE);
	//character_.DeleteEnemyRockCol();
	//敵の岩の攻撃判定終了
	for (auto& rock : rock_)
	{
		rock->DeleteRockCollider();
	}
	//character_.SetIsAliveEnemyRock(false);
}

void EnemyCardAttackStomp::LoadAnimVar(const ACTION_LOAD_DATA& _data)
{
	if (_data.name != "StompAttack")return;

	animVar_ = _data.animVariable;
	LoadAttackStatus(_data.jsonData, atk_);

	//岩の生成
	//character_.AddEnemyRock(STOMP_ATK_ROCK_NUM, atk_.pos);
	for (int i = 0; i < STOMP_ATK_ROCK_NUM; i++)
	{
		std::shared_ptr<EnemyRock> rock = std::make_shared<EnemyRock>(i, atk_.pos);
		rock_.emplace_back(rock);
	}
}
