#include "Utility/Utility3D.h"
#include "Manager/Generic/Camera.h"
#include "Manager/Generic/SceneManager.h"
#include "Manager/Generic/InputManager.h"
#include "Object/Common/EffectController.h"
#include "Object/Card/CardDeck.h"
#include "Object/Character/Action/ActionController.h"
#include "./EnemyOnHit.h"
#include "./EnemyRock.h"
#include "./EnemyLogic.h"
#include "./Enemy.h"

Enemy::Enemy(void):
	modelScl_(MODEL_SIZE_MULTIPLITER)
{
	objectName_ = ENEMY_STR;
	characterType_ = CHARACTER_TYPE::ENEMY;
	isRoar_ = false;
	logic_ = std::make_unique<EnemyLogic>(trans_);
	deck_ = std::make_shared<CardDeck>(characterType_, ENEMY_NUM);
	cardPresent_ = std::make_unique<CardPresenter>(characterType_, *deck_);
	effect_ = std::make_unique<EffectController>();
	tag_ = Collider::TAG::ENEMY1;
	//アクション
	actionCtrl_ = std::make_unique<ActionController>(*this, *logic_, trans_, *cardPresent_, *animCtrl_, InputManager::JOYPAD_NO::PAD1);
}

Enemy::~Enemy(void)
{
	collider_.clear();
	effect_->AllStop();
}

void Enemy::UpdateNormalCharacter(void)
{

}

void Enemy::UpdateDirectionCharacter(void)
{
	//咆哮演出の更新
	UpdateRoarDirection();
}
void Enemy::UpdateClearDirectionCharacter(void)
{
	VECTOR effPos = MV1GetFramePosition(trans_.modelId, chestFrameNum_);
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

void Enemy::UpdateOverDirectionCharacter(void)
{
	constexpr float ANIM_SPD_SCL = 0.2f;
	animSpdScl_ = ANIM_SPD_SCL;
}

void Enemy::DrawCharacter(void)
{
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
	VECTOR effPos = MV1GetFramePosition(trans_.modelId, chestFrameNum_);
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
}

void Enemy::InitCharacter(void)
{
}

void Enemy::MakeColliderGeometry(void)
{
	MakeColliderFromJsonData();
	onHit_ = std::make_unique<EnemyOnHit>(*this, movedPos_, moveDiff_, *actionCtrl_, collider_, trans_);
}
void Enemy::LoadCharacterActionDataCallBack(const ACTION_LOAD_DATA& _animVar)
{
	if (_animVar.name == "Death")
	{
		deathAnim_ = _animVar.animVariable;
	}
	else if (_animVar.name == "Idle")
	{
		idleAnim_ = _animVar.animVariable;
	}
	else if (_animVar.name == "Roar")
	{
		roarAnim_ = _animVar.animVariable;
	}
}

void Enemy::LoadModelDataCharacter(const nlohmann::json& _data)
{
	chestFrameNum_ = _data.value("chestFrameNum", 0);
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
