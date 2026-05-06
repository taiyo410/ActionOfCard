#pragma once

//フレームレート
static constexpr float FRAME_RATE(1000 / 60);

class FpsControl
{

public:

	/// @brief コンストラクタ
	/// @param  
	FpsControl(void);

	/// @brief デストラクタ
	/// @param  
	~FpsControl(void);
	
	/// @brief 初期化
	/// @param  
	void Init(void);

	/// @brief フレームレート更新
	/// @param  
	/// @return 
	bool UpdateFrameRate(void);

	/// @brief フレームレート計算
	/// @param  
	void CalcFrameRate(void);
	
	/// @brief フレームレート表示(デバッグ表示)
	/// @param  
	void DrawFrameRate(void);

private:

	//現在の時間
	int currentTime_;

	//前回のフレーム実行時の時間
	int prevFrameTime_;

	//フレームカウント用
	int frameCnt_;

	//フレームレートを更新した時間
	int updateFrameRateTime_;

	//フレームレート(表示用)
	float frameRate_;
};