#pragma once
#include "../../Base/CardActionBase.h"
class EnemyCardReload :
    public CardActionBase
{

public:

    /// @brief コンストラクタ
    /// @param _actCntl アクションコントローラー
    /// @param _character キャラクター
    /// @param _cardPresent カードプレゼンター
    EnemyCardReload(ActionController& _actCtrl, CharacterBase& _character, CardPresenter& _cardPresent);

    /// @brief デストラクタ
    /// @param  
    ~EnemyCardReload(void)override;

    /// @brief 初期化
    /// @param  
    void Init(void)override;

    /// @brief 更新
    /// @param  
    void Update(void)override;

    /// @brief 解放
    /// @param  
    void Release(void)override;

private:

};

