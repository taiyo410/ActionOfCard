#pragma once

#include "Template/Singleton.h"

class SceneManager;
class CharacterManager;

class Shadow
{
public:
	/// @brief コンストラクタ
	/// @param  
	Shadow(void);

	/// @brief デストラクタ
	/// @param  
	~Shadow(void);

	/// @brief 初期化
	/// @param  
	void Init(void);

	///// @brief 描画
	///// @param  
	//void Draw(void);

	/// @brief シャドウ描画の初期化
	/// @param  
	void DrawInitShadow(void);

	/// @brief 通常のメッシュのシャドウマップ描画
	/// @param _drawFunc 描画する関数
	void DrawShadowNormal(void);

	/// @brief スキンメッシュのシャドウマップ描画
	/// @param _drawFunc 描画する関数
	void DrawShadowSkinned(void);

	/// @brief シェーダーのリセット
	/// @param  
	void ResetShader(void);

	/// @brief ライトビュー行列の取得
	/// @param
	/// @return ライトビュー行列
	const MATRIX& GetLightViewMatrix(void) const { return lightViewMatrix_; }

	/// @brief ライトプロジェクション行列の取得
	/// @param
	/// @return ライトプロジェクション行列
	const MATRIX& GetLightProjectionMatrix(void) const { return lightProjectionMatrix_; }

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

	// 通常のチャンネル数
	static constexpr int DEFAULT_CHANNEL_NUM = 4;
#pragma endregion


#pragma region メンバー変数
	SceneManager& scnMng_;				// シーン管理クラスの参照
	CharacterManager& charaMng_;	// キャラクター管理クラスの参照

	int shadowPsHandle_;			// シャドウマップ生成用ピクセルシェーダーのハンドル
	int shadowMeshVSHandle_;		// シャドウマップ生成用メッシュ頂点シェーダーのハンドル
	int shadowSkinnedMeshVSHandle_;	// シャドウマップ生成用スキンメッシュのハンドル

	MATRIX lightViewMatrix_;			// ライトのビュー行列
	MATRIX lightProjectionMatrix_;	// ライトのプロジェクション行列
#pragma endregion


};

