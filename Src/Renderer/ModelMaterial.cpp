#include "../Application.h"
#include "../Manager/Resource/ResourceManager.h"
#include "ModelMaterial.h"

ModelMaterial::ModelMaterial(const ResourceManager::SRC _vsSrc, const ResourceManager::SRC _psSrc):
	resMng_(ResourceManager::GetInstance())
{
	//素材タイプが異なる場合は処理を飛ばす
	if (resMng_.Load(_vsSrc).type_ != ResourceData::TYPE::VERTEX_SHADER
		|| resMng_.Load(_psSrc).type_ != ResourceData::TYPE::PIXEL_SHADER)
	{
		return;
	}


	// 頂点シェーダのロード
	shaderVS_ = resMng_.Load(_vsSrc).handleId_;

	// 頂点定数バッファの確保サイズ(FLOAT4をいくつ作るか)
	constBufFloat4SizeVS_ = resMng_.Load(_vsSrc).constBufNum;

	// 頂点シェーダー用の定数バッファを作成
	constBufVS_ = CreateShaderConstantBuffer(sizeof(FLOAT4) * constBufFloat4SizeVS_);

	// ピクセルシェーダのロード
	shaderPS_ = resMng_.Load(_psSrc).handleId_;

	// ピクセル定数バッファの確保サイズ(FLOAT4をいくつ作るか)
	constBufFloat4SizePS_ = resMng_.Load(_psSrc).constBufNum;

	// ピクセルシェーダー用の定数バッファを作成
	constBufPS_ = CreateShaderConstantBuffer(sizeof(FLOAT4) * constBufFloat4SizePS_);

	// テクスチャアドレス
	texAddress_ = TEXADDRESS::CLAMP;

}

void ModelMaterial::AddConstBufVS(const FLOAT4& contBuf)
{
	if (constBufFloat4SizeVS_ > constBufsVS_.size())
	{
		constBufsVS_.emplace_back(contBuf);
	}
}

void ModelMaterial::AddConstBufPS(const FLOAT4& contBuf)
{
	if (constBufFloat4SizePS_ > constBufsPS_.size())
	{
		constBufsPS_.emplace_back(contBuf);
	}
}

void ModelMaterial::SetConstBufVS(int idx, const FLOAT4& contBuf)
{
	if (idx >= constBufsVS_.size())
	{
		return;
	}

	constBufsVS_[idx] = contBuf;
}

void ModelMaterial::SetConstBufPS(int idx, const FLOAT4& contBuf)
{
	if (idx >= constBufsPS_.size())
	{
		return;
	}

	constBufsPS_[idx] = contBuf;
}

void ModelMaterial::SetTextureBuf(int slot, int texDiffuse)
{
	if (textures_.count(slot) == 0)
	{
		textures_.emplace(slot, texDiffuse);
	}
	else
	{
		textures_[slot] = texDiffuse;
	}
}

ModelMaterial::~ModelMaterial(void)
{
	DeleteShader(shaderVS_);
	DeleteShaderConstantBuffer(constBufVS_);
	DeleteShader(shaderPS_);
	DeleteShaderConstantBuffer(constBufPS_);
}
