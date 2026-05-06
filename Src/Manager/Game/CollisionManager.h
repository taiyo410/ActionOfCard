#pragma once

#include<vector>
#include<memory>
#include<map>
#include<functional>
#include"../Template/Singleton.h"
#include"../Object/Common/Collider.h"

class CollisionManager :public Singleton<CollisionManager>
{
	// シングルトン
	friend class Singleton<CollisionManager>;

public:

	//当たり判定をする範囲
	static constexpr float HIT_RANGE_NORMAL = 5000.0f;	//通常の当たり判定距離
	static constexpr float HIT_RANGE_CAMERA = 10000.0f;	//通常の当たり判定距離
	static constexpr float HIT_RANGE_ROCK = 1000.0f;	//通常の当たり判定距離
	static constexpr float HIT_RANGE_JUMP_ATK = 5000.0f;
	static constexpr float HIT_RANGE_STAGE = 30000.0f;	//風用の当たり判定距離

	//更新用
	static constexpr int COL_UPDATE_FRAME = 0;		//更新フレーム

	/// @brief コライダの追加
	/// @param _collider 追加したいコライダ
	void AddCollider(const std::shared_ptr<Collider> _collider);

	/// @brief 必要なくなったコライダの削除(更新の最後に置く)
	/// @param  
	void Sweep(void);

	/// @brief 更新
	/// @param  
	void Update(void);
	
	/// @brief そのタグがプレイヤーかどうか調べる
	/// @param _tag 調べるタグ
	/// @return true:プレイヤーだった
	const bool IsPlayer(const Collider::TAG _tag)const;

	/// @brief そのタグがアイテムかどうか調べる
	/// @param _tag 調べるタグ
	/// @return アイテムだった
	const bool IsItem(const Collider::TAG _tag)const;

private:

	//当たり判定格納
	std::vector<std::shared_ptr<Collider>>colliders3D_;

	//当たり判定距離の二乗
	std::map<Collider::TAG, float> hitRange_;

	//当たり判定するフレーム
	int updateFrame_;

	/// @brief //コンストラクタ
	/// @param  
	CollisionManager(void);

	/// @brief //コピーの禁止
	/// @param _copy 
	CollisionManager(const CollisionManager& _copy) = delete;
	CollisionManager& operator= (const CollisionManager& _copy) = delete;

	/// @brief //デストラクタ
	/// @param  
	~CollisionManager(void)override;

	/// @brief 当たり判定距離内にいるか
	/// @param _col1 1つ目のコライダ
	/// @param _col2 2つ目のコライダ
	/// @return true:範囲内
	const bool IsWithInHitRange(const std::weak_ptr<Collider> _col1, const std::weak_ptr<Collider> _col2)const;

	/// @brief 当たり判定をするか(全部当てはまったらtrue)
	/// @param _col1Num 1つ目のコライダ番号
	/// @param _col2Num 2つ目のコライダ番号
	/// @return 当たり判定
	const bool JudgeIsCollision(const int _col1Num, const int _col2Num)const;
	
	/// @brief 当たり判定
	/// @param _col1 1つ目のコライダ
	/// @param _col2 2つ目のコライダ
	/// @return 
	bool IsCollision(const std::weak_ptr<Collider> _col1, const std::weak_ptr<Collider> _col2);

	/// @brief 一番上のタグを取得
	/// @param _col どのコリジョンか
	/// @return 
	Collider::TAG GetTopTags(const std::weak_ptr<Collider> _col);
};