#pragma once
#include<memory>
#include<set>
#include"Geometry/Geometry.h"

class ObjectBase;
class CharacterBase;
class Collider
{

public :

	//衝突用タグ
	enum class TAG
	{
		NONE,			//タグなし
		PLAYER1,		//プレイヤー1
		ENEMY1,			//敵1
		NML_ATK,		//剣
		JUMP_ATK,		//ジャンプ
		STAGE,			//ステージ
		ROCK,			//敵の攻撃用岩
		CAMERA,			//カメラ
	};

	/// @brief コンストラクタ
	/// @param _parent 親
	/// @param _tags 自身の衝突用タグ
	/// @param _geometry 当たり判定の形状
	/// @param _notHitTags 衝突させないタグ
	Collider(ObjectBase& _parent, const std::set<TAG> _tags, std::unique_ptr<Geometry>_geometry, const std::set<TAG> _notHitTags);

	/// @brief デストラクタ
	/// @param 
	~Collider(void);

	/// @brief 衝突用タグの取得
	/// @param  
	/// @return 
	inline const std::set<TAG>& GetTags(void)const { return tags_; }


	/// @brief 当たり判定の形状を取得
	/// @param  
	/// @return 
	Geometry& GetGeometry(void)const;

	/// @brief 衝突させないタグの取得
	/// @param  
	/// @return 
	inline const std::set<TAG>& GetNotHitTags(void)const { return notHitTags_; }

	/// @brief 親を取得
	/// @param  
	/// @return 
	ObjectBase& GetParent(void)const { return parent_; }

	/// @brief 子クラスがキャラクターである場合、キャラクターベースを取得する
	/// @param  
	/// @return 
	CharacterBase& GetParentCharacter(void);

	/// @brief 当たったかの判定の取得
	/// @param  
	/// @return 
	inline const bool& IsHit(void)const { return isHit_; }

	/// @brief 当たっていない
	/// @param  
	inline void NotHit(void) { isHit_ = false; }

	/// @brief 終了判定の取得
	/// @param  
	/// @return 
	inline const bool& IsDead(void)const { return isDead_; }

	/// @brief 終了処理(所持者の解放時に置く)
	/// @param  
	inline void Kill(void) { isDead_ = true; }


	/// @brief 当たった時の処理
	/// @param _collider 相手のコライダ
	void OnHit(const std::weak_ptr<Collider> _collider);

private:

	//親がキャラクター出ないときのエラー文
	const std::string NOT_CHARACTER_STR = "親がCharacterBase型ではありません";

	//親
	ObjectBase& parent_;

	// 衝突用タグ
	std::set<TAG> tags_;

	// 衝突しないタグ
	std::set<TAG> notHitTags_;

	//当たり判定の形状
	std::unique_ptr<Geometry> geometry_;

	//優先順位を決めるためのタグ配列
	std::vector<TAG>tagTable_;

	//当たったかの判定
	bool isHit_;

	//終了判定
	bool isDead_;
};
