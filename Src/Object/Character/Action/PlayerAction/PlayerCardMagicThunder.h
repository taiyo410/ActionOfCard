#pragma once

#include "PlayerCardAttackBase.h"

class PlayerMagicThunder;

class PlayerCardMagicThunder :
    public PlayerCardAttackBase
{
public:

    /// @brief コンストラクタ
     /// @param _actCntl アクションコントローラ
     /// @param _charaObj キャラクターオブジェクト
     /// @param _deck デッキ
    PlayerCardMagicThunder(ActionController& _actCntl, CharacterBase& _charaObj, CardPresenter& _deck);

    /// @brief デストラクタ
    /// @param  
    ~PlayerCardMagicThunder(void)override;

    /// @brief ロード
    /// @param  
    void Load(void) override;

    /// @brief 初期化
    /// @param  
    void InitAttack(void) override;

    /// @brief 更新
    /// @param  
    void AttackUpdate(void) override;

    /// @brief 解放
    /// @param
    void AttackRelease(void) override;

    /// @brief アクションデータのロード
    /// @param animVar 
    void LoadAnimVar(const ACTION_LOAD_DATA& _data) override;

private:

#pragma region 外部ファイル読み込み

#pragma endregion

#pragma region メンバー変数
    std::shared_ptr<PlayerMagicThunder>thunder_;    //サンダー
    VECTOR targetPos_;         //敵の座標
#pragma endregion

#pragma region メンバー関数

#pragma endregion



};

