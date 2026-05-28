#pragma once

#include "../../Base/CardActionBase.h"

class PlayerCardAttackThree:
	public CardActionBase
{

public:
	
    /// @brief コンストラクタ
    /// @param _actCntl アクションコントローラー
    /// @param _charaObj キャラクター
    /// @param _deck カードプレゼンター
    PlayerCardAttackThree(ActionController& _actCntl, CharacterBase& _charaObj, CardPresenter& _cardPresenter);

    /// @brief デストラクタ
    /// @param  
    ~PlayerCardAttackThree(void)override;

    /// @brief ロード
    /// @param  
    void Load(void) override;

    /// @brief 初期化
    /// @param  
    void Init(void) override;

    /// @brief 更新
    /// @param  
    void Update(void) override;

    /// @brief アニメーション情報のロード
    /// @param jsonData アクションロードデータ
    void LoadAnimVar(const ACTION_LOAD_DATA& _data) override;

private:

    //攻撃3段目判定
    static constexpr float ATTACK_ANIM_SPD = 10.0f;               //攻撃アニメーション3段階目の溜め時のアニメ速度
    static constexpr float ATTACK_LERP_TIME = 0.4f;          //攻撃3段階目アニメスピード補完時間

    //攻撃3段階目判定終了カウント
    float atkThreeEndCnt_;

    //攻撃3段階目のアニメーション速度補完
    float atkAnimLerpCnt_;
};

