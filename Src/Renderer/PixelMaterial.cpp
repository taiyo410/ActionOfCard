#include "../Application.h"
#include "../Manager/Resource/ResourceManager.h"
#include "PixelMaterial.h"

PixelMaterial::PixelMaterial(const ResourceManager::SRC _psSrc):
	resMng_(ResourceManager::GetInstance())
	, psSrc_(_psSrc)
{
	//素材タイプがピクセルシェーダでない場合は処理を飛ばす
	if (resMng_.Load(_psSrc).type_ != ResourceData::TYPE::PIXEL_SHADER)
	{
		return;
	}

	// ピクセルシェーダのロード
	shader_ = resMng_.Load(_psSrc).handleId_;

	// 定数バッファの確保サイズ(FLOAT4をいくつ作るか)
	constBufFloat4Size_ = resMng_.Load(_psSrc).constBufNum;

	// ピクセルシェーダー用の定数バッファを作成
	constBuf_ = CreateShaderConstantBuffer(sizeof(FLOAT4) * constBufFloat4Size_);

	// テクスチャアドレス
	texAddress_ = TEXADDRESS::CLAMP;
}

void PixelMaterial::AddConstBuf(const FLOAT4& contBuf)
{
	if (constBufFloat4Size_ > constBufs_.size())
	{
		constBufs_.emplace_back(contBuf);
	}
}

void PixelMaterial::SetConstBuf(int idx, const FLOAT4& contBuf)
{
	if (idx >= constBufs_.size())
	{
		return;
	}

	constBufs_[idx] = contBuf;
}

void PixelMaterial::AddTextureBuf(int texDiffuse)
{
	if (MAX_TEXTURES > textures_.size())
	{
		textures_.emplace_back(texDiffuse);
	}
}

void PixelMaterial::SetTextureBuf(int idx, int texDiffuse)
{
	if (idx >= textures_.size())
	{
		return;
	}

	textures_[idx] = texDiffuse;
}

PixelMaterial::~PixelMaterial(void)
{
}