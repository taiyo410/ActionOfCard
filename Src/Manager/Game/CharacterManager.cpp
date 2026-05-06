#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/UIManager.h"
#include"../Manager/Generic/Camera.h"
#include "../Object/Character/Base/CharacterBase.h"
#include "../Object/Character/Player/Player.h"
#include "../Object/Character/Enemy/Enemy.h"
#include "CharacterManager.h"

void CharacterManager::Load(void)
{
	for (auto& chara : characters_)
	{
		chara->Load();
	}

	//カメラターゲットの設定
	SceneManager::GetInstance().GetCamera().lock()->SetTarget(&characters_[ENEMY_NO]->GetTransform());
}

void CharacterManager::Init(void)
{
	//プレイヤーのローカル中心座標をカプセルの中央に合わせ、カメラを演出状態に変更
	const float playerCenterLocalPosY = (Player::CAP_LOCAL_TOP.y - Player::CAP_LOCAL_DOWN.y) * 0.5f;
	SceneManager::GetInstance().GetCamera().lock()->SetFollow(&characters_[PLAYER_NO]->GetTransform(), {0.0f,playerCenterLocalPosY ,0.0f});
	SceneManager::GetInstance().GetCamera().lock()->ChangeMode(Camera::MODE::START_DIRECTION);

	//プレイヤーと敵のターゲットをお互いに設定
	characters_[PLAYER_NO]->SetLogicTargetCharacter(characters_[ENEMY_NO]);
	characters_[ENEMY_NO]->SetLogicTargetCharacter(characters_[PLAYER_NO]);

	//初期化
	for (const auto& chara : characters_)
	{
		chara->Init();
	}
}

void CharacterManager::Update(void)
{
	for (const auto& chara : characters_)
	{
		chara->Update();
	}
}

void CharacterManager::Draw(void)
{
	for (const auto& chara : characters_)
	{
		chara->Draw();
	}
}

void CharacterManager::Draw2D(void)
{
	for (const auto& chara : characters_)
	{
		chara->Draw2D();
	}
}

const bool CharacterManager::IsSceneChageClearCondition(void) const
{
	return characters_[ENEMY_NO]->GetStatus().hp <= 0;
}

const bool CharacterManager::IsSceneChangeGameOverCondition(void) const
{
	return characters_[PLAYER_NO]->GetStatus().hp <= 0;
}

const bool CharacterManager::GetIsEnemyRoar(void) const
{
	return dynamic_pointer_cast<Enemy>(characters_[ENEMY_NO])->GetIsRoar();
}

const bool CharacterManager::GetIsEndClearDirection(void) const
{
	return dynamic_pointer_cast<Enemy>(characters_[ENEMY_NO])->GetIsEndDirect();
}

const bool CharacterManager::GetIsEndOverDirection(void) const
{
	return characters_[PLAYER_NO]->GetIsEndDirect();
}

void CharacterManager::ChangeCharacterNormalUpdate(void)
{
	for (auto& chara : characters_)
	{
		chara->ChangeDirectToNormal();
	}
}

void CharacterManager::ChangeCharacterDirectionUpdate(void)
{
	for (auto& chara : characters_)
	{
		chara->ChangeUpdatePhase(CharacterBase::UPDATE_PHASE::DIRECTION);
	}
}

void CharacterManager::ChangeCharacterClearDirection(void)
{
	for (const auto& chara : characters_)
	{
		chara->ChangeUpdatePhase(CharacterBase::UPDATE_PHASE::CLEAR_DIRECTION);
	}
}

void CharacterManager::ChangeCharacterOverDirection(void)
{
	for (const auto& chara : characters_)
	{
		chara->ChangeUpdatePhase(CharacterBase::UPDATE_PHASE::OVER_DIRECTION);
	}
}

CharacterManager::CharacterManager(void)
{
	characters_[PLAYER_NO] = std::make_shared<Player>();
	characters_[ENEMY_NO] = std::make_shared<Enemy>();
}

CharacterManager::~CharacterManager(void)
{
}
