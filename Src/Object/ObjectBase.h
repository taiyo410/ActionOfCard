#pragma once
#include <memory>
#include <map>
#include <vector>
#include <nlohmann/json.hpp>
#include "Common/Transform.h"
#include "Common/IntVector3.h"
#include "./Common/Collider.h"

class ResourceManager;
class SceneManager;
class Geometry;

class ObjectBase
{

public:

	//タグ優先順位
	enum class  TAG_PRIORITY
	{
		STAGE,
		WALL_BACK,
		WALL_LEFT,
		WALL_RIGHT,
		WALL_FRONT,
		CAMERA_2_PLAYER_LINE,
		BODY,
		MOVE_LINE,
		UPDOWN_LINE,
		ATK_SPHERE,
		ROCK_SPHERE,
		FIRE_SPHERE,
		THUNDER_LINE,
	};

	/// @brief コンストラクタ
	/// @param  
	ObjectBase(void);

	/// @brief デストラクタ
	/// @param  
	virtual ~ObjectBase(void);

	/// @brief ロード
	/// @param  
	virtual void Load(void) {}

	/// @brief 初期化
	/// @param  
	virtual void Init(void) = 0;

	/// @brief 更新
	/// @param  
	virtual void Update(void) = 0;

	/// @brief 描画
	/// @param  
	virtual void Draw(void) {}
	
	/// @brief 基本情報の取得
	/// @param  
	/// @return 
	inline const Transform& GetTransform(void) const { return trans_; }
	
	/// @brief 当たったときの処理
	/// @param _hitCol ヒットしたコライダ
	virtual void OnHit(const std::weak_ptr<Collider> _hitCol);

	/// @brief 特定のオブジェクトがあるかどうかを判定する
	/// @param _chataTag 自身のタグ
	/// @param _tag 作りたいタグ
	/// @return true:特定のオブジェクトが存在する	false存在しない
	const bool IsAliveCollider(const Collider::TAG _chataTag, const Collider::TAG _tag);

	/// @brief オブジェクト名の取得
	/// @param  
	/// @return オブジェクト名
	const std::string& GetObjectName(void) { return objectName_; }

protected:

	// シングルトン参照
	ResourceManager& resMng_;		//リソース
	SceneManager& scnMng_;			//シーン

	// モデル制御の基本情報
	Transform trans_;

	//当たり判定関係
	std::map<TAG_PRIORITY, std::shared_ptr<Collider>> collider_;	//全体の当たり判定情報

	//タグ
	Collider::TAG tag_;

	//オブジェクトの名前
	std::string objectName_;

	//当たり判定をしないタグ設定
	std::set<Collider::TAG> noneHitTag_;

	//初めのタグ順格納
	std::vector<TAG_PRIORITY>tagPrioritys_;

	//タグと文字列の対応表
	std::unordered_map<std::string, TAG_PRIORITY>tagStrTable_;
	
	/// @brief 当たり判定作成(形状情報作成後)
	/// @param _tag 自身の当たり判定タグ
	/// @param _geometry 自身の形状情報
	/// @param _notHitTags 衝突させないタグ
	void MakeCollider(const TAG_PRIORITY _tagPriority,const std::set<Collider::TAG> _tag, std::unique_ptr<Geometry> _geometry, const std::set<Collider::TAG> _notHitTags = {});

	//Json読み込みで当たり判定を生成
	void MakeColliderFromJsonData(void);

	/// @brief オブジェクト名を指定して当たり判定を生成
	/// @param _tagName 
	void MakeColliderFromJsonData(const std::string _tagName);

	/// @brief 特定の配列番号の当たり判定の削除
	/// @param _priority タグの種類
	void DeleteCollider(const TAG_PRIORITY _priority);

	//全当たり判定の削除
	void DeleteAllCollider(void);

	//オブジェクト毎に外部データをロード
	virtual void LoadObjectData(void);


private:
	//当たり判定形状の文字列と生成する関数の対応表
	std::unordered_map<std::string, std::function<void(const std::string& _priStr,const nlohmann::json& _data)>>makeCollisionTable_;

	//タグと文字列の対応表
	std::unordered_map<std::string, Collider::TAG>colTagStrTable_;

	//タグ優先度用のタグと文字列対応表
	std::unordered_map<std::string, TAG_PRIORITY>colTagPriorityStrTable_;

	//当たり判定の生成
	void MakeLineCollision(const std::string& _priStr, const nlohmann::json& _data);		//線分
	void MakeCapsuleCollision(const std::string& _priStr, const nlohmann::json& _data);		//カプセル
	void MakeCubeCollision(const std::string& _priStr, const nlohmann::json& _data);		//キューブ
	void MakeSphereCollision(const std::string& _priStr, const nlohmann::json& _data);		//球体
	void MakeModelCollision(const std::string& _priStr, const nlohmann::json& _data);		//モデル
	void MakePlaneCollision(const std::string& _priStr, const nlohmann::json& _data);		//プレーン

	//当たらないタグの取得
	const std::set<Collider::TAG> GetNoneHitTag(const nlohmann::json& _data);

	//自分のタグの取得
	const std::set<Collider::TAG> GetMyTags(const nlohmann::json& _data);
	
};