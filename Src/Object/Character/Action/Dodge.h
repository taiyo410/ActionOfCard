#pragma once
#include "../Base/ActionBase.h"
#include "../Object/Common/Transform.h"

class Dodge :
    public ActionBase
{

public:

    /// @brief コンストラクタ
    /// @param _actCntl アクションコントローラ
    /// @param _trans モデル情報
    /// @param _spd 回避速度
    Dodge(ActionController& _actCntl,Transform& _trans,float _spd);

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

private:

    //通常移動スピードにプラスする
    static constexpr float ADD_DODGE_SPEED = 16.0f;

    //回避を終えるアニメーションステップ
    static constexpr float END_DODGE_ANIM_STEP = 30.0f;

    //回避方向
    VECTOR dodgeDir_;

    //回避スピード
    float& dodgeSpd_;

    //キャラの向いている方向
    Transform& trans_;
};

