#include "Utility/Utility3D.h"
#include "Utility/Utility2D.h"
#include "Utility/UtilityCommon.h"
#include "Manager/Resource/ResourceManager.h"
#include "Manager/Generic/SceneManager.h"
#include "Manager/Generic/Camera.h"
#include "Manager/Generic/DataBank.h"
#include "Object/Card/CardDeck.h"
#include "Object/Card/PlayerCardUI.h"
#include "Object/Character/Action/ActionController.h"
#include "Object/Character/Base/CharacterOnHitBase.h"
#include "./PlayerOnHit.h"
#include "./Weapon.h"
#include "./PlayerMagicFIre.h"
#include "./PlayerLogic.h"
#include "./Player.h"

Player::Player(void)
	:playerNum_(),
	cntl_(InputManager::CONTROLL_TYPE::ALL),
	padNum_()
{
	tag_ = Collider::TAG::PLAYER1;
	objectName_ = PLAYER_STR;
	padNum_ = static_cast<InputManager::JOYPAD_NO>(playerNum_ + 1);		//初めのJOYPADがkey_padなのでパッドの番号に合わせる
	characterType_ = CHARACTER_TYPE::PLAYER;
	deck_ = std::make_shared<CardDeck>(characterType_, PLAYER_NUM);
	cardPresent_ = std::make_unique<CardPresenter>(characterType_, *deck_);
	weapon_ = std::make_unique<Weapon>(*this);
	logic_ = std::make_unique<PlayerLogic>(trans_, isMoveable_, padNum_, InputManager::CONTROLL_TYPE::ALL);
	actionCtrl_ = std::make_unique<ActionController>(*this, *logic_, trans_, *cardPresent_, *animCtrl_, padNum_);
}

Player::~Player(void)
{
	collider_.clear();
}

void Player::UpdateDirectionCharacter(void)
{
	//武器の更新
	weapon_->Update();
}

void Player::LoadCharacter(void)
{
	weapon_->Load();
	resMng_.Load(SoundManager::SRC::ENEMY_HIT_SE);

	//手のボーン番号を外部ファイルから取得
	const auto& data = resMng_.Load(ResourceManager::SRC::CHARA_DATA).jsonData;
	if (data.contains("ModelData"))
	{
		const auto& handBornNoData = data["ModelData"]["handFrameNum"];
		handFrameNo_ = handBornNoData.value("handFrameNum", 0);
	}
}

void Player::InitCharacter(void)
{
	//武器の追従対象をセット
	weapon_->SetTargetAndFrameNo(&trans_, handFrameNo_);
	weapon_->Init();
}

void Player::UpdateNormalCharacter(void)
{
	weapon_->Update();
}
void Player::UpdateClearDirectionCharacter(void)
{
	//敵倒した後は操作を受け付ける
	AcceptLogicControl();
}

void Player::ChangeUpdateOverDirection(void)
{
	animCtrl_->PlayBlend(static_cast<int>(ANIM_TYPE::DEATH), deathAnim_);
	CharacterBase::ChangeUpdateOverDirection();
}

void Player::UpdateOverDirectionCharacter(void)
{
	if (animCtrl_->IsEnd(static_cast<int>(ANIM_TYPE::DEATH)))
	{
		isEndClearDirect_ = true;
	}
}

void Player::DrawCharacter(void)
{
	//武器
	weapon_->Draw();

	//炎があれば描画する
	if (!drawableItem_.expired())
	{
		drawableItem_.lock()->Draw();
	}
}

void Player::LoadCharacterActionDataCallBack(const ACTION_LOAD_DATA& _animVar)
{
	if (_animVar.name == "Death")
	{
		deathAnim_ = _animVar.animVariable;
	}
	else if (_animVar.name == "Idle")
	{
		idleAnim_ = _animVar.animVariable;
	}
}

void Player::LoadModelDataCharacter(const nlohmann::json& _data)
{
	handFrameNo_ = _data.value("handFrameNum", 0);
}

void Player::Draw2D(void)
{

#ifdef _DEBUG
	//actionCtrl_->DrawDebug();
	DrawDebug();
#endif // _DEBUG
}
void Player::OnHit(const std::weak_ptr<Collider> _hitCol)
{
	onHit_->OnHitUpdate(_hitCol);
}

void Player::MoveDirFromInput(void)
{
	//プレイヤー入力クラスから角度を取得
	charaRot_.dir_ = logic_->GetDir();
	charaRot_.dir_ = VNorm(charaRot_.dir_);
}

void Player::SetGoalRotate(void)
{
	//ベクトルからクォータニオンへ
	Quaternion axis = Quaternion::LookRotation(logic_->GetDir());

	//現在設定されている回転との角度差を取る
	double angleDiff = Quaternion::Angle(axis, charaRot_.goalQuaRot_);

	constexpr double ANGLE_THRESHOLD = 0.1;
	// しきい値
	if (angleDiff > ANGLE_THRESHOLD)
	{
		charaRot_.stepRotTime_ = TIME_ROT;
	}

	charaRot_.goalQuaRot_ = axis;
}

void Player::MakeAttackCol(const Collider::TAG _charaTag, const Collider::TAG _attackTag, const VECTOR& _atkPos, const float& _radius)
{
	//すでに当たり判定がある場合は作らない
	if (weapon_->IsAliveCollider(_charaTag, _attackTag))return;
	weapon_->MakeWeaponCollider();
}

void Player::DeleteAttackCol(const Collider::TAG& _charaTag, const Collider::TAG& _attackCol)
{
	if (!weapon_->IsAliveCollider(_charaTag, _attackCol))return;
	weapon_->DeleteWeaponCollider();
}

void Player::Damage(const int _dam)
{
	CharacterBase::Damage(_dam);
}

#ifdef _DEBUG
void Player::DrawDebug(void)
{
	animCtrl_->DrawDebug();

	cardPresent_->DrawCardDeckError();
}

#endif // _DEBUG

void Player::AcceptLogicControl(void)
{
	CharacterBase::AcceptLogicControl();

	//武器の更新
	weapon_->Update();
}

void Player::MakeColliderGeometry(void)
{
	MakeColliderFromJsonData();

	onHit_ = std::make_unique<PlayerOnHit>(*this, movedPos_, moveDiff_, *actionCtrl_, collider_, trans_);
	onHit_->Init();
	onHit_->Load();
}