#pragma once

#include <string>
#include <unordered_map>
#include "../Object/Character/UIData/CharacterUIData.h"
#include "../Object/Card/CardBase.h"
#include "../../Template/Singleton.h"

class DataBank :
	public Singleton<DataBank>
{
	// シングルトン
	friend class Singleton<DataBank>;

public:	

	/// @brief 初期化
	/// @param  
	void Init(void);

	/// @brief フルスクリーンフラグの取得
	/// @param  
	/// @return フルスクリーンフラグ
	const bool GetIsFullScreen(void) { return isFullScreen_; }

	/// @brief フルスクリーンの設定
	/// @param _isFullScreen フルスクリーンフラグ(true:フルスクリーン　false:ウィンドウ)
	void SetIsFullScreen(const bool _isFullScreen);

private:

	/// @brief コンストラクタ
	/// @param  
	DataBank(void);

	/// @brief コピー禁止
	/// @param instance_ 
	DataBank(const DataBank& instance_) = delete;
	DataBank& operator=(const DataBank& _copy) = delete;

	/// @brief デストラクタ
	/// @param  
	~DataBank(void)override;

	//フルスクリーンの設定
	bool isFullScreen_;
};