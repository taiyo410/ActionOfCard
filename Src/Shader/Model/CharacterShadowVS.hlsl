// VS/PS共通
#include "../Common/VertexToPixelHeader.hlsli"

// IN
#include "../Common/Vertex/VertexInputType.hlsli"
#define VERTEX_INPUT DX_MV1_VERTEX_TYPE_NMAP_4FRAME

// OUT
#define VS_OUTPUT VertexToPixelLit
#include "../Common/Vertex/VertexShader3DHeader.hlsli"

#define L_W_MAT g_localWorldMatrix.lwMatrix
#define L_W_MAT_MAX_INDEX (DX_D3D11_VS_CONST_WORLD_MAT_NUM * 3 - 3)

cbuffer cbParam : register(b7)
{
    float3 g_camera_pos;
    float g_fog_start;
    
    float3 g_light_pos;
    float g_fog_end;
}

// 定数バッファ：スロット8番目
cbuffer cbParamShadow : register(b8)
{
    float4x4 g_light_viewmatrix;
    float4x4 g_light_projectionMatrix;
};

VS_OUTPUT main(VS_INPUT VSInput)
{
    VS_OUTPUT ret;
    
    // スキンメッシュ用のローカル⇒ワールド変換行列を作成+++( 開始 )
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
    
    
    // スキンメッシュ用のローカル⇒ワールド変換行列を作成+++( 終了 )
    
    //// 頂点座標変換 
    //float4 lLocalPosition;
    //float4 lWorldPosition;
    //float4 lViewPosition;
    //float3 lWorldNormal;
    //float3 localNormal = VSInput.norm;
    
    // float3 → float4
    //lLocalPosition.xyz = VSInput.pos;
    //lLocalPosition.w = 1.0f;
    
    // ローカル座標をワールド座標に変換(剛体)
    float4 localPosition = float4(VSInput.pos, 1.0f);
    float4 worldPosition;
    worldPosition.x = dot(localPosition, skinMat0);
    worldPosition.y = dot(localPosition, skinMat1);
    worldPosition.z = dot(localPosition, skinMat2);
    worldPosition.w = 1.0f;
    ret.worldPos = worldPosition.xyz;
    
    // ワールド座標をビュー座標に変換
    float4 viewPosition;
    viewPosition.xyz = mul(worldPosition, g_base.viewMatrix);
    viewPosition.w = 1.0f;
    //ret.vwPos.xyz = viewPosition.xyz;
    
    // ビュー座標を射影座標に変換
    ret.svPos = mul(viewPosition, g_base.projectionMatrix);
    
    // ライトのビュー座標をライトの射影座標に変換
    float4 lLViewPosition = mul(g_light_viewmatrix, worldPosition);
 
	// ライトのビュー座標をライトの射影座標に変換
    ret.lightAtPos = mul(g_light_projectionMatrix, lLViewPosition).xyz;
    
    // その他、ピクセルシェーダへ引継&初期化 ++++++++++++( 開始 )
    // UV座標
    ret.uv.x = VSInput.uv0.x;
    ret.uv.y = VSInput.uv0.y;
    // 法線
    ret.normal = normalize(mul(VSInput.norm, (float3x3) g_base.localWorldMatrix));
    float4 localNormal = float4(VSInput.norm, 0.0f);
    ret.normal.x = dot(localNormal, skinMat0);
    ret.normal.y = dot(localNormal, skinMat1);
    ret.normal.z = dot(localNormal, skinMat2);
    ret.normal = normalize(ret.normal);
    // ディフューズカラー
    ret.diffuse = VSInput.diffuse;
    
    //// フォグの強さ
    //float foglength = length(worldPosition.xyz - g_camera_pos);
    //float fog = (g_fog_end - foglength) / (g_fog_end - g_fog_start);
    //ret.fogFactor = saturate(fog);
    
    //// ポイントライト
    //float lightDit = 200.0f;
    //float lightLength = length(worldPosition.xyz - g_light_pos);
    //ret.lightPower = saturate((lightDit - lightLength) / lightDit);
    // その他、ピクセルシェーダへ引継&初期化 ++++++++++++( 終了 )
    
    // 出力パラメータを返す
    return ret;
}