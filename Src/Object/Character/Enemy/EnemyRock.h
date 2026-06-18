#pragma once

#include "Object/ObjectBase.h"

class EnemyRock :
    public ObjectBase
{

public:

    /// @brief コンストラクタ
    /// @param  
    EnemyRock(int& _num,VECTOR& _startPos);

    /// @brief デストラクタ
    /// @param  
    ~EnemyRock(void)override;

    /// @brief ロード
    /// @param  
    void Load(void)override;

    /// @brief 初期化
    /// @param  
    void Init(void)override;

    /// @brief 更新
    /// @param  
    void Update(void)override;

    /// @brief 描画
    /// @param  
    void Draw(void)override;

    /// @brief 当たった時の処理
    /// @param _hitCol 
    void OnHit(const std::weak_ptr<Collider> _hitCol)override;

    /// @brief 岩がダメージを与えたか
    /// @param  
    /// @return 
    const bool GetIsDamaged(void) { return isDamaged_; }

	/// @brief 生存状態のセット
	/// @param _isAlive 
	void SetIsAlive(const bool _isAlive) { isAlive_ = _isAlive; }

	/// @brief ダメージフラグのセット
	/// @param  
	void SetIsDamaged(void) { isDamaged_ = true; }

	/// @brief 岩の当たり判定削除
    /// @param  
    void DeleteRockCollider(void);

    /// @brief 岩の攻撃力の取得
    /// @param  
    /// @return 岩の攻撃力
    const float GetAttackPow(void) { return atkPow_; }

    /// @brief 外部ファイル読み込み
    /// @param _jsonData 
    void LoadRockData(const nlohmann::json _jsonData);

private:

#pragma region メンバー定数
    //オブジェクト名に使用する文字列
    const std::string ROCK_STR = "StompRock";
#pragma endregion


#pragma region 外部ファイル読み込み
    float atkPow_ = 0.0f;               //攻撃力
    float gravity_ = 0.0f;              //重力
    float jumpHeight_ = 0.0f;           //ジャンプ力
    int rockNum_ = 0;                 //岩の生成数
    float radius_ = 0.0f;               //岩の半径
    float horizontalMoveSpd_ = 0.0f;    //水平方向への移動速度
    float aroundPerRad_ = 0.0f;         //岩と岩の間隔
    float moveDistance_ = 0.0f;         //岩の動く距離
#pragma endregion

#pragma region メンバー変数

    int createNum_;         //何個目に生成されたか
    VECTOR centerPos_;      //センター座標
    VECTOR& startPos_;      //初期座標
    VECTOR jumpPow_;        //岩を放物線で飛ばすためのY
    VECTOR velocity_;       //移動量
    float distance_;        //飛距離
    VECTOR goalPos_;        //目標の座標
    int modelId_;           //モデルID
    bool isAlive_;          //生存(攻撃時にセット)
    bool isDamaged_;        //ダメージ処理済みフラグ
#pragma endregion
};