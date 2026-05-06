#pragma once
#include <DxLib.h>
#include "../Common/Vector2.h"
#include "../Renderer/PixelMaterial.h"

class PixelRenderer
{

public:

	// ピクセルシェーダ用オリジナル定数バッファの使用開始スロット
	static constexpr int CONSTANT_BUF_SLOT_BEGIN_PS = 4;

	// 頂点数
	static constexpr int NUM_VERTEX = 4;

	// 頂点インデックス数
	static constexpr int NUM_VERTEX_IDX = 6;

	// ポリゴン数
	static constexpr int NUM_POLYGON = 2;

	// コンストラクタ
	PixelRenderer(PixelMaterial& pixelMaterial);

	// デストラクタ
	~PixelRenderer(void);

	// 描画矩形の生成
	
	/// @brief 描画矩形の生成
	/// @param pos 描画座標
	/// @param size サイズ
	void MakeSquareVertex(Vector2F pos, Vector2F size); 
	void MakeSquareVertex(void);
	
	/// @brief 中心から作る矩形
	/// @param centerPos 中心座標
	/// @param size サイズ
	void MakeSquareVertexFromCenter(Vector2F centerPos, Vector2F size);
	void MakeSquareVertexFromCenter(void);

	/// @brief 頂点の作成
	/// @param sX 始端X座標
	/// @param sY 始端Y座標
	/// @param eX 終端X座標
	/// @param eY 終端Y座標
	void MakeVertex(float sX, float sY, float eX, float eY);

	/// @brief スクリーンサイズに合わせた頂点作成
	/// @param  
	void MakeScreenVertex(void);

	/// @brief 座標設定
	/// @param pos 座標
	void SetPos(Vector2F pos);

	/// @brief 画像サイズ設定
	/// @param size サイズ
	void SetSize(Vector2F size);

	// 描画
	void Draw(void);
	void Draw(float x, float y);
	void DrawFromCenter(float centerX, float centerY);

private:

	// 座標
	Vector2F pos_;

	// 描画サイズ
	Vector2F size_;

	// 頂点
	VERTEX2DSHADER vertexs_[NUM_VERTEX];

	// 頂点インデックス
	WORD indexes_[NUM_VERTEX_IDX];

	// ピクセルマテリアル
	PixelMaterial& material_;
};
