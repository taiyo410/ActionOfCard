#pragma once

#include "./SceneBase.h"

class PixelMaterial;
class PixelRenderer;

class GameEventRevolutionScene :
    public SceneBase
{
public:

	/// @brief コンストラクタ
	/// @param  
	GameEventRevolutionScene(void);

	/// @brief デストラクタ
	/// @param  
	~GameEventRevolutionScene(void)override;

	/// @brief 読み込み
	/// @param  
	void Load(void) override;

	/// @brief 初期化
	/// @param  
	void Init(void) override;

	/// @brief 解放
	/// @param  
	void Release(void)override;
private:

#pragma region メンバー定数
	static constexpr float FADE_TIME = 0.3f;
	static constexpr float WAIT_TIME = 1.0f;
#pragma endregion

#pragma region 外部ファイルから読み込み

#pragma endregion

#pragma region メンバー変数
	//ポストエフェクト
	std::unique_ptr<PixelMaterial>material_;		//マテリアル
	std::shared_ptr<PixelRenderer>renderer_;		//レンダラー

	float fadeCnt_;			//色反転フェードのカウント
	std::function<void(void)> revolutionFadeFunc_;		//反転フェード更新
	float waitCnt_;			//シーン待機時間

#pragma endregion

#pragma region メンバー関数
	//更新
	void NormalUpdate(void)override;
	//描画
	void NormalDraw(void)override;

	//革命更新
	void RevolutionFadeIn(void);
	void RevolutionFadeOut(void);

	//シーンに入った後に行う処理
	void OnSceneEnter(void)override;

#pragma endregion

};

