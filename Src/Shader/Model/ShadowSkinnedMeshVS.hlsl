// VS/PS共通
#include "../Common/VertexToPixelHeader.hlsli"
 
// IN
#include "../Common/Vertex/VertexInputType.hlsli"
#define VERTEX_INPUT DX_MV1_VERTEX_TYPE_NMAP_4FRAME
 
// OUT
#define VS_OUTPUT VertexToPixelShadow
#include "../Common/Vertex/VertexShader3DHeader.hlsli"

#define L_W_MAT g_localWorldMatrix.lwMatrix
#define L_W_MAT_MAX_INDEX (DX_D3D11_VS_CONST_WORLD_MAT_NUM * 3 - 3)
 
VS_OUTPUT main(VS_INPUT VSInput)
{
 
	VS_OUTPUT ret = (VS_OUTPUT) 0;
 
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
    float4 localNormal = float4(VSInput.norm, 0.0f);
    ret.normal.x = dot(localNormal, skinMat0);
    ret.normal.y = dot(localNormal, skinMat1);
    ret.normal.z = dot(localNormal, skinMat2);
    ret.normal = normalize(ret.normal);
 
	// 出力パラメータを返す
    return ret;
 
}