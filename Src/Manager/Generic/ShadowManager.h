#pragma once

#include "Template/Singleton.h"

class CharacterManager;

class ShadowManager:
	public Singleton<ShadowManager>
{
	//シングルトン
	friend class Singleton<ShadowManager>;

public:

	/// @brief 初期化
	/// @param  
	void Init(void);

	/// @brief 解放
	/// @param  
	void Release(void);

	/// @brief シャドウ描画の初期化
	/// @param  
	void DrawInitShadow(void);

	/// @brief DxLibによるシャドウマップを始める際のセット
	/// @param  
	void DrawStartSetUp(void);

	/// @brief DxLibによるシャドウマップを終える際のセット
	/// @param  
	void DrawEndSetUp(void);

	/// @brief シェーダーのリセット
	/// @param  
	void ResetShader(void);

	/// @brief シャドウマップテクスチャの取得
	/// @param  
	/// @return 
	const int GetShadowMapTexture(void) { return shadowMapTexture_; }

	/// @brief ライト×ビュープロジェクションの行列の取得
	/// @param  
	/// @return ライト×ビュープロジェクションの行列
	const MATRIX& GetShadowMapViewProjectionMat(void) const { return shadowMapViewProjectionMat__; }

private:

#pragma region 定数
	// 色深度
	static constexpr int COLOR_BIT_DEPTH = 24;

	// シャドウマップサイズ
	static constexpr int SHADOW_MAP_SIZE = 3000;

	//シャドウマップの範囲
	static constexpr int SHADOW_MAP_WIDTH = 4096;
	static constexpr int SHADOW_MAP_HEIGHT = 4096;

	// 通常のチャンネル数
	static constexpr int DEFAULT_CHANNEL_NUM = 4;

	//シャドウマップ範囲
	static constexpr float SHADOWMAP_SIZE_HORIZON = 3000.0F;
	static constexpr float SHADOWMAP_SIZE_VERTICAL = 3000.0F;
#pragma endregion

#pragma region メンバー変数
	SceneManager& scnMng_;				// シーン管理クラスの参照
	CharacterManager& charaMng_;	// キャラクター管理クラスの参照

	int shadowMapTexture_;			//シャドウマップ用テクスチャのハンドル

	MATRIX shadowMapViewProjectionMat__;				//ライト×ビュープロジェクションの行列
#pragma endregion

#pragma region メンバー関数

	//コンストラクタ(シングルトンのためprivate)
	ShadowManager(void);

	//コピー禁止
	ShadowManager(const ShadowManager& _copy) = delete;
	ShadowManager& operator=(const ShadowManager& _copy) = delete;

	//デストラクタ
	~ShadowManager(void)override = default;
#pragma endregion



};

