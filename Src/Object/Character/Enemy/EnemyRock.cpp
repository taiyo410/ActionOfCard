#include "EnemyRock.h"
#include "../pch.h"
#include "../Utility/Utility3D.h"
#include "../Utility/UtilityCommon.h"
#include "../Object/Common/Collider.h"
#include "../Object/Common/Geometry/Sphere.h"
#include "../Manager/Resource/ResourceManager.h"
#include "EnemyRock.h"

EnemyRock::EnemyRock(int& _num, VECTOR& _startPos):
	num_(_num),
	startPos_(_startPos),
	distance_(),
	jumpPow_(),
	isAlive_(false),
	isDamaged_(false),
	modelId_(UtilityCommon::INITIAL_HANDLE)
{
}

EnemyRock::~EnemyRock(void)
{
}

void EnemyRock::Load(void)
{
	//モデル
	trans_.modelId = ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::SPHERE_ROCK);

	//岩攻撃力を外部から取得
	LoadAttackPow();
}

void EnemyRock::Init(void)
{
	//何個目に生成されたかで角度を変える
	float angle = ARROUND_PER_RAD * num_;

	goalPos_.x = startPos_.x + sin(angle) * DISTANCE_RADIUS;
	goalPos_.y = 0.0f;
	goalPos_.z = startPos_.z - cos(angle) * DISTANCE_RADIUS;

	tag_ = Collider::TAG::ROCK;
	velocity_.y = sqrtf(2.0f * GRAVITY * JUMP_HEIGHT);

	isDamaged_ = false;

	noneHitTag_.emplace(Collider::TAG::STAGE);
	noneHitTag_.emplace(Collider::TAG::CAMERA);
	noneHitTag_.emplace(Collider::TAG::ENEMY1);
	noneHitTag_.emplace(Collider::TAG::NML_ATK);

	std::unique_ptr<Geometry> geo = std::make_unique<Sphere>(trans_.pos, ROCK_COL_RADIUS);
	MakeCollider(TAG_PRIORITY::ROCK_SPHERE, {tag_ }, std::move(geo),noneHitTag_);

	trans_.pos = startPos_;
}

void EnemyRock::Update(void)
{
	//生存していなければ処理しない
	if (!isAlive_)return;

	//重力分加速度を引く
	velocity_.y -= GRAVITY;
	jumpPow_.y = velocity_.y;

	//岩座標を更新する
	VECTOR vec = VNorm(VSub(goalPos_, startPos_));
	trans_.pos = VAdd(trans_.pos, VScale(vec, MOVE_HORIZONTAL_SPD));
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

void EnemyRock::LoadAttackPow(void)
{
	nlohmann::json j = resMng_.Load(ResourceManager::SRC::CHARA_DATA).jsonData;
	const std::string DATA_NAME = "EnemyAttack";
	for (const auto& data : j[DATA_NAME])
	{
		if (data.contains("StompAttack"))
		{
			auto& atk = data.at("StompAttack");
			if (atk.contains("attackPoint"))
			{
				atkPow_ = atk.value("attackPoint", 0.0f);
			}
		}
	}
}
