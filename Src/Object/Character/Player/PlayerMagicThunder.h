#pragma once

#include "../Base/ItemBase.h"

class PlayerMagicThunder :
    public ItemBase
{

public:

    /// @brief コンストラクタ
    /// @param _targetPos 相手の座標
    PlayerMagicThunder(VECTOR& _targetPos);

    /// @brief デストラクタ
    /// @param  
    PlayerMagicThunder(void);

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
    void Draw(void) override;

    /// @brief 当たり判定
    /// @param _hitCol 当たったコライダー
    void OnHit(const std::weak_ptr<Collider> _hitCol) override;

    /// @brief Json読み込み
    /// @param _jsonData 
    void LoadThunderData(const nlohmann::json _jsonData);

    /// @brief 炎の当たり判定を作成
    /// @param  
    void MakeThunderCollider(void);

private:

#pragma region メンバー定数
    const std::string THUNDER_STR = "Thunder";
#pragma endregion


#pragma region 外部ファイル読み込み

#pragma endregion

#pragma region メンバー変数
    float thunderEffScl_;       //サンダーエフェクトの大きさ
    int thunderEffPlayId_;      //サンダーエフェクトのプレイID
#pragma endregion

#pragma region メンバー関数
    VECTOR& targetPos_;     //使用主の相手の座標
    VECTOR topPos_;         //上座標
    VECTOR downPos_;        //下座標
#pragma endregion


};

