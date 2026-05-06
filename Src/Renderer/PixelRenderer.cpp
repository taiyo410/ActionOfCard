#include "../Application.h"
#include "PixelRenderer.h"

PixelRenderer::PixelRenderer(PixelMaterial& pixelMaterial) :
	material_(pixelMaterial),
	pos_(),
	size_()
{
}

PixelRenderer::~PixelRenderer(void)
{
}

void PixelRenderer::MakeSquareVertex(Vector2F pos, Vector2F size)
{

	pos_ = pos;
	size_ = size;

	float sX = pos.x;
	float sY = pos.y;
	float eX = pos.x + size.x;
	float eY = pos.y + size.y;

	MakeVertex(sX, sY, eX, eY);
}

void PixelRenderer::MakeSquareVertexFromCenter(Vector2F centerPos, Vector2F size)
{
	pos_ = { centerPos.x - size.x / 2.0f, centerPos.y - size.y / 2.0f };
	size_ = size;
	float sX = centerPos.x - size.x / 2.0f;
	float sY = centerPos.y - size.y / 2.0f;
	float eX = centerPos.x + size.x / 2.0f;
	float eY = centerPos.y + size.y / 2.0f;
	MakeVertex(sX, sY, eX, eY);
}

void PixelRenderer::MakeSquareVertexFromCenter(void)
{
	MakeSquareVertexFromCenter(pos_, size_);
}

void PixelRenderer::MakeVertex(float sX, float sY, float eX, float eY)
{
	// ４頂点の初期化
	for (int i = 0; i < 4; i++)
	{
		vertexs_[i].rhw = 1.0f;
		vertexs_[i].dif = GetColorU8(255, 255, 255, 255);
		vertexs_[i].spc = GetColorU8(255, 255, 255, 255);
		vertexs_[i].su = 0.0f;
		vertexs_[i].sv = 0.0f;
	}

	int cnt = 0;
	// 左上
	vertexs_[cnt].pos = VGet(sX, sY, 0.0f);
	vertexs_[cnt].u = 0.0f;
	vertexs_[cnt].v = 0.0f;
	cnt++;

	// 右上
	vertexs_[cnt].pos = VGet(eX, sY, 0.0f);
	vertexs_[cnt].u = 1.0f;
	vertexs_[cnt].v = 0.0f;
	cnt++;

	// 右下
	vertexs_[cnt].pos = VGet(eX, eY, 0.0f);
	vertexs_[cnt].u = 1.0f;
	vertexs_[cnt].v = 1.0f;
	cnt++;

	// 左下
	vertexs_[cnt].pos = VGet(sX, eY, 0.0f);
	vertexs_[cnt].u = 0.0f;
	vertexs_[cnt].v = 1.0f;


	// 頂点インデックス
	cnt = 0;
	indexes_[cnt++] = 0;
	indexes_[cnt++] = 1;
	indexes_[cnt++] = 3;

	indexes_[cnt++] = 1;
	indexes_[cnt++] = 2;
	indexes_[cnt++] = 3;
}

void PixelRenderer::MakeSquareVertex(void)
{
	MakeSquareVertex(pos_, size_);
}

void PixelRenderer::MakeScreenVertex(void)
{
	const float SCREEN_SIZE_X = static_cast<float>(Application::SCREEN_SIZE_X);
	const float SCREEN_SIZE_Y = static_cast<float>(Application::SCREEN_SIZE_Y);
	MakeSquareVertex({ 0.0f,0.0f }, { SCREEN_SIZE_X,SCREEN_SIZE_Y });
}

void PixelRenderer::SetPos(Vector2F pos)
{
	pos_ = pos;
}

void PixelRenderer::SetSize(Vector2F size)
{
	size_ = size;
}

void PixelRenderer::Draw(void)
{
	// ピクセルシェーダ設定
	SetUsePixelShader(material_.GetShader());

	size_t size;

	// ピクセルシェーダにテクスチャを転送
	const auto& textures = material_.GetTextures();
	size = textures.size();
	for (int i = 0; i < size; i++)
	{
		SetUseTextureToShader(i, textures[i]);
	}

	// 定数バッファハンドル
	int constBuf = material_.GetConstBuf();

	FLOAT4* constBufsPtr = (FLOAT4*)GetBufferShaderConstantBuffer(constBuf);
	const auto& constBufs = material_.GetConstBufs();

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

	// 定数バッファを更新して書き込んだ内容を反映する
	UpdateShaderConstantBuffer(constBuf);

	// 定数バッファをピクセルシェーダー用定数バッファレジスタにセット
	SetShaderConstantBuffer(
		constBuf, DX_SHADERTYPE_PIXEL, CONSTANT_BUF_SLOT_BEGIN_PS);

	// テクスチャアドレスタイプの取得
	auto texA = material_.GetTextureAddress();
	int texAType = static_cast<int>(texA);

	// テクスチャアドレスタイプを変更
	SetTextureAddressModeUV(texAType, texAType);

	// 描画
	DrawPolygonIndexed2DToShader(vertexs_, NUM_VERTEX, indexes_, NUM_POLYGON);

	// テクスチャアドレスタイプを元に戻す
	SetTextureAddressModeUV(DX_TEXADDRESS_CLAMP, DX_TEXADDRESS_CLAMP);
	
	// 後始末
	//-----------------------------------------

	// テクスチャ解除
	size = textures.size();
	for (int i = 0; i < size; i++)
	{
		SetUseTextureToShader(i, -1);
	}

	// ピクセルシェーダ解除
	SetUsePixelShader(-1);

	// オリジナルシェーダ設定(OFF)
	MV1SetUseOrigShader(false);
	//-----------------------------------------

}

void PixelRenderer::Draw(float x, float y)
{
	pos_.x = x;
	pos_.y = y;
	MakeSquareVertex();
	Draw();
}

void PixelRenderer::DrawFromCenter(float centerX, float centerY)
{
	pos_.x = centerX;
	pos_.y = centerY;
	MakeSquareVertexFromCenter();
	Draw();
}

