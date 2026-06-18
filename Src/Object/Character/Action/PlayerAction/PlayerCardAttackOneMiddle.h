#pragma once

#include "./PlayerCardAttackBase.h"

class PlayerCardAttackOneMiddle :
    public PlayerCardAttackBase
{
public:

    /// @brief コンストラクタ
    /// @param _actCntl アクションコントローラー
    /// @param _charaObj キャラクター
    /// @param _deck カードプレゼンター
    PlayerCardAttackOneMiddle(ActionController& _actCntl, CharacterBase& _charaObj, CardPresenter& _cardPresenter);

    /// @brief デストラクタ
    /// @param  
    ~PlayerCardAttackOneMiddle(void)override;

    /// @brief 初期化
    /// @param  
    void InitAttack(void) override;

    /// @brief 更新
    /// @param  
    void AttackUpdate(void) override;

    /// @brief 遷移時の解放
    /// @param  
    void AttackRelease(void)override;

    /// @brief アニメーション情報のロード
    /// @param jsonData アクションロードデータ
    void LoadAnimVar(const ACTION_LOAD_DATA& _data) override;

private:

#pragma region 外部ファイル読み込み
    float attackTime_ = 0.0f;                      //中距離移動時間
    float initVelocity_ = 0.0f;                    //中距離初期速度
    float comboTIme_ = 0.0f;                       //中距離移動コンボ受付時間
#pragma endregion

#pragma region メンバー変数
    std::unique_ptr<Easing>easing_;         //イージング
    float midAtkCnt_;                       //中距離攻撃突きカウント
    float midAtkOverCnt_;                   //中距離攻撃突きオーバーカウント
#pragma endregion
};

