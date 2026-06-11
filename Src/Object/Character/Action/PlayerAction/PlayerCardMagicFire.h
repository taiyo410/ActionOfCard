#pragma once

#include <unordered_map>
#include "./PlayerCardMagicBase.h"

class PlayerMagicFire;

class PlayerCardMagicFire :
    public PlayerCardMagicBase
{

public:

    /// @brief コンストラクタ
     /// @param _actCntl アクションコントローラ
     /// @param _charaObj キャラクターオブジェクト
     /// @param _deck デッキ
    PlayerCardMagicFire(ActionController& _actCntl, CharacterBase& _charaObj, CardPresenter& _deck);

    /// @brief デストラクタ
    /// @param  
    ~PlayerCardMagicFire(void)override;

    /// @brief ロード
    /// @param  
    void Load(void) override;

    /// @brief 初期化
    /// @param  
    void InitAttack(void) override;

    /// @brief 更新
    /// @param  
    //void AttackUpdate(void) override;

    /// @brief 解放
    /// @param
    void AttackRelease(void) override;

	/// @brief アクションデータのロード
	/// @param animVar 
	void LoadAnimVar(const ACTION_LOAD_DATA& _data) override;

private:

    enum class STATE
    {
        SPELL_CAST,
        ATTACK
    };

#pragma region 外部ファイル読み込み
    float actMaxTime_;          //炎のアクション最大時間(一定時間何も起こらなかった場合のアクション時間)
#pragma endregion

#pragma region メンバー変数
    std::shared_ptr<PlayerMagicFire>fireBall_;    //ファイアボール

    std::unordered_map<STATE, std::function<void(void)>>changeState_;   //遷移系
    std::function<void(void)>updateState_;                              //更新系

    VECTOR fireLocalPos_;           //炎のローカル座標
    VECTOR toTargetDir_;            //ターゲットへの方向
    float actCnt_;                  //アクション時間カウント
#pragma endregion

#pragma region メンバー関数
    //更新
    void UpdateSpellCast(void) override;     //詠唱中
    void UpdateMagicAttack(void) override;        //攻撃

    //遷移
    void ChangeSpellCast(void) override;     //詠唱中
    void ChangeMagicAttack(void) override;        //攻撃
#pragma endregion
};

