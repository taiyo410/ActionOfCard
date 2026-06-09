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
#include "../Action/ActionController.h"
#include "../Base/ActionBase.h"
#include "../Action/Idle.h"
#include "../Action/Run.h"
#include "../Action/React.h"
#include "Enemy.h"

Enemy::Enemy(void):
	cardCenterPos_({}),
	modelScl_(MODEL_SIZE_MULTIPLITER)
{
	objectName_ = ENEMY_STR;
	characterType_ = CHARACTER_TYPE::ENEMY;
	isRoar_ = false;
	logic_ = std::make_unique<EnemyLogic>(trans_);
	deck_ = std::make_shared<CardDeck>(characterType_, ENEMY_NUM);
	cardPresent_ = std::make_unique<CardPresenter>(characterType_, *deck_);
	effect_ = std::make_unique<EffectController>();
	spineBoneNo_ = SPINE_FRAME_NO;
	tag_ = Collider::TAG::ENEMY1;

	//アニメーションコントローラーの生成
	animCtrl_ = std::make_unique<AnimationController>(trans_.modelId, spineBoneNo_);
	//アクション
	actionCtrl_ = std::make_unique<ActionController>(*this, *logic_, trans_, *cardPresent_, *animCtrl_, InputManager::JOYPAD_NO::PAD1);
}

Enemy::~Enemy(void)
{
	collider_.clear();
	effect_->AllStop();
}
//void Enemy::Load(void)
//{
//	//trans_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::ENEMY));
//	//trans_.quaRot = Quaternion();
//	//trans_.quaRotLocal =
//	//	Quaternion::Euler({ 0.0f,UtilityCommon::Deg2RadF(MODEL_LOCAL_DEG), 0.0f });
//}

//void Enemy::Init(void)
//{
//	//deck_->Init();
//	//actionCtrl_->Init();
//	//logic_->Init();
//
//	////Transformの設定
//	//trans_.quaRot = Quaternion();
//	//trans_.scl = { modelScl_,modelScl_,modelScl_ };
//	//trans_.quaRotLocal =
//	//	Quaternion::Euler({ 0.0f, UtilityCommon::Deg2RadF(MODEL_LOCAL_DEG), 0.0f });
//
//	//trans_.pos = { 0.0f,0.0f,CENTER_POS_Z_OFFSET };
//	//trans_.localPos = { 0.0f,0.0f,0.0f };
//	//trans_.Update();
//
//	//当たり判定の作成
//	//MakeColliderGeometry();
//}

//void Enemy::UpdateDirection(void)
//{
//	//方向の更新
//	actionCtrl_->Update();
//
//	//アニメーションの更新
//	animCtrl_->Update();
//
//	//Transformの更新
//	trans_.quaRot = charaRot_.playerRotY_;
//	trans_.Update();
//}

void Enemy::UpdateDirectionCharacter(void)
{
	//咆哮演出の更新
	UpdateRoarDirection();
}

//void Enemy::UpdateClearDirection(void)
//{
//	animCtrl_->Update();
//
//	trans_.Update();
//}

void Enemy::UpdateClearDirectionCharacter(void)
{
	VECTOR effPos = MV1GetFramePosition(trans_.modelId, CHEST_FRAME_NO);
	effect_->SetPos(EffectController::EFF_TYPE::E_DEATH, 0, effPos);
	effect_->Update();
	if (animCtrl_->GetAnimStep(static_cast<int>(CharacterBase::ANIM_TYPE::DEATH)) >= DEATH_BLAST_ANIM_STEP)
	{
		if (modelScl_ <= 0.0f)
		{
			isEndClearDirect_ = true;
			return;
		}
		modelScl_ -= 0.1f;
		trans_.scl = { modelScl_,modelScl_,modelScl_ };
	}
}

void Enemy::UpdateOverDirection(void)
{
	constexpr float ANIM_SPD_SCL = 0.2f;
	animCtrl_->Update(ANIM_SPD_SCL);
}

void Enemy::UpdateOverDirectionCharacter(void)
{
	constexpr float ANIM_SPD_SCL = 0.2f;
	animCtrl_->Update(ANIM_SPD_SCL);
}

void Enemy::Draw(void)
{
	//通常描画
	MV1DrawModel(trans_.modelId);

	//岩の描画
	if (!drawableRocks_.empty())
	{
		for (auto& rock : drawableRocks_)
		{
			rock.lock()->Draw();
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
		animCtrl_->PlayBlend(static_cast<int>(ANIM_TYPE::ROAR_ATK), roarAnim_);
		float roarAnimStep = animCtrl_->GetAnimStep(static_cast<int>(CharacterBase::ANIM_TYPE::ROAR_ATK));
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
		animCtrl_->PlayBlend(static_cast<int>(ANIM_TYPE::IDLE), idleAnim_);
	}
}

void Enemy::ChangeUpdateClearDirection(void)
{
	isRoar_ = false;
	VECTOR effPos = MV1GetFramePosition(trans_.modelId, CHEST_FRAME_NO);
	soundMng_.Stop(ResourceManager::SRC::ENEMY_FOOT_SE);
	soundMng_.Stop(ResourceManager::SRC::ENEMY_JUMP_LAND_SE);
	soundMng_.Stop(ResourceManager::SRC::ENEMY_CHARGE_SE);
	effect_->Play(EffectController::EFF_TYPE::E_DEATH, effPos, trans_.quaRot, DEATH_EFF_SCL_VEC);
	animCtrl_->PlayBlend(static_cast<int>(ANIM_TYPE::DEATH), deathAnim_);
	CharacterBase::ChangeUpdateClearDirection();
}

void Enemy::LoadCharacter(void)
{
	//エフェクト
	effect_->Add(resMng_.Load(ResourceManager::SRC::E_DEATH_EFF).handleId_, EffectController::EFF_TYPE::E_DEATH);

	//Jsonからアクションごとのデータのロード
	LoadAddAnimation([this](const ACTION_LOAD_DATA& animVar)
		{
			//アクションコントローラーの全行動クラスに通知
			actionCtrl_->AnimLoadNotify(animVar);
			if (animVar.name == "Death")
			{
				deathAnim_ = animVar.animVariable;
			}
			else if (animVar.name == "Idle")
			{
				idleAnim_ = animVar.animVariable;
			}
			else if (animVar.name == "Roar")
			{
				roarAnim_ = animVar.animVariable;
			}
		});
}

void Enemy::MakeColliderGeometry(void)
{
	MakeColliderFromJsonData();
	onHit_ = std::make_unique<EnemyOnHit>(*this, movedPos_, moveDiff_, *actionCtrl_, collider_, trans_);
}
void Enemy::UpdateNormal(void)
{
	////ロジックの更新
	//logic_->Update();

	////アクションの更新
	//actionCtrl_->Update();

	//アニメーションの更新
	animCtrl_->Update();

	//回転の同期
	UpdatePost();

	//Transformの更新
	trans_.quaRot = charaRot_.playerRotY_;
	trans_.Update();
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
