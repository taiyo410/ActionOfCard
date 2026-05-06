#pragma once
#include "CardUIBase.h"

class EnemyCardUI :
    public CardUIBase
{

public:

    /// @brief コンストラクタ
    /// @param  
    EnemyCardUI(void);

    /// @brief デストラクタ
    /// @param  
    ~EnemyCardUI(void)override;

    /// @brief ロード
    /// @param  
    void Load(void) override;

    /// @brief 初期化
    /// @param  
    void Init(void) override;

    /// @brief 更新
    /// @param  
    void Update(void) override;

    /// @brief 描画
    /// @param
	void Draw(void) override;

    /// @brief カードUI初期化
    /// @param
    void InitCardUI(void)override;

#ifdef _DEBUG
	void DrawDebug(void);
#endif // _DEBUG

private:

    //弾かれた後のゴール座標(敵)
    static constexpr Vector2F REACT_GOAL_CARD_POS = { Application::SCREEN_SIZE_X + 200.0f, Application::SCREEN_HALF_Y + 500 };

	//カードブレイクサウンド音量
	static constexpr float CARD_BREAK_SOUND_VOLUME = 0.6f;

    //遷移系
    void ChangeNone(void) override;		        //通常
    void ChangeDecision(void) override;	        //決定
    void ChangeReloadWait(void) override;       //リロード待機

    //更新系
    void UpdateNone(void) override;             //何もしない
    void UpdateDecision(void) override;         //決定
	void UpdateReloadWait(void) override;

    //カードUI描画オブジェクト更新
    void UpdateDrawCardUI(void) override;
};