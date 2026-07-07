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

	///// @brief 描画
	///// @param  
	//void Draw(void);

	/// @brief シャドウ描画の初期化
	/// @param  
	void DrawInitShadow(void);

	/// @brief DxLibによるシャドウマップを始める際のセット
	/// @param  
	void DrawStartSetUp(void);
	void DrawEndSetUp(void);

	/// @brief 通常のメッシュのシャドウマップ描画
	/// @param _drawFunc 描画する関数
	void DrawShadowNormal(void);

	/// @brief スキンメッシュのシャドウマップ描画
	/// @param _drawFunc 描画する関数
	void DrawShadowSkinned(void);

	/// @brief シェーダーのリセット
	/// @param  
	void ResetShader(void);

	/// @brief シャドウマップテクスチャの取得
	/// @param  
	/// @return 
	const int GetShadowMapTexture(void) { return shadowMapTexture_; }

	/// @brief ライトビュー行列の取得
	/// @param
	/// @return ライトビュー行列
	const MATRIX& GetLightViewMatrix(void) const { return lightViewMatrix_; }

	/// @brief ライトプロジェクション行列の取得
	/// @param
	/// @return ライトプロジェクション行列
	const MATRIX& GetLightProjectionMatrix(void) const { return lightProjectionMatrix_; }

	/// @brief シャドウマップ行列の取得
	/// @param  
	/// @return 
	const MATRIX& GetShadowMatrix(void) const { return shadowMat_; }

	/// @brief ライトのビュー行列を設定
	/// @param lightViewMat ビュー行列
	void SetLightViewMatrix(const MATRIX& lightViewMat) { lightViewMatrix_ = lightViewMat; }

	/// @brief ライトのプロジェクション行列を設定
	/// @param lightProjectionMat プロジェクション行列
	void SetLightProjectionMatrix(const MATRIX& lightProjectionMat) { lightProjectionMatrix_ = lightProjectionMat; }

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
	int shadowPsHandle_;			// シャドウマップ生成用ピクセルシェーダーのハンドル
	int shadowMeshVSHandle_;		// シャドウマップ生成用メッシュ頂点シェーダーのハンドル
	int shadowSkinnedMeshVSHandle_;	// シャドウマップ生成用スキンメッシュのハンドル

	MATRIX shadowMat_;				//シャドウマップの行列
	MATRIX lightViewMatrix_;			// ライトのビュー行列
	MATRIX lightProjectionMatrix_;	// ライトのプロジェクション行列
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

