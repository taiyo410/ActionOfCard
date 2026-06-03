#pragma once

#include "./PlayerCardAttackBase.h"

class PlayerCardAttackThree:
	public PlayerCardAttackBase
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

    /// @brief 初期化
    /// @param  
    void InitAttack(void) override;

    /// @brief 更新
    /// @param  
    void AttackUpdate(void) override;

    /// @brief アニメーション情報のロード
    /// @param jsonData アクションロードデータ
    void LoadAnimVar(const ACTION_LOAD_DATA& _data) override;

private:

    //攻撃3段階目判定終了カウント
    float atkThreeEndCnt_;

    //攻撃3段階目のアニメーション速度補完
    float atkAnimLerpCnt_;

    //外部読み込み
	float chargeAnimSpd_;       //攻撃3段階目のアニメーション速度
	float attackLerpTime_;      //アニメスピード保管時間
    float cameraShakeLimit_;    //カメラシェイクの強さ
	float attackEndCnt_;        //攻撃終了後の後隙
};

