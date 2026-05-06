#pragma once
#include <string>
#include "Template/Singleton.h"

class FpsControl;
class FontManager;

class Application : public Singleton<Application>
{

	friend class Singleton<Application>; 

public:

	// スクリーンサイズ
	static constexpr int SCREEN_SIZE_X = 1024;
	static constexpr int SCREEN_SIZE_Y = 640;

	// スクリーンサイズハーフ
	static constexpr int SCREEN_HALF_X = SCREEN_SIZE_X / 2;
	static constexpr int SCREEN_HALF_Y = SCREEN_SIZE_Y / 2;

	// データパス関連
	static const std::wstring PATH_IMAGE;
	static const std::wstring PATH_MODEL;
	static const std::wstring PATH_ANIM_PLAYER;
	static const std::wstring PATH_ANIM_ENEMY;
	static const std::wstring PATH_EFFECT;
	static const std::wstring PATH_SOUND;
	static const std::wstring PATH_SOUND_BGM;
	static const std::wstring PATH_SOUND_SE;
	static const std::wstring PATH_FONT;
	static const std::wstring PATH_TEXT;
	static const std::wstring PATH_JSON;
	static const std::wstring PATH_CSV;
	static const std::wstring PATH_SHADER;
	
	/// @brief 初期化
	/// @param  
	/// @return true:初期化成功 false:初期化失敗
	bool Init(void);
	
	/// @brief ゲームの実行ループ
	/// @param  
	void Run(void);
	
	/// @brief リソースの破棄
	/// @param  
	/// @return 初期化成功 false:初期化失敗
	bool Release(void);

	/// @brief ゲームの終了判定をセットする
	/// @param 
	void IsGameEnd(void) { isGameEnd_ = true; }

private:

	//ウィンドウ名
	const std::wstring WINDOW_NAME = L"ActionCardBattle";

	// カラービット数
	static constexpr int COLOR_DEPTH = 32;

	//FPS
	std::unique_ptr<FpsControl> fps_;

	//フォント
	std::unique_ptr<FontManager> fontMng_;

	//ゲーム終了フラグ
	bool isGameEnd_;

	//コンストラクタ
	Application();

	//デストラクタ
	~Application()override = default;

	// Effekseerの初期化
	void InitEffekseer();

};