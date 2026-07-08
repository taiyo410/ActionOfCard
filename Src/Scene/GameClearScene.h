#pragma once
#include "./SceneBase.h"

class Easing;

class GameClearScene :
    public SceneBase
{

public:

	/// @brief コンストラクタ
	/// @param  
	GameClearScene(void);

	/// @brief デストラクタ
	/// @param  
	~GameClearScene(void)override;

	/// @brief ロード
	/// @param  
	void Load(void) override;

	/// @brief 初期化
	/// @param  
	void Init(void) override;

	/// @brief 解放
	/// @param  
	void Release(void) override;

private:

#pragma region メンバー定数
	//イージング時間
	static constexpr float EASING_TIME = 1.0f;

	//イージングY座標上限
	static constexpr float BACK_TITLE_STRING_POS_EASE_LIMIT = 40;
#pragma endregion

#pragma region メンバー変数
	std::unique_ptr<Easing>easing_;	//イージング
	int imgGameClear_;	//ゲームクリア画像
	float easeCnt_;		//イージングカウント
	float strYPos_;		//文字のY座標
#pragma endregion

#pragma region メンバー関数
	//更新関数
	void NormalUpdate(void) override;

	//描画関数
	void NormalDraw(void) override;

	//処理の変更
	void OnSceneEnter(void) override;
#pragma endregion
};

