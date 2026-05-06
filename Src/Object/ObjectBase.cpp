#include "../Manager/Resource/ResourceManager.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Game/CollisionManager.h"
#include "ObjectBase.h"

ObjectBase::ObjectBase(void)
	: resMng_(ResourceManager::GetInstance()),
	scnMng_(SceneManager::GetInstance()),
	tag_(Collider::TAG::NONE),
	trans_()
{
}

ObjectBase::~ObjectBase(void)
{
	collider_.clear();
}
void ObjectBase::OnHit(const std::weak_ptr<Collider> _hitCol)
{
}

void ObjectBase::MakeCollider(const TAG_PRIORITY _tagPriority, const std::set<Collider::TAG> _tag, std::unique_ptr<Geometry> _geometry, const std::set<Collider::TAG> _notHitTags)
{
	//情報を使ってコライダの作成
	std::shared_ptr col = std::make_shared<Collider>(*this, _tag, std::move(_geometry), _notHitTags);
	collider_.emplace(_tagPriority,col);

	//コライダを管理マネージャーに追加
	CollisionManager::GetInstance().AddCollider(col);
}

const bool ObjectBase::IsAliveCollider(const Collider::TAG _chataTag, const Collider::TAG _tag)
{
	for (auto& col:collider_)
	{
		auto tags = col.second->GetTags();
		//特定のタグを見つけるまでイテレータを回す
		//発見できなかった場合、最後まで回る
		auto it = tags.find(_chataTag);
		if (it != tags.end())
		{
			//キャラタグを発見したら、さらに攻撃の当たり判定を探す
			auto it2 = tags.find(_tag);
			if (it2 != tags.end())
			{
				return true;
			}
		}
	}
	return false;
}


void ObjectBase::DeleteCollider(const TAG_PRIORITY _priority)
{
	//特定のタグを探す
	auto it = collider_.find(_priority);

	//見つからない、もしくは空の場合は飛ばす
	if (it == collider_.end() || it->second == nullptr)return;

	//コライダの削除
	collider_[_priority]->Kill();

	//配列の削除
	std::erase_if(collider_, [](auto& col) {return col.second->IsDead(); });
}

void ObjectBase::DeleteAllCollider(void)
{
	for (auto& col : collider_)
	{
		//当たり判定が存在しないならスキップ
		if (col.second == nullptr)continue;

		//コライダの削除
		col.second->Kill();
	}

	//当たり判定情報の削除
	collider_.clear();
}

