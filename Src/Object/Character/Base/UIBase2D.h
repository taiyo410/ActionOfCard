#pragma once
#include<memory>
#include "../Common/Vector2F.h"
class Easing;
class UIBase2D
{
public:

	/// @brief コンストラクタ
	/// @param  
	UIBase2D(void);

	/// @brief デストラクタ
	/// @param  
	virtual ~UIBase2D(void);

	/// @brief ロード
	/// @param  
	virtual void Load(void) = 0;

	/// @brief 初期化
	/// @param  
	virtual void Init(void) = 0;

	/// @brief 更新
	/// @param  
	virtual void Update(void) = 0;

	/// @brief 描画
	/// @param  
	virtual void Draw(void) = 0;
protected:

	//イージング
	std::unique_ptr<Easing>easing_;
};

