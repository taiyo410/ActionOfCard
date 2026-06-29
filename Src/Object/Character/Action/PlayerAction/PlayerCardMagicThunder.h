#pragma once

#include "./PlayerCardMagicBase.h"

class PlayerMagicThunder;

class PlayerCardMagicThunder :
    public PlayerCardMagicBase
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

    /// @brief 解放
    /// @param
    void AttackRelease(void) override;

    /// @brief アクションデータのロード
    /// @param animVar 
    void LoadActionParameter(const ACTION_LOAD_DATA& _data) override;

private:

#pragma region メンバー変数
    //サンダー
    std::shared_ptr<PlayerMagicThunder>thunder_;    
    VECTOR targetPos_;         //敵の座標
#pragma endregion

#pragma region メンバー関数
    //更新
    void UpdateSpellCast(void) override;     //詠唱中
    void UpdateMagicAttack(void) override;   //攻撃

    //遷移
    void ChangeSpellCast(void) override;     //詠唱中
    void ChangeMagicAttack(void) override;   //攻撃
#pragma endregion



};

