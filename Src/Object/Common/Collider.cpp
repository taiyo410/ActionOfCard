#include "../ObjectBase.h"
#include "../Character/Base/CharacterBase.h"
#include "Geometry/Geometry.h"
#include "Collider.h"

Collider::Collider(ObjectBase& _parent, const std::set<TAG> _tags, std::unique_ptr<Geometry>_geometry, const std::set<TAG> _notHitTags) :
	parent_(_parent),
	tags_(_tags),
	geometry_(std::move(_geometry)),
	notHitTags_(_notHitTags)
{
	isHit_ = false;
	isDead_ = false;

	tagTable_.emplace_back(TAG::PLAYER1);
	tagTable_.emplace_back(TAG::ENEMY1);
	tagTable_.emplace_back(TAG::STAGE);
	tagTable_.emplace_back(TAG::NML_ATK);
}

Collider::~Collider(void)
{
	tagTable_.clear();
}

Geometry& Collider::GetGeometry(void) const
{
	return *geometry_;
}

CharacterBase& Collider::GetParentCharacter(void)
{
	CharacterBase& chara = dynamic_cast<CharacterBase&>(parent_);
	assert(chara && NOT_CHARACTER_STR);
	return chara;
}

void Collider::OnHit(const std::weak_ptr<Collider> _collider)
{
	//この当たり判定が当たった
	isHit_ = true;

	//親に相手のコライダを渡す
	parent_.OnHit(_collider);
}
