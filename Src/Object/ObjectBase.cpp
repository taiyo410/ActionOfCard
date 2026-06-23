#include "Utility/UtilityJson.h"
#include "Utility/Utility3D.h"
#include "Manager/Resource/ResourceManager.h"
#include "Manager/Generic/SceneManager.h"
#include "Manager/Game/CollisionManager.h"
#include "./Common/Geometry/Capsule.h"
#include "./Common/Geometry/Cube.h"
#include "./Common/Geometry/Model.h"
#include "./Common/Geometry/Sphere.h"
#include "./Common/Geometry/Plane.h"
#include "./Common/Geometry/Line.h"
#include "ObjectBase.h"

ObjectBase::ObjectBase(void)
	: resMng_(ResourceManager::GetInstance()),
	scnMng_(SceneManager::GetInstance()),
	tag_(Collider::TAG::NONE),
	trans_()
{
	makeCollisionTable_ = {
		{"capsule",[this](const std::string& _priStr,const nlohmann::json& _data) {MakeCapsuleCollision(_priStr,_data); }},
		{"line",[this](const std::string& _priStr,const nlohmann::json& _data) {MakeLineCollision(_priStr,_data); }},
		{"cube",[this](const std::string& _priStr,const nlohmann::json& _data) {MakeCubeCollision(_priStr,_data); }},
		{"sphere",[this](const std::string& _priStr,const nlohmann::json& _data) {MakeSphereCollision(_priStr,_data); }},
		{"model",[this](const std::string& _priStr,const nlohmann::json& _data) {MakeModelCollision(_priStr,_data); }},
		{"plane",[this](const std::string& _priStr,const nlohmann::json& _data) {MakePlaneCollision(_priStr,_data); }}
	};

	colTagPriorityStrTable_ = {
		{"Stage",TAG_PRIORITY::STAGE},
		{"WallLeft",TAG_PRIORITY::WALL_LEFT},
		{"WallRight",TAG_PRIORITY::WALL_RIGHT},
		{"WallBack",TAG_PRIORITY::WALL_BACK},
		{"WallFront",TAG_PRIORITY::WALL_FRONT},
		{"Body",TAG_PRIORITY::BODY},
		{"MoveLine",TAG_PRIORITY::MOVE_LINE},
		{"UpdownLine",TAG_PRIORITY::UPDOWN_LINE},
		{"AttackSphere",TAG_PRIORITY::ATK_SPHERE},
		{"RockSphere",TAG_PRIORITY::ROCK_SPHERE},
		{"FireSphere",TAG_PRIORITY::FIRE_SPHERE},
		{"ThunderLine",TAG_PRIORITY::THUNDER_LINE}
	};

	colTagStrTable_ = {
		{ "player", Collider::TAG::PLAYER1 },
		{ "enemy",Collider::TAG::ENEMY1 },
		{ "normalAttack",Collider::TAG::NML_ATK },
		{ "jumpAttack",Collider::TAG::JUMP_ATK },
		{ "stage",Collider::TAG::STAGE },
		{ "wall",Collider::TAG::WALL },
		{ "stompRock",Collider::TAG::ROCK },
		{ "fire",Collider::TAG::FIRE },
		{ "camera",Collider::TAG::CAMERA }
	};
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
	for (auto& col : collider_)
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

void ObjectBase::LoadObjectData(void)
{

}

void ObjectBase::MakeColliderFromJsonData(void)
{
	const auto& colData = resMng_.Load(ResourceManager::SRC::COLLISION_DATA).jsonData;

	//取得したいオブジェクト名がなければ処理を飛ばす
	if (!colData.contains(objectName_))return;
	for (const auto& [key, data] : colData[objectName_].items())
	{
		const std::string& geoStr = data.value("geometry", "");
		makeCollisionTable_[geoStr](key,data);
	}
}

void ObjectBase::MakeColliderFromJsonData(const std::string _tagName)
{
	const auto& colData = resMng_.Load(ResourceManager::SRC::COLLISION_DATA).jsonData;

	//取得したいオブジェクト名がなければ処理を飛ばす
	if (!colData.contains(_tagName))return;
	for (const auto& [key, data] : colData[objectName_].items())
	{
		const std::string& geoStr = data.value("geometry", "");
		makeCollisionTable_[geoStr](key, data);
	}
}

void ObjectBase::MakeLineCollision(const std::string& _priStr, const nlohmann::json& _data)
{
	//当たらないタグの取得
	std::set<Collider::TAG> noneHitTag = GetNoneHitTag(_data);

	//複数のタグを持つ場合の自分のタグの取得
	std::set<Collider::TAG>myTags = GetMyTags(_data);

	//優先順位用タグの取得
	auto it = colTagPriorityStrTable_.find(_priStr);

	//ラインパラメーター取得
	const VECTOR& localPos1 = UtilityJson::GetLoadVector3("localPos1",_data);
	const VECTOR& localPos2 = UtilityJson::GetLoadVector3("localPos2",_data);

	//コライダーの生成
	std::unique_ptr<Geometry>geo = std::make_unique<Line>(trans_.pos, trans_.quaRot, localPos1, localPos2);
	MakeCollider(it->second, myTags, std::move(geo), noneHitTag);
	tagPrioritys_.emplace_back(it->second);
}


void ObjectBase::MakeCapsuleCollision(const std::string& _priStr, const nlohmann::json& _data)
{
	//当たらないタグの取得
	std::set<Collider::TAG> noneHitTag = GetNoneHitTag(_data);

	//複数のタグを持つ場合の自分のタグの取得
	std::set<Collider::TAG>myTags = GetMyTags(_data);

	//優先順位用タグの取得
	auto it = colTagPriorityStrTable_.find(_priStr);

	//カプセルのパラメーター取得
	const VECTOR localTop = UtilityJson::GetLoadVector3("localPos1",_data);
	const VECTOR localDown = UtilityJson::GetLoadVector3("localPos2",_data);
	const float radius = _data.value("radius", 0.0f);

	std::unique_ptr<Geometry>geo = std::make_unique<Capsule>(trans_.pos, trans_.quaRot, localTop, localDown, radius);
	MakeCollider(it->second, myTags, std::move(geo), noneHitTag);
	tagPrioritys_.emplace_back(it->second);
}

void ObjectBase::MakeCubeCollision(const std::string& _priStr, const nlohmann::json& _data)
{
	//当たらないタグの取得
	std::set<Collider::TAG> noneHitTag = GetNoneHitTag(_data);

	//複数のタグを持つ場合の自分のタグの取得
	std::set<Collider::TAG>myTags = GetMyTags(_data);

	//優先順位用タグの取得
	auto it = colTagPriorityStrTable_.find(_priStr);

	//キューブのパラメーター取得
	const VECTOR min = UtilityJson::GetLoadVector3("min", _data);
	const VECTOR max = UtilityJson::GetLoadVector3("max", _data);
	
	//ハーフサイズ指定用のパラメータ
	const VECTOR halfSize = UtilityJson::GetLoadVector3("halfSize", _data);

	//コライダーの生成
	std::unique_ptr<Geometry>geo = std::make_unique<Cube>(trans_.pos, trans_.quaRot, min,max);
	if (!Utility3D::EqualsVZero(halfSize))
	{
		geo = std::make_unique<Cube>(trans_.pos, trans_.quaRot, halfSize);
	}
	

	//最大頂点と最小頂点が指定されていた場合の生成
	if (Utility3D::EqualsVZero(min) || Utility3D::EqualsVZero(max))
	{
		geo= std::make_unique<Cube>(trans_.pos, trans_.quaRot, min,max);
	}
	MakeCollider(it->second, myTags, std::move(geo), noneHitTag);
	tagPrioritys_.emplace_back(it->second);
}

void ObjectBase::MakeSphereCollision(const std::string& _priStr, const nlohmann::json& _data)
{
	//当たらないタグの取得
	std::set<Collider::TAG> noneHitTag = GetNoneHitTag(_data);

	//複数のタグを持つ場合の自分のタグの取得
	std::set<Collider::TAG>myTags = GetMyTags(_data);

	//優先順位用タグの取得
	auto it = colTagPriorityStrTable_.find(_priStr);

	//球のパラメーター取得
	const float radius = _data.value("radius", 0.0f);

	//コライダーの生成
	std::unique_ptr<Geometry>geo = std::make_unique<Sphere>(trans_.pos, radius);
	MakeCollider(it->second, myTags, std::move(geo), noneHitTag);
	tagPrioritys_.emplace_back(it->second);
}

void ObjectBase::MakeModelCollision(const std::string& _priStr, const nlohmann::json& _data)
{
	//当たらないタグの取得
	std::set<Collider::TAG> noneHitTag = GetNoneHitTag(_data);

	//複数のタグを持つ場合の自分のタグの取得
	std::set<Collider::TAG>myTags = GetMyTags(_data);

	//優先順位用タグの取得
	auto it = colTagPriorityStrTable_.find(_priStr);

	//球のパラメーター取得
	const std::string model = _data.value("model", "");
	const ResourceManager::SRC useSrc = resMng_.GetSrcFromString(model);
	const int modelId = useSrc == ResourceManager::SRC::NONE ? trans_.modelId : resMng_.LoadModelDuplicate(useSrc);

	//コライダーの生成
	std::unique_ptr<Geometry>geo = std::make_unique<Model>(trans_.pos,trans_.quaRot,modelId);
	MakeCollider(it->second, myTags, std::move(geo), noneHitTag);
	tagPrioritys_.emplace_back(it->second);
}

void ObjectBase::MakePlaneCollision(const std::string& _priStr, const nlohmann::json& _data)
{
	//当たらないタグの取得
	std::set<Collider::TAG> noneHitTag = GetNoneHitTag(_data);

	//複数のタグを持つ場合の自分のタグの取得
	std::set<Collider::TAG>myTags = GetMyTags(_data);

	//優先順位用タグの取得
	auto it = colTagPriorityStrTable_.find(_priStr);

	//プレーンのパラメーター取得
	const VECTOR centerLocalPos = UtilityJson::GetLoadVector3("localCenterPos", _data);
	VECTOR localRotDeg = UtilityJson::GetLoadVector3("localRotDeg", _data);
	localRotDeg = Utility3D::Deg2Rad3D(localRotDeg);
	const VECTOR min = UtilityJson::GetLoadVector3("min", _data);
	const VECTOR max = UtilityJson::GetLoadVector3("max", _data);

	//コライダーの生成
	std::unique_ptr<Geometry>geo = std::make_unique<Plane>(trans_.pos, trans_.quaRot, centerLocalPos, localRotDeg, min, max);
	MakeCollider(it->second, myTags, std::move(geo), noneHitTag);
	tagPrioritys_.emplace_back(it->second);
}

const std::set<Collider::TAG> ObjectBase::GetNoneHitTag(const nlohmann::json& _data)
{
	std::set<Collider::TAG> ret = {};
	if (_data.contains("noneHitTag"))
	{
		for (const auto& noneHit : _data["noneHitTag"])
		{
			const std::string& tagStr = noneHit.get<std::string>();
			auto it = colTagStrTable_.find(tagStr);

			//登録されていれば配列に追加
			if (it != colTagStrTable_.end())ret.emplace(it->second);
		}
	}
	return ret;
}

const std::set<Collider::TAG> ObjectBase::GetMyTags(const nlohmann::json& _data)
{
	std::set<Collider::TAG> ret = {};
	if (_data.contains("myTag"))
	{
		for (const auto& noneHit : _data["myTag"])
		{
			const std::string& tagStr = noneHit.get<std::string>();
			auto it = colTagStrTable_.find(tagStr);

			//登録されていれば配列に追加
			if (it != colTagStrTable_.end())ret.emplace(it->second);
		}
	}

	//自分のタグを追加
	ret.emplace(tag_);
	return ret;
}

