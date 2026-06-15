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

#pragma endregion

#pragma region 外部ファイルから読み込み

#pragma endregion

#pragma region メンバー変数
	//ポストエフェクト
	std::unique_ptr<PixelMaterial>material_;		//マテリアル
	std::shared_ptr<PixelRenderer>renderer_;		//レンダラー

#pragma endregion

#pragma region メンバー関数
	/// @brief 更新
	/// @param  
	void NormalUpdate(void)override;

	/// @brief 描画
	/// @param  
	void NormalDraw(void)override;

#pragma endregion

};

