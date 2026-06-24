#pragma once
#include "Object/Character/Base/ActionBase.h"
class EnemyRevolutionAction :
    public ActionBase
{
public:

    /// @brief コンストラクタ
    /// @param _actCntl アクションコントローラー
    /// @param _character キャラクター
    EnemyRevolutionAction(ActionController& _actCntl, CharacterBase& _character);

    /// @brief デストラクタ
    /// @param  
    ~EnemyRevolutionAction(void);

    /// @brief ロード
    /// @param  
    void Load(void) override;
    /// @brief 初期化
    /// @param
    void Init(void) override;

    ///@brief 更新
    /// @param
    void Update(void) override;

    /// @brief 解放
    /// @param  
    void Release(void) override;

    /// @brief アニメーション情報のロード
    /// @param jsonData アクションロードデータ
    void LoadAnimVar(const ACTION_LOAD_DATA& _data) override;
private:

#pragma region 外部ファイル読み込み
    float revolutionStartAnimStep_ = 0.0f;          //革命開始アニメーションステップ
    float animationLoopStartStep_ = 0.0f;           //ループ開始ステップ
    float animationLoopEndStep_ = 0.0f;             //ループ終了ステップ
    float animationLoopSpeed_ = 0.0f;               //ループ中のアニメーションスピード
    float effectPlaySpeed_;                         //エフェクトの再生速度
    VECTOR effectScale_ = {};                       //エフェクトの大きさ
    float waitTime_;                                //待ち時間
#pragma endregion

#pragma region メンバー変数
    int effectPlayId_;          //プレイ中のエフェクトID  
    float waitCnt_;             //アクションカウント
#pragma endregion
};