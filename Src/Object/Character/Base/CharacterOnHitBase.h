#pragma once
#include"Object/ObjectBase.h"

class ActionController;
class CharacterBase;

class CharacterOnHitBase
{

public:

	struct HIT_POINT
	{
		bool isDown = false;
		bool isOverHead = false;
		bool isSide = false;
	};

	CharacterOnHitBase(CharacterBase& _chara, VECTOR& _movedPos,VECTOR& _moveDiff,
		ActionController& _action, std::map<ObjectBase::TAG_PRIORITY, std::shared_ptr<Collider>>& _colParam, Transform& _trans);
	virtual ~CharacterOnHitBase(void);

	/// @brief  初期化
	/// @param  
	virtual void Init(void) = 0;

	/// @brief 当たった時のSEなどの読み込み
	/// @param  
	virtual void Load(void) = 0;

	/// @brief 当たった処理の更新
	/// @param _hitCol 
	void OnHitUpdate(const std::weak_ptr<Collider> _hitCol);

	/// @brief 当たった箇所の取得
	/// @param  
	/// @return 
	const HIT_POINT& GetHitPoint(void)const { return hitPoint_; }

	/// @brief 当たり判定をする前の初期化
	/// @param  
	void InitHit(void);

	/// @brief ターゲットと当たったかどうかの取得
	/// @param  
	/// @return ターゲットと当たったかどうか
	const bool GetIsHitTarget(void)const { return isHitTarget_; }

#ifdef _DEBUG
	/// @brief デバッグ表示
/// @param  
	virtual void DrawDebug(void) = 0;
#endif // _DEBUG

protected:

#pragma region メンバー定数
	//当たり判定の押し出し回数
	static constexpr int COL_TRY_CNT_MAX = 10;

	//ヒットした法線方向へのオフセット
	static constexpr float HIT_NORMAL_OFFSET = 3.0f;

	//押し出す移動量
	static constexpr float POSITION_OFFSET = 0.1f;
#pragma endregion

#pragma region メンバー変数
	ActionController& actionCtrl_;	//アクションコントローラー
	CharacterBase& character_;		//キャラクターの情報
	Transform& trans_;	//プレイヤーの情報
	VECTOR& movedPos_;	//移動後座標
	VECTOR& moveDiff_;	//移動前と移動後の差
	bool isHitTarget_;	//相手の身体を当たったかどうか

	//当たり判定ごとの更新
	std::map<Collider::TAG, std::function<void(const std::weak_ptr<Collider> _hitCol)>>colUpdates_;
	//当たり判定関係
	std::map<ObjectBase::TAG_PRIORITY, std::shared_ptr<Collider>>& colParam_;
	Collider::TAG tag_;		//プレイヤーの当たり判定タグ
	HIT_POINT hitPoint_;	//当たった箇所
#pragma endregion

#pragma region メンバー関数
	//モデルの当たった時の共通処理
	void HitModelCommon(const std::weak_ptr<Collider> _hitCol);

	//衝突後の処理
	void CollNone(void);												//当たっても何もしない(プレイヤー側で何も起きない)
	void CollStage(const std::weak_ptr<Collider> _hitCol);				//ステージ
	virtual void CollChara(const std::weak_ptr<Collider> _hitCol) = 0;	//キャラクター同士
	void CollFire(const std::weak_ptr<Collider>_hitCol);				//炎
#pragma endregion
};