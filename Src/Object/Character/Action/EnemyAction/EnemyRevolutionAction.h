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

    /// @brief アニメーション情報のロード
    /// @param jsonData アクションロードデータ
    void LoadAnimVar(const ACTION_LOAD_DATA& _data) override;
private:

#pragma region 外部ファイル読み込み
    float 
#pragma endregion

};

