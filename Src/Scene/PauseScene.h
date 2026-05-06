#pragma once
#include <string>
#include <unordered_map>
#include "SceneBase.h"

class PauseScene : public SceneBase
{

public:

	enum class LIST
	{
		RESUME,  // 続ける
		TITLE,   // タイトルに戻る
		GAME_END, //ゲーム終了
		MAX
	};

	//リスト
	static constexpr int LIST_MAX = static_cast<int>(LIST::MAX);

	/// @brief コンストラクタ
	/// @param  
	PauseScene(void);

	/// @brief デストラクタ
	/// @param  
	~PauseScene(void) override;

	/// @brief 読み込み
	/// @param  
	void Load(void)override;

	/// @brief 初期化
	/// @param  
	void Init(void)override;

	/// @brief 解放
	/// @param  
	void Release(void);
	
	/// @brief 更新
	/// @param  
	void NormalUpdate(void)override;
	
	/// @brief 描画
	/// @param  
	void NormalDraw(void)override;

private:
	
	//続ける文字列
	const std::wstring CONTINUE_STR = L"つづける";

	//タイトルに戻る文字列
	const std::wstring BACK_TITLE_STR = L"タイトルに戻る";

	//ゲーム終了の文字列
	const std::wstring GAME_END_STR = L"ゲーム終了";

	//画面のアルファ値
	static constexpr int PAUSE_ALPHA = 128; 

	// フォントの太さ
	static constexpr int FONT_THICK = 3; 

	//ポーズフォント
	int pauseFont_; 	

	//ポーズリストの選択
	int selectIndex_;

	//選択リスト
	std::wstring pauseList_[LIST_MAX] = { CONTINUE_STR,BACK_TITLE_STR,GAME_END_STR };

	//リスト選択テーブル
	std::unordered_map<LIST, std::function<void()>> listFuncTable_;

	//シーンに入った後に行う処理
	void OnSceneEnter(void)override;
};

