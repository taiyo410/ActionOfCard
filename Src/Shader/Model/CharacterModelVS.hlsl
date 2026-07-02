// VS/PS共通
#include "../Common/VertexToPixelHeader.hlsli"

// IN
#include "../Common/Vertex/VertexInputType.hlsli"
#define VERTEX_INPUT DX_MV1_VERTEX_TYPE_NMAP_4FRAME

// OUT
#define VS_OUTPUT VertexToPixelLit
#include "../Common/Vertex/VertexShader3DHeader.hlsli"

// 定数バッファ：スロット7番目
cbuffer cbParam : register(b7)
{
	//float3 g_camera_pos;
	//float dummy1;

	//float g_fog_start;
	//float g_fog_end;
	//float dummy2;
	//float g_light_num;

	//float4 g_light_pos[30];
}

#define L_W_MAT g_localWorldMatrix.lwMatrix
#define L_W_MAT_MAX_INDEX (DX_D3D11_VS_CONST_WORLD_MAT_NUM * 3 - 3)

VS_OUTPUT main(VS_INPUT VSInput)
{
	VS_OUTPUT ret;
	
	// 頂点座標変換 +++++++++++++++++++++++++++++++++++++( 開始 )
    int4 boneIdx = clamp(VSInput.blendIndices0, 0, L_W_MAT_MAX_INDEX);
    float4 weight = VSInput.blendWeight0;
    float weightSum = max(dot(weight, 1.0f), 0.0001f);
    weight /= weightSum;

    float4 skinMat0 =
        L_W_MAT[boneIdx.x + 0] * weight.x +
        L_W_MAT[boneIdx.y + 0] * weight.y +
        L_W_MAT[boneIdx.z + 0] * weight.z +
        L_W_MAT[boneIdx.w + 0] * weight.w;

    float4 skinMat1 =
        L_W_MAT[boneIdx.x + 1] * weight.x +
        L_W_MAT[boneIdx.y + 1] * weight.y +
        L_W_MAT[boneIdx.z + 1] * weight.z +
        L_W_MAT[boneIdx.w + 1] * weight.w;

    float4 skinMat2 =
        L_W_MAT[boneIdx.x + 2] * weight.x +
        L_W_MAT[boneIdx.y + 2] * weight.y +
        L_W_MAT[boneIdx.z + 2] * weight.z +
        L_W_MAT[boneIdx.w + 2] * weight.w;
	
	// ローカル座標をワールド座標に変換(剛体)
    float4 localPosition = float4(VSInput.pos, 1.0f);
    float4 worldPosition;
    worldPosition.x = dot(localPosition, skinMat0);
    worldPosition.y = dot(localPosition, skinMat1);
    worldPosition.z = dot(localPosition, skinMat2);
    worldPosition.w = 1.0f;
	
	// ワールド座標をビュー座標に変換
    float4 viewPosition;
    viewPosition.xyz = mul(worldPosition, g_base.viewMatrix);
    viewPosition.w = 1.0f;
    ret.vwPos.xyz = viewPosition.xyz;
	
	// ビュー座標を射影座標に変換
    ret.svPos = mul(viewPosition, g_base.projectionMatrix);

	// 頂点座標変換 +++++++++++++++++++++++++++++++++++++( 終了 )

	// その他、ピクセルシェーダへ引継&初期化 ++++++++++++( 開始 )
	// UV座標
	ret.uv.x = VSInput.uv0.x;
	ret.uv.y = VSInput.uv0.y;
	// 法線をローカル空間からワールド空間へ変換
    ret.normal = VSInput.norm;
    ret.normal = normalize(mul(VSInput.norm, (float3x3) g_base.localWorldMatrix));
    float4 localNormal = float4(VSInput.norm, 0.0f);
    ret.normal.x = dot(localNormal, skinMat0);
    ret.normal.y = dot(localNormal, skinMat1);
    ret.normal.z = dot(localNormal, skinMat2);
    ret.normal = normalize(ret.normal);
	// ディフューズカラー
	ret.diffuse = VSInput.diffuse;
	//// ライト方向(ローカル)
	//ret.lightDir = float3(0.0f, 0.0f, 0.0f);
	//// ライトから見た座標
	//ret.lightAtPos = float3(0.0f, 0.0f, 0.0f);
	//// フォグの強さ(0.0:フォグが濃い、1.0:フォグが薄い)
	//ret.fogFactor =
	//	clamp((g_fog_end - length(worldPosition.xyz - g_camera_pos)) / (g_fog_end - g_fog_start), 0.0, 1.0);
	// ポイントライトの強さ(0.0:暗い、1.0:明るい)
	// その他、ピクセルシェーダへ引継&初期化 ++++++++++++( 終了 )

	// 出力パラメータを返す
	return ret;
}