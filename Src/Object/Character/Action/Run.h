#pragma once
#include "../Manager/Resource/ResourceManager.h"
#include "../Base/ActionBase.h"

class Run :
    public ActionBase
{

public:

    /// @brief コンストラクタ
    /// @param _actCntl アクションコントローラー
    /// @param _speed 速度
    /// @param _src 足音
    /// @param _footSeDis 足音の間隔 
    Run(ActionController& _actCntl, CharacterBase& _character);

    /// @brief デストラクタ
    /// @param  
    ~Run(void)override;

	/// @brief ロード
	/// @param  
	void Load(void) override;

	/// @brief 初期化
	/// @param  
	void Init(void) override;

    /// @brief 更新
    /// @param  
    void Update(void) override;

    /// @brief アニメーション情報のロード
    /// @param jsonData アクションロードデータ
    void LoadActionParameter(const ACTION_LOAD_DATA& _data) override;

public:

#pragma region 外部ファイル読み込み
    float  footSeDis_;    //足音再生間隔
#pragma endregion

#pragma region メンバー変数
    float footSECnt_;                   //足音間隔
    ResourceManager::SRC footSESrc_;    //足音音源
#pragma endregion

};

