#pragma once
#include "../Base/ActionBase.h"

class Idle :
    public ActionBase
{

public:

    /// @brief コンストラクタ
	/// @param _actCntl アクションコントローラー
    Idle(ActionController& _actCntl);

    /// @brief デストラクタ
    /// @param  
    ~Idle(void)override;

	/// @brief ロード
	/// @param  
	void Load(void) override;

    /// @brief 初期化
    /// @param  
    void Init(void) override;

    /// @brief 更新
	/// @param
    void Update(void) override;
};

