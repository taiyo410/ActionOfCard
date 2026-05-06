#include"../Utility/Utility2D.h"
#include"../Utility/Utility3D.h"
#include"../Utility/UtilityCommon.h"
#include "../../../Application.h"
#include "../Player/Player.h"
#include "../Base/CharacterOnHitBase.h"
#include "./EnemyOnHit.h"
#include "./EnemyRock.h"
#include "../Object/Common/AnimationController.h"
#include "../Enemy/EnemyLogic.h"
#include "../../Common/Geometry/Capsule.h"
#include "../../Common/Geometry/Sphere.h"
#include "../../Common/Geometry/Line.h"
#include "../Manager/Resource/ResourceManager.h"
#include "../Manager/Generic/Camera.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/InputManager.h"
#include "../Manager/Generic/DataBank.h" 
#include "../../Card/CardDeck.h"
#include "../Object/Card/EnemyCardUI.h"
#include "../Object/Card/CardPresenter.h"
#include "../Object/Common/EffectController.h"
#include "../Player/ActionController.h"
#include "../Base/ActionBase.h"
#include "../Action/Idle.h"
#include "../Action/Run.h"
#include "../Action/React.h"
#include "../Action/EnemyCardAction.h"
#include "Enemy.h"

Enemy::Enemy(void):
	cardCenterPos_({}),
	modelScl_(MODEL_SIZE_MULTIPLITER)
{
	capRadius_ = CAP_RADIUS;

	characterType_ = CHARACTER_TYPE::ENEMY;

	isRoar_ = false;

	footSEDisCount_ = FOOT_SE_DIS;
	footSE_ = ResourceManager::SRC::ENEMY_FOOT_SE;

	logic_ = std::make_unique<EnemyLogic>(trans_);

	deck_ = std::make_shared<CardDeck>(characterType_, ENEMY_NUM);

	cardPresent_ = std::make_unique<CardPresenter>(characterType_, *deck_);

	effect_ = std::make_unique<EffectController>();
}

Enemy::~Enemy(void)
{
	collider_.clear();
	effect_->AllStop();
	rock_.clear();
}
void Enemy::Load(void)
{
	trans_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::ENEMY));
	trans_.quaRot = Quaternion();
	trans_.quaRotLocal =
		Quaternion::Euler({ 0.0f,UtilityCommon::Deg2RadF(MODEL_LOCAL_DEG), 0.0f });

	//アニメーション
	AddAnimation();

	//エフェクト
	effect_->Add(resMng_.Load(ResourceManager::SRC::E_DEATH_EFF).handleId_, EffectController::EFF_TYPE::E_DEATH);

	//ステータスロード
	LoadStatus();

	//アクションの追加
	AddAction();

	action_->Load();
}

void Enemy::Init(void)
{
	deck_->Init();

	action_->Init();

	logic_->Init();

	tag_ = Collider::TAG::ENEMY1;
	capRadius_ = CAP_RADIUS;

	//Transformの設定
	trans_.quaRot = Quaternion();
	trans_.scl = { modelScl_,modelScl_,modelScl_ };
	trans_.quaRotLocal =
		Quaternion::Euler({ 0.0f, UtilityCommon::Deg2RadF(MODEL_LOCAL_DEG), 0.0f });

	trans_.pos = { 0.0f,0.0f,CENTER_POS_Z_OFFSET };
	trans_.localPos = { 0.0f,0.0f,0.0f };
	trans_.Update();

	//当たり判定の作成
	MakeColliderGeometry();
}

void Enemy::UpdateDirection(void)
{
	animationController_->Update();
	action_->Update();
	UpdateRoarDirection();

	//Transformの更新
	trans_.quaRot = charaRot_.playerRotY_;
	trans_.Update();
}

void Enemy::UpdateClearDirection(void)
{
	animationController_->Update();
	VECTOR effPos = MV1GetFramePosition(trans_.modelId, CHEST_FRAME_NO);
	effect_->SetPos(EffectController::EFF_TYPE::E_DEATH, 0, effPos);
	effect_->Update();
	if (animationController_->GetAnimStep() >= DEATH_BLAST_ANIM_STEP)
	{

		if (modelScl_ <= 0.0f)
		{
			isEndClearDirect_ = true;
			return;
		}
		modelScl_ -= 0.1f;
		trans_.scl = { modelScl_,modelScl_,modelScl_ };
	}
	trans_.Update();
}

void Enemy::UpdateOverDirection(void)
{
	constexpr float ANIM_SPD_SCL = 0.2f;
	animationController_->Update(ANIM_SPD_SCL);
}

void Enemy::Draw(void)
{
	//通常描画
	MV1DrawModel(trans_.modelId);

	//岩の描画
	if (!rock_.empty())
	{
		for (auto& rock : rock_)
		{
			rock->Draw();
		}
	}
}
void Enemy::Draw2D(void)
{

#ifdef _DEBUG
	DrawDebug();
#endif // _DEBUG
}
void Enemy::OnHit(const std::weak_ptr<Collider> _hitCol)
{
	onHit_->OnHitUpdate(_hitCol);
}
void Enemy::MoveDirFromInput(void)
{
	//入力クラスから角度を取得
	VECTOR getDir = logic_->GetDir();
	charaRot_.dir_ = getDir;
}
void Enemy::SetGoalRotate(void)
{
	Quaternion axis= Quaternion::Euler
	(Utility3D::GetRotAxisToTarget(trans_.pos, logic_->GetTargetTransform().pos, Utility3D::AXIS_Y));

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

void Enemy::UpdateRoarDirection(void)
{
	if (scnMng_.GetCamera().lock()->GetDirectionMode() == Camera::DIRECTION_MODE::ENEMY_ROAR_VIEW)
	{
		isRoar_ = false;
		animationController_->Play(static_cast<int>(ANIM_TYPE::ROAR_ATK), false);
		float roarAnimStep = animationController_->GetAnimStep();
		const float ROAR_TIME = ROAR_ANIM_END_ANIM - ROAR_ANIM_SPEED;
		if (roarAnimStep >= ROAR_ANIM_START_ANIM)
		{
			//カメラシェイク
			float t = (roarAnimStep - ROAR_ANIM_START_ANIM) / ROAR_TIME;
			scnMng_.GetCamera().lock()->SetShakeStatus(t, CAM_SHAKE_LIMIT);
			scnMng_.GetCamera().lock()->ChangeSub(Camera::SUB_MODE::SHAKE);

			//咆哮状態にする
			isRoar_ = true;
		}
	}
	else
	{
		isRoar_ = false;
		animationController_->Play(static_cast<int>(ANIM_TYPE::IDLE), false);
	}
}

void Enemy::ChangeUpdateClearDirection(void)
{
	isRoar_ = false;
	animationController_->Play(static_cast<int>(ANIM_TYPE::DEATH), false);
	VECTOR effPos = MV1GetFramePosition(trans_.modelId, CHEST_FRAME_NO);
	soundMng_.Stop(ResourceManager::SRC::ENEMY_FOOT_SE);
	soundMng_.Stop(ResourceManager::SRC::ENEMY_JUMP_LAND_SE);
	soundMng_.Stop(ResourceManager::SRC::ENEMY_CHARGE_SE);
	effect_->Play(EffectController::EFF_TYPE::E_DEATH, effPos, trans_.quaRot, DEATH_EFF_SCL_VEC);
	CharacterBase::ChangeUpdateClearDirection();
}

void Enemy::MakeColliderGeometry(void)
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

	onHit_ = std::make_unique<EnemyOnHit>(*this, movedPos_, moveDiff_, *action_, collider_, trans_);
}
void Enemy::UpdateNormal(void)
{
	animationController_->Update();

	logic_->Update();

	action_->Update();

	//回転の同期
	UpdatePost();
	trans_.quaRot = charaRot_.playerRotY_;

	trans_.Update();
}
void Enemy::AddAction(void)
{
	action_ = std::make_unique<ActionController>(*this, *logic_, trans_, *cardPresent_, *animationController_, InputManager::JOYPAD_NO::PAD1);
	using ACTION_TYPE = ActionController::ACTION_TYPE;
	//使用するアクションを追加
	action_->AddAction({ ACTION_TYPE::IDLE,ACTION_TYPE::MOVE
		,ACTION_TYPE::REACT,ACTION_TYPE::CARD_ACTION });
}
void Enemy::AddAnimation(void)
{
	animationController_ = std::make_unique<AnimationController>(trans_.modelId, SPINE_FRAME_NO);
	animationController_->Add(static_cast<int>(ANIM_TYPE::IDLE), ANIM_SPEED, resMng_.LoadModelDuplicate(ResourceManager::SRC::E_IDLE));
	animationController_->Add(static_cast<int>(ANIM_TYPE::RUN), ANIM_SPEED, resMng_.LoadModelDuplicate(ResourceManager::SRC::E_RUN));
	animationController_->Add(static_cast<int>(ANIM_TYPE::REACT), ANIM_SPEED, resMng_.LoadModelDuplicate(ResourceManager::SRC::REACT));
	animationController_->Add(static_cast<int>(ANIM_TYPE::STOMP_ATK), ANIM_SPEED, resMng_.LoadModelDuplicate(ResourceManager::SRC::E_STOMP_ATK));
	animationController_->Add(static_cast<int>(ANIM_TYPE::JUMP_ATK), ANIM_SPEED, resMng_.LoadModelDuplicate(ResourceManager::SRC::E_JUMP_ATK));
	animationController_->Add(static_cast<int>(ANIM_TYPE::ROAR_ATK), ROAR_ANIM_SPEED, resMng_.LoadModelDuplicate(ResourceManager::SRC::E_ROAR_ATK));
	animationController_->Add(static_cast<int>(ANIM_TYPE::DEATH), ANIM_SPEED, resMng_.LoadModelDuplicate(ResourceManager::SRC::E_DEATH));
}

#ifdef _DEBUG
void Enemy::DrawDebug(void)
{
	for (auto& col : collider_)
	{
		col.second->GetGeometry().Draw();
	}
}
#endif // _DEBUG
