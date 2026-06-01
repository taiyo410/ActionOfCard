#pragma once

#include "./PlayerCardAttackBase.h"

class PlayerCardAttackTwo :
    public PlayerCardAttackBase
{

public:

    /// @brief コンストラクタ
    /// @param _actCntl アクションコントローラー
    /// @param _charaObj キャラクター
    /// @param _deck カードプレゼンター
    PlayerCardAttackTwo(ActionController& _actCntl, CharacterBase& _charaObj, CardPresenter& _cardPresenter);

    /// @brief デストラクタ
    /// @param  
    ~PlayerCardAttackTwo(void)override;

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

};

