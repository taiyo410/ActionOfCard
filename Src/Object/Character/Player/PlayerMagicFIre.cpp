#include "../pch.h"
#include "../Utility/Utility3D.h"
#include "../Manager/Resource/ResourceManager.h"
#include "../Object/Common/Collider.h"
#include "../Object/Common/Geometry/Sphere.h"
#include "PlayerMagicFIre.h"

PlayerMagicFire::PlayerMagicFire(VECTOR& _startPos, VECTOR& _dir) :
	startPos_(_startPos),
	dir_(_dir),
	isAlive_(false),
	moveSpd_()
{
	noneHitTag_.emplace(Collider::TAG::STAGE);
	noneHitTag_.emplace(Collider::TAG::CAMERA);
	noneHitTag_.emplace(Collider::TAG::PLAYER1);
	noneHitTag_.emplace(Collider::TAG::NML_ATK);

	tag_ = Collider::TAG::FIRE;
}

PlayerMagicFire::~PlayerMagicFire(void)
{
}

void PlayerMagicFire::Load(void)
{

}

void PlayerMagicFire::Init(void)
{
	std::unique_ptr<Geometry> geo = std::make_unique<Sphere>(trans_.pos, colRadius_);
	MakeCollider(TAG_PRIORITY::FIRE_SPHERE, { tag_ }, std::move(geo), noneHitTag_);
	trans_.pos = startPos_;
	isAlive_ = true;
}

void PlayerMagicFire::Update(void)
{
	//âäÇÃà⁄ìÆ
	trans_.pos = VAdd(trans_.pos,VScale(dir_, moveSpd_));
}

void PlayerMagicFire::Draw(void)
{
	for(const auto& col : collider_)
	{
		col.second->GetGeometry().Draw();
	}
}

void PlayerMagicFire::OnHit(const std::weak_ptr<Collider> _hitCol)
{
	//ìñÇΩÇ¡ÇΩÇÁè¡Ç∑
	DeleteCollider(TAG_PRIORITY::FIRE_SPHERE);
}

void PlayerMagicFire::LoadFireData(const nlohmann::json& _jsonData)
{
	if (_jsonData.contains("attackPoint"))
	{
		atkPow_ = _jsonData.value("attackPoint", 0.0f);
	}
	if (_jsonData.contains("fireRadius"))
	{
		colRadius_ = _jsonData.value("fireRadius", 0.0f);
	}
	if (_jsonData.contains("fireRadius"))
	{
		colRadius_ = _jsonData.value("fireRadius", 0.0f);
	}
	if (_jsonData.contains("moveSpeed"))
	{
		moveSpd_ = _jsonData.value("moveSpeed", 0.0f);
	}
}
