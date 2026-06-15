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

    //通常移動スピードにプラスする
    static constexpr float ADD_DODGE_SPEED = 16.0f;

    //回避を終えるアニメーションステップ
    static constexpr float END_DODGE_ANIM_STEP = 30.0f;

    //回避方向
    VECTOR dodgeDir_;

	//回避アニメーションの変数
    float dodgeAnimStep_;

	//回避速度の加減
	float dodgeSpdAdd_;
};

