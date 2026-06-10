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
	targetPos_(_targetPos),
	isAlive_(false)
{
	objectName_ = THUNDER_STR;
	tag_ = Collider::TAG::THUNDER;
}

PlayerMagicThunder::~PlayerMagicThunder(void)
{
}

void PlayerMagicThunder::Load(void)
{
	effect_->Add(resMng_.Load(ResourceManager::SRC::THUNDER_EFF).handleId_,EffectController::EFF_TYPE::THUNDER);
}

void PlayerMagicThunder::Init(void)
{
	isAlive_ = true;
	trans_.pos = targetPos_;
	VECTOR sclVec = { thunderEffScl_ ,thunderEffScl_ ,thunderEffScl_ };
	thunderEffPlayId_ = effect_->Play(EffectController::EFF_TYPE::THUNDER, trans_.pos, trans_.quaRot, sclVec);
}

void PlayerMagicThunder::Update(void)
{
	if (effect_->IsEnd(EffectController::EFF_TYPE::THUNDER, thunderEffPlayId_))
	{
		isAlive_ = false;
	}
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

void PlayerMagicThunder::LoadThunderData(const nlohmann::json _data)
{
	thunderEffScl_ = _data.value("effectScale", 0.0f);
}

void PlayerMagicThunder::MakeThunderCollider(void)
{
	MakeColliderFromJsonData();
}
