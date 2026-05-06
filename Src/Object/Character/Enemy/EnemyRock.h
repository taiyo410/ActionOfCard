#pragma once
#include "../Object/ObjectBase.h"

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

private:

    //ジャンプ力
    static constexpr float POW_JUMP = 100.0f;

    //重力
    static constexpr float GRAVITY = 1.3f;

    //ジャンプ高さ
    static constexpr float JUMP_HEIGHT = 300.0f;

    //キャラ同士の間隔
    static constexpr float DISTANCE_RADIUS = 100.0f;

    //岩の角度関連
    static constexpr int ARROUND_NUM = 8;			//1周で出す岩の数	
    static constexpr float ARROUND_PER_DEG = 360.0f / ARROUND_NUM;	//岩と岩の間隔
    static constexpr float ARROUND_PER_RAD = ARROUND_PER_DEG * DX_PI_F / 180.0f;//ラジアン変換

    //当たり判定球の半径
	static constexpr float ROCK_COL_RADIUS = 150.0f;

    //水平方面への移動速度
    static constexpr float MOVE_HORIZONTAL_SPD = 20.0f;

	//番号
	int num_;

    //センター座標
    VECTOR centerPos_;

    //初期座標
    VECTOR& startPos_;

    //岩を放物線で飛ばすためのY
    VECTOR jumpPow_;

    //移動量
    VECTOR velocity_;

    //飛距離
    float distance_;

    //目標の座標
    VECTOR goalPos_;

	//モデルID
	int modelId_;

    //生存(攻撃時にセット)
    bool isAlive_;

	//ダメージ処理済みフラグ
	bool isDamaged_;

    //攻撃力
    float atkPow_;

    //岩の攻撃力を外部から取得
    void LoadAttackPow(void);
};