#pragma once
#include "../../Base/CardActionBase.h"
class PlayerCardReload :
    public CardActionBase
{

public:

    /// @brief コンストラクタ
    /// @param _actCntl アクションコントローラー
    /// @param _charaObj キャラクター
    /// @param _deck カードプレゼンター
    PlayerCardReload(ActionController& _actCntl, CharacterBase& _charaObj, CardPresenter& _cardPresenter);

    /// @brief デストラクタ
    /// @param  
    ~PlayerCardReload(void)override;

    /// @brief ロード
    /// @param  
    void Load(void) override;

    /// @brief 初期化
    /// @param  
    void Init(void) override;

    /// @brief 更新
    /// @param  
    void Update(void) override;

	void Release(void) override;

    /// @brief アニメーション情報のロード
    /// @param jsonData アクションロードデータ
    void LoadAnimVar(const ACTION_LOAD_DATA& _data) override;

public:

    //リロードのボタン押す時間
    static constexpr float RELOAD_TIME = 3.0f;

    //エフェクトの大きさ
    static constexpr float RELOD_EFF_SCL = 100.0f;

    //アニメーション
    static constexpr float RELOAD_LOOP_START = 26.0f;                   //アニメーションループスタート
    static constexpr float RELOAD_LOOP_END = 30.0f;                     //アニメーションループ終了

    //リロードカウント
    float pushReloadCnt_;

    //リロードカウントのセット
    void SetUIReloadCnt(void);
};

