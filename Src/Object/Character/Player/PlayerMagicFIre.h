#pragma once
#include "../../ObjectBase.h"
class PlayerMagicFire :
    public ObjectBase
{

public:

    /// @brief コンストラクタ
    /// @param  
    PlayerMagicFire(VECTOR& _dir);

    /// @brief デストラクタ
    /// @param  
    ~PlayerMagicFire(void);

    /// @brief ロード
    /// @param  
    void Load(void) override;

    /// @brief 初期化
    /// @param  
    void Init(void) override;

	/// @brief 更新
	/// @param  
	void Update(void) override;

	/// @brief 描画
	/// @param  
	void Draw(void) override;
    
	/// @brief 当たり判定
	/// @param _hitCol 当たったコライダー
	void OnHit(const std::weak_ptr<Collider> _hitCol) override;

	/// @brief jsonからのロード
	/// @param _jsonData 
	void LoadJsonData(const auto& _jsonData);

private:

	//移動速度
	float moveSpd_;

	//攻撃力
	float atkPow_;

	//当たり判定の半径
	float colRadius_;

	//方向
	VECTOR& dir_;

};

