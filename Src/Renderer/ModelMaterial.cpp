#include "Application.h"
#include "Manager/Resource/ResourceManager.h"
#include "ModelMaterial.h"

ModelMaterial::ModelMaterial(const ResourceManager::SRC _vsSrc, const ResourceManager::SRC _psSrc):
	resMng_(ResourceManager::GetInstance()),
	constBufMatrixSizeVS_(),
	constBufFloat4SizePS_(),
	constBufFloat4VS_(),
	constBufMatrixVS_(),
	constBufPS_(),
	shaderPS_(),
	shaderVS_(),
	texAddress_()
{
	//ロードした素材データ
	const ResourceData& vsLoadData = resMng_.Load(_vsSrc);
	//ピクセルシェーダのデータ
	const ResourceData& psLoadData = resMng_.Load(_psSrc);


	//素材タイプが異なる場合は処理を飛ばす
	if (vsLoadData.type_ != ResourceData::TYPE::VERTEX_SHADER
		|| psLoadData.type_ != ResourceData::TYPE::PIXEL_SHADER)
	{
		assert(vsLoadData.type_ == ResourceData::TYPE::VERTEX_SHADER
			|| psLoadData.type_ == ResourceData::TYPE::PIXEL_SHADER
			&& "シェーダのタイプが不正です");
		return;
	}
	// ピクセルシェーダのロード
	shaderPS_ = psLoadData.handleId_;

	// 頂点シェーダのロード
	shaderVS_ = vsLoadData.handleId_;

	// 頂点定数バッファの確保サイズ(FLOAT4をいくつ作るか)
	constBufFloat4SizeVS_ = vsLoadData.constBufFloat4Num;

	//MATRIX定数バッファの確保サイズ
	constBufMatrixSizeVS_ = vsLoadData.constBufMatrixNum;

	// 頂点シェーダー用の定数バッファを作成
	//FLOAT4
	constBufFloat4VS_ = CreateShaderConstantBuffer(sizeof(FLOAT4) * constBufFloat4SizeVS_);
	//MATRIX
	constBufMatrixVS_ = CreateShaderConstantBuffer(sizeof(MATRIX) * constBufMatrixSizeVS_);

	// ピクセル定数バッファの確保サイズ(FLOAT4をいくつ作るか)
	constBufFloat4SizePS_ = psLoadData.constBufFloat4Num;

	// ピクセルシェーダー用の定数バッファを作成
	//FLOAT4
	constBufPS_ = CreateShaderConstantBuffer(sizeof(FLOAT4) * constBufFloat4SizePS_);

	// テクスチャアドレス
	texAddress_ = TEXADDRESS::CLAMP;
}

void ModelMaterial::AddConstBufVS(const FLOAT4& contBuf)
{
	if (constBufFloat4SizeVS_ > constBufsFloat4VS_.size())
	{
		constBufsFloat4VS_.emplace_back(contBuf);
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
	if (idx >= constBufsFloat4VS_.size())
	{
		return;
	}

	constBufsFloat4VS_[idx] = contBuf;
}

void ModelMaterial::SetConstBufPS(int idx, const FLOAT4& contBuf)
{
	if (idx >= constBufsPS_.size())
	{
		return;
	}

	constBufsPS_[idx] = contBuf;
}

void ModelMaterial::AddConstBufVSMatrix(const MATRIX& mat)
{
	if (constBufMatrixSizeVS_ > constBufsMatrixVS_.size())
	{
		constBufsMatrixVS_.emplace_back(mat);
	}
}

void ModelMaterial::SetConstBufVSMatrix(const int idx, const MATRIX& mat)
{
	if (idx >= constBufsMatrixVS_.size())
	{
		return;
	}

	constBufsMatrixVS_[idx] = mat;
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
	DeleteShaderConstantBuffer(constBufFloat4VS_);
	DeleteShader(shaderPS_);
	DeleteShaderConstantBuffer(constBufPS_);
}
