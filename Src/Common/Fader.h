#pragma once

class Fader
{

public:

	// フェードが進む速さ
	static constexpr float SPEED_ALPHA = 5.0f;

	// 状態
	enum class STATE
	{
		NONE, 
		FADE_OUT,	// 徐々に暗転
		FADE_IN		// 徐々に明転
	};

	/// @brief コンストラクタ
	/// @param  
	Fader(void);

	/// @brief デストラクタ
	/// @param  
	~Fader(void);

	// 状態の取得
	const STATE GetState(void) const{ return state_; }

	// フェード処理が終了しているか
	const bool IsEnd(void) const { return isEnd_; };
	
	/// @brief 初期化
	/// @param  
	void Init(void);

	/// @brief 更新
	/// @param  
	void Update(void);

	/// @brief 描画
	/// @param  
	void Draw(void);

	/// @brief 指定フェードを開始する
	/// @param state 
	void SetFade(STATE state);

private:

	// 状態
	STATE state_;

	// 透明度
	float alpha_;

	// 状態(STATE)を保ったまま終了判定を行うため、
	// Update->DrawPlayerUI->Updateの1フレーム判定用
	bool isPreEnd_;

	// フェード処理の終了判定
	bool isEnd_;
};