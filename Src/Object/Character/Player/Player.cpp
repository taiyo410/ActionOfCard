#include<algorithm>
#include "../../../Utility/Utility3D.h"
#include "../../../Utility/UtilityCommon.h"
#include "../../../Utility/Utility2D.h"
#include "../../../Manager/Resource/ResourceManager.h"
#include "../../../Manager/Generic/SceneManager.h"
#include "../../../Manager/Generic/Camera.h"
#include "../../../Manager/Generic/DataBank.h"
#include "../../../Manager/Generic/UIManager.h"
#include "../Manager/Game/CharacterManager.h"
#include"../../Common/Geometry/Capsule.h"
#include"../../Common/Geometry/Sphere.h"
#include"../../Common/Geometry/Line.h"
#include"../Object/Card/CardDeck.h"
#include "../../Card/CardPresenter.h"
#include"../Object/Card/PlayerCardUI.h"
#include "../../../Object/Common/AnimationController.h"
#include"../Action/ActionController.h"
#include"../Base/CharacterOnHitBase.h"
#include"./PlayerOnHit.h"
#include "./Weapon.h"
#include"../Base/ActionBase.h"
#include"../Action/Idle.h"
#include"../Action/Run.h"
#include"../Action/React.h"
#include"../Action/Dodge.h"
#include "./PlayerLogic.h"
#include "./PlayerMagicFIre.h"
#include "Player.h"

Player::Player(void)
	:playerNum_(),
	cntl_(InputManager::CONTROLL_TYPE::ALL),
	padNum_()
{
	padNum_ = static_cast<InputManager::JOYPAD_NO>(playerNum_ + 1);		//初めのJOYPADがkey_padなのでパッドの番号に合わせる
	characterType_ = CHARACTER_TYPE::PLAYER;
	capRadius_ = CAP_RADIUS;
	deck_ = std::make_shared<CardDeck>(characterType_, PLAYER_NUM);
	cardPresent_ = std::make_unique<CardPresenter>(characterType_, *deck_);
	weapon_ = std::make_unique<Weapon>(*this);
	logic_ = std::make_unique<PlayerLogic>(trans_, isMoveable_, padNum_, InputManager::CONTROLL_TYPE::ALL);
	hipBoneNo_ = SPINE_FRAME_NO;
}

Player::~Player(void)
{
	collider_.clear();
}

void Player::Load(void)
{
	trans_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::PLAYER));
	trans_.scl = MODEL_SCL;
	trans_.quaRotLocal =
		Quaternion::Euler({ 0.0f, UtilityCommon::Deg2RadF(MODEL_LOCAL_DEG), 0.0f });
	trans_.pos = { 0.0f,0.0f,-CENTER_POS_Z_OFFSET };
	trans_.localPos = { 0.0f,Player::CAP_RADIUS,0.0f };

	//アニメーションコントローラーの生成
	animationController_ = std::make_unique<AnimationController>(trans_.modelId, hipBoneNo_);

	//ステータスのロード
	LoadStatus();

	//アクションの追加
	AddAction();

	LoadAddAnimation([this](const ACTION_LOAD_DATA& animVar)
		{
			//アクションコントローラーの全行動クラスに通知
			action_->AnimLoadNotify(animVar);
			if(animVar.name=="Death")
			{
				deathAnim_ = animVar.animVariable;
			}
			else if(animVar.name=="Idle")
			{
				idleAnim_ = animVar.animVariable;
			}
		});
	
	action_->Load();
	weapon_->Load();
	resMng_.Load(SoundManager::SRC::ENEMY_HIT_SE);
}

void Player::Init(void)
{

	//武器の追従対象をセット
	weapon_->SetTargetAndFrameNo(&trans_, HAND_FRAME_NO);

	tag_ = Collider::TAG::PLAYER1;
	
	//プレイヤー入力
	logic_->Init();

	MakeColliderGeometry();

	action_->Init();
	weapon_->Init();

	deck_->Init();

	//更新
	trans_.Update();
}

void Player::UpdateDirection(void)
{
	//アニメーションの更新
	animationController_->Update();

	//武器の更新
	weapon_->Update();

	//Transformの更新
	trans_.quaRot = charaRot_.playerRotY_;
	trans_.Update();
}

void Player::UpdateNormal(void)
{
	//プレイヤー状態更新
	Action();

	//アニメーション
	animationController_->Update();

	//武器
	weapon_->Update();
}

void Player::UpdateClearDirection(void)
{
	//演出と同じ更新
	UpdateDirection();

	//プレイヤー状態更新
	Action();
}

void Player::UpdateOverDirection(void)
{
	UpdateDirection();
	if (animationController_->IsEnd(static_cast<int>(ANIM_TYPE::DEATH)))
	{
		isEndClearDirect_ = true;
	}
}

void Player::ChangeUpdateOverDirection(void)
{
	animationController_->PlayBlend(static_cast<int>(ANIM_TYPE::DEATH), deathAnim_);
	CharacterBase::ChangeUpdateOverDirection();
}

void Player::Draw(void)
{
	//通常描画
	MV1DrawModel(trans_.modelId);

	//武器
	weapon_->Draw();

	//炎の描画
	if (!drawableFire_.expired())
	{
		drawableFire_.lock()->Draw();
	}

}

void Player::Draw2D(void)
{

#ifdef _DEBUG
	//action_->DrawDebug();
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
	//for (auto& col : collider_)
	//{
	//	col.second->GetGeometry().Draw();
	//}

	animationController_->DrawDebug();

	cardPresent_->DrawCardDeckError();
}

#endif // _DEBUG

void Player::AddAction(void)
{
	//アクション
	action_ = std::make_unique<ActionController>(*this, *logic_, trans_, *cardPresent_, *animationController_, padNum_);
	using ACTION_TYPE = ActionController::ACTION_TYPE;
	action_->AddAction({ ACTION_TYPE::IDLE, ACTION_TYPE::MOVE,ACTION_TYPE::REACT, ACTION_TYPE::DODGE
		,ACTION_TYPE::CARD_ATTACK_ONE_MIDDLE, ACTION_TYPE::CARD_ATTACK_ONE_SHORT
		,ACTION_TYPE::CARD_ATTACK_TWO,ACTION_TYPE::CARD_ATTACK_THREE,ACTION_TYPE::CARD_MAGIC_FIRE,ACTION_TYPE::CARD_RELOAD });
}

void Player::MakeColliderGeometry(void)
{
	//カプセル
	std::unique_ptr<Geometry>geo = std::make_unique<Capsule>(trans_.pos, trans_.quaRot, CAP_LOCAL_TOP, CAP_LOCAL_DOWN, CAP_RADIUS);
	MakeCollider(TAG_PRIORITY::BODY, { tag_ }, std::move(geo));
	tagPrioritys_.emplace_back(TAG_PRIORITY::BODY);

	//現在の座標と移動後座標を結んだ線のコライダ(落下時の当たり判定)
	geo = std::make_unique<Line>(trans_.pos, trans_.quaRot, Utility3D::VECTOR_ZERO, Utility3D::VECTOR_ZERO);
	MakeCollider(TAG_PRIORITY::MOVE_LINE, { tag_ }, std::move(geo));
	tagPrioritys_.emplace_back(TAG_PRIORITY::MOVE_LINE);

	//上下ライン
	geo = std::make_unique<Line>(trans_.pos, trans_.quaRot, CAP_LOCAL_TOP, CAP_LOCAL_DOWN);
	MakeCollider(TAG_PRIORITY::UPDOWN_LINE, { tag_ }, std::move(geo));
	tagPrioritys_.emplace_back(TAG_PRIORITY::UPDOWN_LINE);

	onHit_ = std::make_unique<PlayerOnHit>(*this, movedPos_, moveDiff_, *action_, collider_, trans_);
	onHit_->Init();
	onHit_->Load();
}

void Player::Action(void)
{
	//ロジック
	logic_->Update();

	//アクション関係の更新
	action_->Update();

	//当たり判定の更新
	UpdatePost();

	trans_.quaRot = charaRot_.playerRotY_;
	trans_.Update();
}

