#pragma once

#include "./PlayerCardAttackBase.h"

class ItemBase;

class PlayerCardMagicBase :
    public PlayerCardAttackBase
{
public:

    /// @brief コンストラクタ
     /// @param _actCntl アクションコントローラ
     /// @param _charaObj キャラクターオブジェクト
     /// @param _deck デッキ
    PlayerCardMagicBase(ActionController& _actCntl, CharacterBase& _charaObj, CardPresenter& _deck);

    /// @brief デストラクタ
    /// @param  
    ~PlayerCardMagicBase(void)override;

    /// @brief ロード
    /// @param  
    void Load(void) override = 0;

    /// @brief 初期化
    /// @param  
    virtual void InitAttack(void) override = 0;

    /// @brief 更新
    /// @param  
    void AttackUpdate(void) override;

    /// @brief 解放
    /// @param
    virtual void AttackRelease(void) override = 0;

    ///// @brief アクションデータのロード
    ///// @param animVar 
    //void LoadAnimVar(const ACTION_LOAD_DATA& _data) override;

protected:

    enum class STATE
    {
        SPELL_CAST,
        ATTACK
    };

#pragma region 外部ファイル読み込み
#pragma endregion

#pragma region メンバー変数
    std::shared_ptr<ItemBase>fireBall_;    //ファイアボール

    std::unordered_map<STATE, std::function<void(void)>>changeState_;   //遷移系
    std::function<void(void)>updateState_;                              //更新系
#pragma endregion

#pragma region メンバー関数
    //更新
    virtual void UpdateSpellCast(void) = 0;     //詠唱中
    virtual void UpdateMagicAttack(void) = 0;        //攻撃

    //遷移
    virtual void ChangeSpellCast(void) = 0;     //詠唱中
    virtual void ChangeMagicAttack(void) = 0;        //攻撃
#pragma endregion
};

