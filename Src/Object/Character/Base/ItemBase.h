#pragma once

#include "../Object/ObjectBase.h"

class EffectController;

class ItemBase :
    public ObjectBase
{

public:

    /// @brief コンストラクタ
    /// @param  
    ItemBase(void);

    /// @brief デストラクタ
    /// @param  
    ~ItemBase(void)override;

	/// @brief ロード
	/// @param  
	virtual void Load(void) override = 0;

	/// @brief 初期化
	/// @param  
	virtual void Init(void) override = 0;

	/// @brief 更新
	/// @param  
	virtual void Update(void) override = 0;

	/// @brief 描画
	/// @param  
	virtual void Draw(void) override = 0;

	/// @brief // ダメージフラグの取得
	/// @param  
	/// @return true:ダメージ判定あり　false:ダメージ判定なし
	const bool& GetIsDamage(void) const { return isDamage_; }

	/// @brief /// ダメージフラグのリセット
	/// @param  
	void SetIsDamage(void) { isDamage_ = true; }

protected:

	//エフェクト
	std::shared_ptr<EffectController> effect_;

	//ダメージフラグ
	bool isDamage_;

};

