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
    Run(ActionController& _actCntl,const float _speed, ResourceManager::SRC _src,const float _footSeDis);

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

public:

    //移動速度
    const float moveSpd_;

    //足音間隔
	float footSECnt_;

	//足音音源
	ResourceManager::SRC footSESrc_;

    //足音再生間隔
	float  footSeDis_;
};

