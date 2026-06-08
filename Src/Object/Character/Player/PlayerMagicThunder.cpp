#include "../pch.h"
#include "../Utility/Utility3D.h"
#include "../Utility/UtilityCommon.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Resource/ResourceManager.h"
#include "../Object/Common/Collider.h"
#include "../Object/Common/Geometry/Line.h"
#include "../Object/Common/EffectController.h"
#include "PlayerMagicThunder.h"

PlayerMagicThunder::PlayerMagicThunder(VECTOR& _targetPos):
	targetPos_(_targetPos)
{
	noneHitTag_.emplace(Collider::TAG::STAGE);
	noneHitTag_.emplace(Collider::TAG::CAMERA);
	noneHitTag_.emplace(Collider::TAG::PLAYER1);
	noneHitTag_.emplace(Collider::TAG::NML_ATK);
}

void PlayerMagicThunder::Load(void)
{
}

void PlayerMagicThunder::Init(void)
{
	trans_.pos = targetPos_;
}

void PlayerMagicThunder::Update(void)
{

}

void PlayerMagicThunder::Draw(void)
{
	for (const auto& col : collider_)
	{
		col.second->GetGeometry().Draw();
	}
}

void PlayerMagicThunder::OnHit(const std::weak_ptr<Collider> _hitCol)
{
	//ìñÇΩÇ¡ÇΩÇÁè¡Ç∑
	DeleteCollider(TAG_PRIORITY::FIRE_SPHERE);
}

void PlayerMagicThunder::LoadThunderData(const nlohmann::json _jsonData)
{
}

void PlayerMagicThunder::MakeThunderCollider(void)
{
	VECTOR localPos1 = { trans_.pos.x,trans_.pos.y - 500.0f,trans_.pos.z };
	VECTOR localPos2 = { trans_.pos.x,trans_.pos.y + 500.0f,trans_.pos.z };
	std::unique_ptr<Geometry> geo = std::make_unique<Line>(trans_.pos, trans_.quaRot, localPos1, localPos2);
	MakeCollider(TAG_PRIORITY::FIRE_SPHERE, { tag_ }, std::move(geo), noneHitTag_);
}
