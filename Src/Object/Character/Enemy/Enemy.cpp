#include "Utility/Utility3D.h"
#include "Utility/UtilityJson.h"
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
	deathEffPlayId_(UtilityCommon::INITIAL_HANDLE)
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
	effect_->SetPos(EffectController::EFF_TYPE::E_DEATH, deathEffPlayId_, effPos);
	effect_->Update();
	if (animCtrl_->GetAnimStep(static_cast<int>(CharacterBase::ANIM_TYPE::DEATH)) >= deathEffectStartAnimStep_)
	{
		if (modelScl_ <= 0.0f)
		{
			isEndClearDirect_ = true;
			return;
		}
		modelScl_ -= deathSclDownDiff_;
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
		const float ROAR_TIME = roarEndAnimStep_ - roarStartAnimStep_;
		if (roarAnimStep >= roarStartAnimStep_)
		{
			//カメラシェイク
			float t = (roarAnimStep - roarStartAnimStep_) / ROAR_TIME;
			scnMng_.GetCamera().lock()->SetShakeStatus(t, roarCameraShakeLimit_);
			scnMng_.GetCamera().lock()->ChangeSub(Camera::SUB_MODE::SHAKE);
			soundMng_.PlayOnce(SoundManager::SRC::ENEMY_ROAR_SE, SoundManager::PLAYTYPE::BACK);

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
	soundMng_.Stop(ResourceManager::SRC::ENEMY_FOOT_SE);
	soundMng_.Stop(ResourceManager::SRC::ENEMY_JUMP_LAND_SE);
	soundMng_.Stop(ResourceManager::SRC::ENEMY_CHARGE_SE);
	VECTOR effPos = MV1GetFramePosition(trans_.modelId, chestFrameNum_);
	deathEffPlayId_= effect_->Play(EffectController::EFF_TYPE::E_DEATH, effPos, trans_.quaRot, deathEffectScale_);
	animCtrl_->PlayBlend(static_cast<int>(ANIM_TYPE::DEATH), deathAnim_);
	CharacterBase::ChangeUpdateClearDirection();
}

void Enemy::LoadCharacter(void)
{
	//エフェクト
	effect_->Add(resMng_.Load(ResourceManager::SRC::E_DEATH_EFF).handleId_, EffectController::EFF_TYPE::E_DEATH);
	resMng_.Load(ResourceManager::SRC::ENEMY_ROAR_SE);
}

void Enemy::InitCharacter(void)
{
}

void Enemy::MakeColliderGeometry(void)
{
	MakeColliderFromJsonData();
	onHit_ = std::make_unique<EnemyOnHit>(*this, movedPos_, moveDiff_, *actionCtrl_, collider_, trans_);
}
void Enemy::LoadCharacterActionDataCallBack(const ACTION_LOAD_DATA& _actionVar)
{
	if (_actionVar.name == "Death")
	{
		deathAnim_ = _actionVar.animVariable;
		deathEffectScale_ =UtilityJson::GetLoadVector3("deathEffectScale", _actionVar.jsonData);							//死亡エフェクト
		deathEffectStartAnimStep_ = _actionVar.jsonData.value("deathEffectStartAnimStep",0.0f);
		deathSclDownDiff_ = _actionVar.jsonData.value("deathScaleDownDiff", 0.0f);
	}
	else if (_actionVar.name == "Idle")
	{
		idleAnim_ = _actionVar.animVariable;
	}
	else if (_actionVar.name == "Roar")
	{
		roarAnim_ = _actionVar.animVariable;
		roarStartAnimStep_ = _actionVar.jsonData.value("roarStartAnimStep", 0.0f);
		roarEndAnimStep_ = _actionVar.jsonData.value("roarEndAnimStep", 0.0f);
		roarCameraShakeLimit_ = _actionVar.jsonData.value("cameraShakeLimit", 0.0f);
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
