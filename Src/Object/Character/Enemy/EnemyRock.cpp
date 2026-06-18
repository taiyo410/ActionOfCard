#include "pch.h"
#include "Utility/Utility3D.h"
#include "Utility/UtilityCommon.h"
#include "Manager/Resource/ResourceManager.h"
#include "Object/Common/Collider.h"
#include "Object/Common/Geometry/Sphere.h"
#include "EnemyRock.h"

EnemyRock::EnemyRock(int& _num, VECTOR& _startPos):
	createNum_(_num),
	startPos_(_startPos),
	distance_(),
	jumpPow_(),
	isAlive_(false),
	isDamaged_(false),
	modelId_(UtilityCommon::INITIAL_HANDLE),
	centerPos_({}),
	goalPos_({}),
	velocity_()
{
	objectName_ = ROCK_STR;
}

EnemyRock::~EnemyRock(void)
{
}

void EnemyRock::Load(void)
{
	//モデル
	trans_.modelId = resMng_.LoadModelDuplicate(ResourceManager::SRC::SPHERE_ROCK);
}

void EnemyRock::Init(void)
{
	//何個目に生成されたかで角度を変える
	float angle = aroundPerRad_ * createNum_;

	goalPos_.x = startPos_.x + sin(angle) * moveDistance_;
	goalPos_.y = 0.0f;
	goalPos_.z = startPos_.z - cos(angle) * moveDistance_;

	tag_ = Collider::TAG::ROCK;
	velocity_.y = sqrtf(2.0f * gravity_ * jumpHeight_);

	isDamaged_ = false;

	noneHitTag_.emplace(Collider::TAG::STAGE);
	noneHitTag_.emplace(Collider::TAG::CAMERA);
	noneHitTag_.emplace(Collider::TAG::ENEMY1);
	noneHitTag_.emplace(Collider::TAG::NML_ATK);

	std::unique_ptr<Geometry> geo = std::make_unique<Sphere>(trans_.pos, radius_);
	MakeCollider(TAG_PRIORITY::ROCK_SPHERE, {tag_ }, std::move(geo),noneHitTag_);

	trans_.pos = startPos_;
}

void EnemyRock::Update(void)
{
	//生存していなければ処理しない
	if (!isAlive_)return;

	//重力分加速度を引く
	velocity_.y -= gravity_;
	jumpPow_.y = velocity_.y;

	//岩座標を更新する
	VECTOR vec = VNorm(VSub(goalPos_, startPos_));
	trans_.pos = VAdd(trans_.pos, VScale(vec, horizontalMoveSpd_));
	trans_.pos = VAdd(trans_.pos, jumpPow_);

	trans_.Update();
}

void EnemyRock::Draw(void)
{
	//生存していなければ処理しない
	if (!isAlive_)return;
	MV1DrawModel(trans_.modelId);
}

void EnemyRock::OnHit(const std::weak_ptr<Collider> _hitCol)
{
}

void EnemyRock::DeleteRockCollider(void)
{
	DeleteCollider(TAG_PRIORITY::ROCK_SPHERE);
}

void EnemyRock::LoadRockData(const nlohmann::json _jsonData)
{
	atkPow_ = _jsonData.value("attackPoint", 0.0f);
	gravity_ = _jsonData.value("gravity",0.0f);
	jumpHeight_ = _jsonData.value("jumpHeight", 0.0f);
	rockNum_ = _jsonData.value("rockNum", 0);
	radius_ = _jsonData.value("rockRadius", 0.0f);
	horizontalMoveSpd_ = _jsonData.value("moveHorizontalSpeed", 0.0f);
	moveDistance_= _jsonData.value("moveDistance", 0.0f);
	float aroundPerDeg= 360.0f / rockNum_;
	//ラジアン変換
	aroundPerRad_ = aroundPerDeg * DX_PI_F / 180.0f;
}