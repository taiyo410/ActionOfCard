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

	/// @brief シャドウマップ用テクスチャの取得
	/// @param  
	/// @return 
	const int GetShadowMapTexture(void)const { return shadowMapTexture_; }

private:

#pragma region 定数
	// 色深度
	static constexpr int COLOR_BIT_DEPTH = 24;

	// シャドウマップサイズ
	static constexpr int SHADOW_MAP_SIZE = 16000;

	// 通常のチャンネル数
	static constexpr int DEFAULT_CHANNEL_NUM = 4;
#pragma endregion


#pragma region メンバー変数
	SceneManager& scnMng_;				// シーン管理クラスの参照
	CharacterManager& charaMng_;	// キャラクター管理クラスの参照

	int shadowMapTexture_;			// シャドウマップ用テクスチャのハンドル
	int shadowPsHandle_;			// シャドウマップ生成用ピクセルシェーダーのハンドル
	int shadowMeshVSHandle_;		// シャドウマップ生成用メッシュ頂点シェーダーのハンドル
	int shadowSkinnedMeshVSHandle_;	// シャドウマップ生成用スキンメッシュのハンドル
#pragma endregion


};

