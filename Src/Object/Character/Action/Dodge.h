#pragma once

#include "../Base/ActionBase.h"
#include "../Object/Common/Transform.h"

class Dodge :
    public ActionBase
{

public:

    /// @brief コンストラクタ
    /// @param _actCntl アクションコントローラ
    /// @param _spd 回避速度
    Dodge(ActionController& _actCntl, CharacterBase& _character);

    /// @brief デストラクタ
    /// @param  
    ~Dodge(void)override;

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
    //回避アニメーションの変数
    float dodgeAnimStep_ = 0.0f;
#pragma endregion

#pragma region メンバー変数
    VECTOR dodgeDir_;       //回避方向
    float dodgeSpdAdd_;    //回避速度の加減
#pragma endregion

};