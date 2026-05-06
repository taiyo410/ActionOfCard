#pragma once
#include "SceneBase.h"

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
	void Release(void)override;

private:

	//イージング時間
	static constexpr float EASING_TIME = 1.0f;

	//BGMの大きさ
	static constexpr float BGM_VOL = 0.8f;

	//イージングY座標上限
	static constexpr float BACK_TITLE_STRING_POS_EASE_LIMIT = 40;

	//ゲームクリア画像
	int imgGameClear_;

	//サウンド
	SoundManager& soundMng_;

	//イージング
	std::unique_ptr<Easing>easing_;

	//イージングカウント
	float easeCnt_;

	//文字のY座標
	float strYPos_;

	//更新関数
	void NormalUpdate(void) override;

	//描画関数
	void NormalDraw(void) override;

	//処理の変更
	void OnSceneEnter(void) override;
};

