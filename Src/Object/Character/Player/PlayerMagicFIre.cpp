#include "../pch.h"
#include "../Utility/Utility3D.h"
#include "../Manager/Resource/ResourceManager.h"
#include "../Object/Common/Collider.h"
#include "../Object/Common/Geometry/Sphere.h"
#include "PlayerMagicFIre.h"

PlayerMagicFire::PlayerMagicFire(VECTOR& _dir) :
	dir_(_dir)
{
}

PlayerMagicFire::~PlayerMagicFire(void)
{
}

void PlayerMagicFire::Load(void)
{

}

void PlayerMagicFire::Init(void)
{
	noneHitTag_.emplace(Collider::TAG::STAGE);
	noneHitTag_.emplace(Collider::TAG::CAMERA);
	noneHitTag_.emplace(Collider::TAG::PLAYER1);
	noneHitTag_.emplace(Collider::TAG::NML_ATK);

	std::unique_ptr<Geometry> geo = std::make_unique<Sphere>(trans_.pos, colRadius_);
	MakeCollider(TAG_PRIORITY::FIRE_SPHERE, { tag_ }, std::move(geo), noneHitTag_);
}

void PlayerMagicFire::Update(void)
{
	//âäÇÃà⁄ìÆ
	trans_.pos = VScale(dir_, moveSpd_);
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

void PlayerMagicFire::LoadJsonData(const auto& _jsonData)
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
}
