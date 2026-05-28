#pragma once

#include "../../Base/CardActionBase.h"

class PlayerMagicFire;

class PlayerCardMagicFire :
    public CardActionBase
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
    void Init(void) override;

    /// @brief 更新
    /// @param  
    void Update(void) override;

    /// @brief 解放
    /// @param
    void Release(void)override;

	/// @brief アクションデータのロード
	/// @param animVar 
	void LoadAnimVar(const ACTION_LOAD_DATA& _data) override;

private:

    //炎
    //std::unique_ptr<PlayerMagicFire>fireBall_;


};

