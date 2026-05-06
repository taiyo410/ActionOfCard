#pragma once
#include <DxLib.h>
#include "../Common/Vector2.h"
#include "../Renderer/ModelMaterial.h"

class ModelRenderer
{

public:

	// 頂点シェーダ用オリジナル定数バッファの使用開始スロット
	static constexpr int CONSTANT_BUF_SLOT_BEGIN_VS = 7;

	// ピクセルシェーダ用オリジナル定数バッファの使用開始スロット
	static constexpr int CONSTANT_BUF_SLOT_BEGIN_PS = 4;

	/// @brief コンストラクタ
	/// @param modelId_ モデルID
	/// @param modelMaterial モデルマテリアル参照
	ModelRenderer(int modelId_, ModelMaterial& modelMaterial);

	/// @brief デストラクタ
	/// @param  
	~ModelRenderer(void);

	/// @brief 描画
	/// @param  
	void Draw(void);

private:

	// モデルのハンドルID
	int modelId_;

	// モデルマテリアル
	ModelMaterial& modelMaterial_;

	// シェーダ設定(頂点)
	void SetReserveVS(void);

	// シェーダ設定(ピクセル)
	void SetReservePS(void);
};
