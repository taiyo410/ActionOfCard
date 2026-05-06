#include"../Object/Common/Transform.h"
#include"../Object/Common/Geometry/Geometry.h"
#include"../Object/Common/Collider.h"
#include"../Object/ObjectBase.h"
#include"../Utility/Utility3D.h"
#include "CollisionManager.h"

void CollisionManager::AddCollider(const std::shared_ptr<Collider> _collider)
{
	//コライダの追加
	colliders3D_.push_back(_collider);

	//コライダーの並べ替え
	std::sort(colliders3D_.begin(), colliders3D_.end(), [this](std::weak_ptr<Collider> a, std::weak_ptr<Collider> b) {
		return static_cast<int>(GetTopTags(a)) < static_cast<int>(GetTopTags(b));
		});
}

void CollisionManager::Sweep(void)
{
	//終了したコライダの並び変え
	auto it = std::remove_if(colliders3D_.begin(), colliders3D_.end(),
		[](const std::shared_ptr<Collider> _col)
		{
			return _col->IsDead();
		});

	//終了したコライダを削除
	colliders3D_.erase(it, colliders3D_.end());
}

void CollisionManager::Update(void)
{
	//コライダが一つもないなら処理を飛ばす
	if (colliders3D_.empty())return;

	//当たり判定フレーム
	if (updateFrame_ < COL_UPDATE_FRAME)
	{
		//カウンタ
		updateFrame_++;
		return;
	}

	for (int i = 0; i < colliders3D_.size() - 1; i++)
	{
		for (int j = i + 1; j < colliders3D_.size(); j++)
		{
			//当たり判定をするか
			if (!JudgeIsCollision(i, j))
			{
				//当たり判定の条件に合わなかったので飛ばす
				continue;
			}
		
			if (GetTopTags(colliders3D_[i]) == Collider::TAG::STAGE &&
				GetTopTags(colliders3D_[j]) == Collider::TAG::CAMERA)
			{
				//当たり判定
				if (IsCollision(colliders3D_[i], colliders3D_[j]))
				{
					//それぞれの当たった処理
					colliders3D_[i]->OnHit(colliders3D_[j]);
					colliders3D_[j]->OnHit(colliders3D_[i]);

					//当たった後の処理
					colliders3D_[i]->GetGeometry().HitAfter();
					colliders3D_[j]->GetGeometry().HitAfter();
				}
				continue;
			}

			//当たり判定
			if(IsCollision(colliders3D_[i],colliders3D_[j]))
			{
				//それぞれの当たった処理
				colliders3D_[i]->OnHit(colliders3D_[j]);
				colliders3D_[j]->OnHit(colliders3D_[i]);
				
				//当たった後の処理
				colliders3D_[i]->GetGeometry().HitAfter();
				colliders3D_[j]->GetGeometry().HitAfter();
			}
		}
	}
	//カウンタの初期化
	updateFrame_ = 0;
}

const bool CollisionManager::IsPlayer(const Collider::TAG _tag)const
{
	return _tag == Collider::TAG::PLAYER1;
}

const bool CollisionManager::IsItem(const Collider::TAG _tag) const
{
	//今のところはプレイヤー以外がアイテム
	return !IsPlayer(_tag);
}

CollisionManager::CollisionManager(void)
{
	updateFrame_ = 0;

	//ここに当たり判定する範囲の広さをタグごとで設定
	hitRange_[Collider::TAG::PLAYER1] = HIT_RANGE_NORMAL;
	hitRange_[Collider::TAG::ENEMY1] = HIT_RANGE_NORMAL;
	hitRange_[Collider::TAG::NML_ATK] = HIT_RANGE_NORMAL;
	hitRange_[Collider::TAG::JUMP_ATK] = HIT_RANGE_NORMAL;
	hitRange_[Collider::TAG::CAMERA] = HIT_RANGE_CAMERA;
	hitRange_[Collider::TAG::STAGE] = HIT_RANGE_STAGE;
	hitRange_[Collider::TAG::ROCK] = HIT_RANGE_ROCK;
}

CollisionManager::~CollisionManager(void)
{
	//コライダの全削除
	colliders3D_.clear();

	//判定を行う範囲の削除
	hitRange_.clear();
}

const bool CollisionManager::IsWithInHitRange(const std::weak_ptr<Collider> _col1, const std::weak_ptr<Collider> _col2) const
{
	//総合
	bool ret = false;

	// weak_ptrをshared_ptrに昇格
	auto col1 = _col1.lock();
	auto col2 = _col2.lock();

	// 有効性チェック
	if (!col1 || !col2)
	{
		// どちらかが既に破棄されている
		return false;
	}

	auto& geom1 = col1->GetGeometry();
	auto& geom2 = col2->GetGeometry();

	const VECTOR pos1 = geom1.GetColPos();
	const VECTOR pos2 = geom2.GetColPos();

	//双方の距離
	double sqrtDis = Utility3D::SqrMagnitude(
		pos1,
		pos2);

	//双方のタグ
	for (const auto tag1 : _col1.lock()->GetTags())
	{
		for (const auto tag2 : _col2.lock()->GetTags())
		{
			//距離範囲の比較
			float range = hitRange_.at(tag1) >= hitRange_.at(tag2) ? hitRange_.at(tag1) : hitRange_.at(tag2);

			//範囲内かの比較
			ret = sqrtDis <= range * range;

			//当たったなら強制終了
			if (ret)
			{
				//当たった
				return true;
			}
		}
	}
	//当たらなかった
	return false;
}

const bool CollisionManager::JudgeIsCollision(const int _col1Num, const int _col2Num)const
{
	//範囲内か
	if (!IsWithInHitRange(colliders3D_[_col1Num], colliders3D_[_col2Num]))
	{
		//範囲内でなかった
		return false;
	}

	//双方のタグ
	const auto& tags1 = colliders3D_[_col1Num]->GetTags();
	const auto& tags2 = colliders3D_[_col2Num]->GetTags();



	
	//1人目のタグ
	for (auto tag1 : tags1)
	{
		//2人目のタグ
		for (auto tag2 : tags2)
		{
			//双方のタグが違うタグか
			if (tag1 == tag2)
			{
				//同じタグを持っていた
				return false;
			}
		}
	}

	//双方の当たり判定しないタグか
	//1人目のタグ
	for (auto tag1 : colliders3D_[_col1Num]->GetTags())
	{
		//2人目の当たり判定しないタグ
		for (auto notColTag2 : colliders3D_[_col2Num]->GetNotHitTags())
		{
			if (tag1 == notColTag2)
			{
				//1人目のタグが2人目の当たり判定しないタグと同一だった
				return false;
			}
		}
	}

	//2人目のタグ
	for (auto tag2 : colliders3D_[_col2Num]->GetTags())
	{
		//1人目の当たり判定しないタグ
		for (auto notColTag1 : colliders3D_[_col1Num]->GetNotHitTags())
		{
			if (tag2 == notColTag1)
			{
				//2人目のタグが1人目の当たり判定しないタグと同一だった
				return false;
			}
		}
	}

	//全判定をクリアしたので当たり判定をする
	return true;
}

bool CollisionManager::IsCollision(const std::weak_ptr<Collider> _col1, const std::weak_ptr<Collider> _col2)
{
	//1つ目の当たり判定形状
	auto& geo1 = _col1.lock()->GetGeometry();
	
	//2つ目の当たり判定形状
	auto& geo2 = _col2.lock()->GetGeometry();

	//当たり判定
	bool ret = geo1.IsHit(geo2);

	//当たっていないなら
	if (!ret)
	{
		//当たっていないことを返す
		_col1.lock()->NotHit();
		_col2.lock()->NotHit();
	}
	
	//当たり判定
	return ret;
}

Collider::TAG CollisionManager::GetTopTags(const std::weak_ptr<Collider> _col)
{
	const auto& tags = _col.lock()->GetTags();
	return *tags.begin();
}
