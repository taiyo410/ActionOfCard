#include "./ModelRenderer.h"

ModelRenderer::ModelRenderer(int modelId, ModelMaterial& modelMaterial):
	modelId_(modelId)
	, modelMaterial_(modelMaterial)
{
}

ModelRenderer::~ModelRenderer(void)
{
}

void ModelRenderer::Draw(void)
{
	// オリジナルシェーダ設定(ON)
	MV1SetUseOrigShader(true);

	// シェーダ設定(頂点)
	SetReserveVS();

	// シェーダ設定(ピクセル)
	SetReservePS();

	// テクスチャアドレスタイプの取得
	auto texA = modelMaterial_.GetTextureAddress();
	int texAType = static_cast<int>(texA);

	// テクスチャアドレスタイプを変更
	SetTextureAddressModeUV(texAType, texAType);

	// 描画
	MV1DrawModel(modelId_);

	// テクスチャアドレスタイプを元に戻す
	SetTextureAddressModeUV(DX_TEXADDRESS_CLAMP, DX_TEXADDRESS_CLAMP);

	// 後始末
	//-----------------------------------------

	// テクスチャ解除
	const auto& textures = modelMaterial_.GetTextures();
	size_t size = textures.size();
	if (size == 0)
	{
		// 前回使用分のテクスチャを引き継がないように
		SetUseTextureToShader(0, -1);
	}
	else
	{
		for (const auto& pair : textures)
		{
			SetUseTextureToShader(pair.first, -1);
		}
	}

	// 頂点シェーダ解除
	SetUseVertexShader(-1);

	// ピクセルシェーダ解除
	SetUsePixelShader(-1);

	// オリジナルシェーダ設定(OFF)
	MV1SetUseOrigShader(false);
	//-----------------------------------------
}

void ModelRenderer::SetReserveVS(void)
{
	// 定数バッファハンドル
	int constBuf = modelMaterial_.GetConstBufFloat4VS();

	FLOAT4* constBufsPtr = (FLOAT4*)GetBufferShaderConstantBuffer(constBuf);
	const auto& constBufs = modelMaterial_.GetConstBufsFloat4VS();

	size_t size = constBufs.size();
	for (int i = 0; i < size; i++)
	{
		if (i != 0)
		{
			constBufsPtr++;
		}
		constBufsPtr->x = constBufs[i].x;
		constBufsPtr->y = constBufs[i].y;
		constBufsPtr->z = constBufs[i].z;
		constBufsPtr->w = constBufs[i].w;
	}

	// 頂点シェーダー用の定数バッファを更新して書き込んだ内容を反映する
	UpdateShaderConstantBuffer(constBuf);

	// 頂点シェーダー用の定数バッファを定数バッファレジスタにセット
	SetShaderConstantBuffer(
		constBuf, DX_SHADERTYPE_VERTEX, FLOAT4_CONSTANT_BUF_SLOT_BEGIN_VS);

	//MATRIXの定数バッファハンドルを取得
	int constBufMat = modelMaterial_.GetConstBufMatrixVS();

	//書き込み元のマトリックスの取得
	const auto& constBufsMat = modelMaterial_.GetConstBufsMatrixVS();

	//必要なバイトサイズ
	size_t requiredSize = constBufsMat.size() * sizeof(MATRIX);

	//ポインタを取得
	MATRIX* constBufsMatPtr = (MATRIX*)GetBufferShaderConstantBuffer(constBufMat);

	//バッファへの書き込みが不可な場合
	if (!constBufsMatPtr)
	{
		//処理を終了する
		return;
	}
	//書き込み元のマトリックスが空の場合
	if (!constBufsMat.empty())
	{
		//中身の一括コピー
		memcpy(constBufsMatPtr, constBufsMat.data(), requiredSize);
	}
	for (size_t i = 0; i < constBufsMat.size(); ++i)
	{
		//格納
		constBufsMatPtr[i] = constBufsMat[i];
	}

	//頂点シェーダー用の定数バッファを更新して書き込んだ内容を反映する
	UpdateShaderConstantBuffer(constBufMat);

	//MATRIXの頂点シェーダ用定数バッファ定数バッファレジスタにセット
	SetShaderConstantBuffer(constBufMat, DX_SHADERTYPE_VERTEX, MATRIX_CONSTANT_BUF_SLOT_BEGIN_VS);

	// 頂点シェーダー設定
	SetUseVertexShader(modelMaterial_.GetShaderVS());
}

void ModelRenderer::SetReservePS(void)
{
	// ピクセルシェーダーにテクスチャを転送
	const auto& textures = modelMaterial_.GetTextures();
	size_t size = textures.size();
	if (size == 0)
	{
		// 前回使用分のテクスチャを引き継がないように
		SetUseTextureToShader(0, -1);
	}
	else
	{
		for (const auto& pair : textures)
		{
			SetUseTextureToShader(pair.first, pair.second);
		}
	}

	// 定数バッファハンドル
	int constBuf = modelMaterial_.GetConstBufPS();

	FLOAT4* constBufsPtr = (FLOAT4*)GetBufferShaderConstantBuffer(constBuf);
	const auto& constBufs = modelMaterial_.GetConstBufsPS();

	size = constBufs.size();
	for (int i = 0; i < size; i++)
	{
		if (i != 0)
		{
			constBufsPtr++;
		}
		constBufsPtr->x = constBufs[i].x;
		constBufsPtr->y = constBufs[i].y;
		constBufsPtr->z = constBufs[i].z;
		constBufsPtr->w = constBufs[i].w;
	}

	// ピクセルシェーダー用の定数バッファを更新して書き込んだ内容を反映する
	UpdateShaderConstantBuffer(constBuf);

	// ピクセルシェーダー用の定数バッファを定数バッファレジスタにセット
	SetShaderConstantBuffer(
		constBuf, DX_SHADERTYPE_PIXEL, CONSTANT_BUF_SLOT_BEGIN_PS);

	// ピクセルシェーダー設定
	SetUsePixelShader(modelMaterial_.GetShaderPS());
}
