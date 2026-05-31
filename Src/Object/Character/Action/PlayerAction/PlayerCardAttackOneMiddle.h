#pragma once
#include "../../Base/CardActionBase.h"
class PlayerCardAttackOneMiddle :
    public CardActionBase
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

    /// @brief ロード
    /// @param  
    void Load(void) override;

    /// @brief 初期化
    /// @param  
    void Init(void) override;

    /// @brief 更新
    /// @param  
    void Update(void) override;

    /// @brief 遷移時の解放
    /// @param  
    void Release(void)override;

    /// @brief アニメーション情報のロード
    /// @param jsonData アクションロードデータ
    void LoadAnimVar(const ACTION_LOAD_DATA& _data) override;

private:

    //中距離攻撃
    static constexpr float ATTACK_ONE_MID_TIME = 1.0f;                  //中距離移動時間
    static constexpr float ATTACK_ONE_MID_SPD = 30.0f;                  //中距離初期速度

    //攻撃1段目判定(中距離)
    static constexpr float ATTACK_ONE_MID_COMBO_TIME = 0.2f;            //コンボ受付時間

    //中距離攻撃突きカウント
    float midAtkCnt_;
    float midAtkOverCnt_;

    //イージング
    std::unique_ptr<Easing>easing_;

};

