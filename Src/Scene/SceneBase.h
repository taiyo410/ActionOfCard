#pragma once
#include<functional>
#include "../Common/Vector2.h"
#include "../Application.h"

class SceneManager;
class ResourceManager;
class InputManager;
class InputManagerS;
class SoundManager;

class SceneBase
{

public:

	//最低ローディング時間
	static constexpr float LOADING_TIME = 0.0f;

	//フォントサイズ
	static constexpr int FONT_SIZE = 40;

	//ローディング
	static constexpr int COMMA_MAX_NUM = 7;											//「now loading......」のコンマの数
	static constexpr float COMMA_TIME = 0.5f;										//「now loading......」のコンマ数を増やす時間
	static constexpr int LOADING_STRING_POS_X = Application::SCREEN_SIZE_X - 300;	//「now loading......」の座標X
	static constexpr int LOADING_STRING_POS_Y = Application::SCREEN_SIZE_Y - 40;	//「now loading......」の座標Y

	//タイトル戻る文字列の座標
	static constexpr float BACK_TITLE_STRING_POS_Y = 500.0f;

	/// @brief コンストラクタ
	/// @param  
	SceneBase(void);

	/// @brief デストラクタ 
	/// @param  
	virtual ~SceneBase(void);

	/// @brief ロード
	/// @param  
	virtual void Load(void);
	
	/// @brief 初期化
	/// @param  
	virtual void Init(void);

	/// @brief 更新
	/// @param  
	virtual void Update(void);

	/// @brief 描画
	/// @param  
	virtual void Draw(void);

	/// @brief 解放
	/// @param  
	virtual void Release(void);

protected:
	
	//Now Loading文字列
	const std::wstring NOW_LOAD_STR = L"Now loading";

	//ドット
	const std::wstring DOT_STR = L".";

	//クリアシーンとゲームオーバーシーンの戻る説明の文字列
	const std::wstring BACK_TITLE_SCENE_STR = L"'B'ボタンまたは'スペースキー'でタイトルに戻る";

	// リソース管理
	ResourceManager& resMng_;
	InputManager& inputMng_;
	InputManagerS& inputMngS_;
	SceneManager& scnMng_;
	SoundManager& soundMng_;

	//更新処理
	std::function<void(void)> updateFunc_;

	//描画処理
	std::function<void(void)> drawFunc_;

	//ローディング経過時間
	float loadingTime_;

	//選択ボタンフォントハンドル
	int buttonFontHandle_;

	//更新関数
	virtual void LoadingUpdate(void);
	virtual void NormalUpdate(void);

	//描画関数
	virtual void LoadingDraw(void);
	virtual void NormalDraw(void);

	//ローディング処理から通常処理へ
	virtual void OnSceneEnter(void);
	
	//「now loading......」の描画
	void DrawNowLoading(void);
};