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

	/// @brief 解放
	/// @param  
	void Release(void) override;

    /// @brief アニメーション情報のロード
    /// @param jsonData アクションロードデータ
    void LoadAnimVar(const ACTION_LOAD_DATA& _data) override;

public:

#pragma region 外部ファイル読み込み
    float reloadTime_;              //リロードにかかる時間
    float reloadEffScl_;            //リロードエフェクトの大きさ
    float animLoopStartStep_;       //アニメーションループスタートステップ
    float animLoopEndStep_;         //アニメーションループ終了ステップ
#pragma endregion

#pragma region メンバー変数
    float pushReloadCnt_;           //リロードカウント
    int effPlayID_;                 //エフェクトプレイID
    int reloadEndEffPlayID_;        //リロード終了エフェクトプレイID
#pragma endregion

#pragma region メンバー関数
    //リロードカウントのセット
    void SetUIReloadCnt(void);
#pragma endregion
};

