#include "../pch.h"
#include "../Utility/Utility3D.h"
#include "../Utility/UtilityCommon.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Resource/ResourceManager.h"
#include "../Object/Common/Collider.h"
#include "../Object/Common/Geometry/Sphere.h"
#include "../Object/Common/EffectController.h"
#include "PlayerMagicFIre.h"

PlayerMagicFire::PlayerMagicFire(VECTOR& _startPos, VECTOR& _dir) :
	startPos_(_startPos),
	dir_(_dir),
	isAlive_(false),
	moveSpd_(),
	fireBallEffScl_(),
	effAlpha_(),
	effDeleteCnt_()
{
	noneHitTag_.emplace(Collider::TAG::STAGE);
	noneHitTag_.emplace(Collider::TAG::CAMERA);
	noneHitTag_.emplace(Collider::TAG::PLAYER1);
	noneHitTag_.emplace(Collider::TAG::NML_ATK);
	objectName_=FIRE_STR;

	tag_ = Collider::TAG::FIRE;
}

PlayerMagicFire::~PlayerMagicFire(void)
{
}

void PlayerMagicFire::Load(void)
{
	effect_->Add(resMng_.Load(ResourceManager::SRC::FIRE_BALL_EFF).handleId_, EffectController::EFF_TYPE::FIRE_BALL);
}

void PlayerMagicFire::Init(void)
{
	trans_.pos = startPos_;
	isAlive_ = true;
	isDamage_ = false;
	effDeleteCnt_ = effDeleteTime_;
	updateState_ = [this]() {UpdateNormal(); };
	//エフェクト再生
	fireEffPlayId_=effect_->Play(EffectController::EFF_TYPE::FIRE_BALL
		, trans_.pos, trans_.quaRot
		, { fireBallEffScl_,fireBallEffScl_ ,fireBallEffScl_ });

	effDeleteScl_ = fireBallEffScl_;
}

void PlayerMagicFire::Update(void)
{
	updateState_();
	effect_->Update();
}

void PlayerMagicFire::Draw(void)
{
}

void PlayerMagicFire::OnHit(const std::weak_ptr<Collider> _hitCol)
{
	//当たったら消す
	DeleteCollider(TAG_PRIORITY::FIRE_SPHERE);

	//エフェクトのフェードアウト
	ChangeDelete();
}

void PlayerMagicFire::LoadFireData(const nlohmann::json& _jsonData)
{
	atkPow_ = _jsonData.value("attackPoint", 0.0f);
	colRadius_ = _jsonData.value("fireRadius", 0.0f);
	fireBallEffScl_ = _jsonData.value("effectScale", 0.0f);
	moveSpd_ = _jsonData.value("moveSpeed", 0.0f);
	effDeleteTime_ = _jsonData.value("effectDeleteTime", 0.0f);
}

void PlayerMagicFire::MakeFireBallCollider(void)
{
	std::unique_ptr<Geometry> geo = std::make_unique<Sphere>(trans_.pos, colRadius_);
	MakeCollider(TAG_PRIORITY::FIRE_SPHERE, { tag_ }, std::move(geo), noneHitTag_);
}

void PlayerMagicFire::ChangeDelete(void)
{
	updateState_ = [this]() {UpdateFadeOut(); };
}

void PlayerMagicFire::UpdateNormal(void)
{
	//炎の移動
	trans_.pos = VAdd(trans_.pos, VScale(dir_, moveSpd_));
	effect_->SetPos(EffectController::EFF_TYPE::FIRE_BALL, fireEffPlayId_, trans_.pos);
}

void PlayerMagicFire::UpdateFadeOut(void)
{
	if (effDeleteCnt_ <= 0.0f&& isAlive_)
	{
		effect_->Stop(EffectController::EFF_TYPE::FIRE_BALL, fireEffPlayId_);
		isAlive_ = false;
		return;
	}

	//エフェクトが爆発して消えていく
	float per = effDeleteCnt_ / effDeleteTime_;
	//サイズを大きくしていく
	float scl = fireBallEffScl_ + fireBallEffScl_ * (UtilityCommon::RATIO_MAX - per);

	//アルファ値はなくしていく
	float alpha = static_cast<float>(UtilityCommon::ALPHA_MAX) * per;

	effect_->SetScale(EffectController::EFF_TYPE::FIRE_BALL, fireEffPlayId_, { scl,scl,scl });
	effect_->SetColor(EffectController::EFF_TYPE::FIRE_BALL, fireEffPlayId_
		, { UtilityCommon::ALPHA_MAX,UtilityCommon::ALPHA_MAX,UtilityCommon::ALPHA_MAX,static_cast<int>(alpha) });

	effDeleteCnt_ -= scnMng_.GetDeltaTime();
}
