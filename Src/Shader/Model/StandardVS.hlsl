// VS/PS共通
#include "../Common/VertexToPixelHeader.hlsli"
// IN
#include "../Common/Vertex/VertexInputType.hlsli"
#define VERTEX_INPUT DX_MV1_VERTEX_TYPE_NMAP_1FRAME
// OUT
#define VS_OUTPUT VertexToPixelLit
#include "../Common/Vertex/VertexShader3DHeader.hlsli"
cbuffer cbParam : register(b7)
{
    float g_uv_scale;
    float3 dummy;
    float4x4 g_LightVPMatrix; // ライト用 ビュー×プロジェクション行列 (FLOAT4 x 4行分)
    
}
// 定数バッファ：スロット8番目
cbuffer cbParamShadow : register(b8)
{
    //float4x3 g_light_viewmatrix;
    //float4x4 g_light_projectionMatrix;
    
    //float4x4 g_LightVPMatrix; // ライト用 ビュー×プロジェクション行列 (FLOAT4 x 4行分)
};
VS_OUTPUT main(VS_INPUT VSInput)
{
VS_OUTPUT ret;
// 頂点座標変換 +++++++++++++++++++++++++++++++++++++( 開始 )
    float4 lLocalPosition;
    float4 lWorldPosition;
    float4 lViewPosition;
    
// float3 → float4
    lLocalPosition.xyz = VSInput.pos;
    lLocalPosition.w = 1.0f;
    
// ローカル座標をワールド座標に変換(剛体)
    lWorldPosition.w = 1.0f;
    lWorldPosition.xyz = mul(lLocalPosition, g_base.localWorldMatrix);
    ret.worldPos = lWorldPosition.xyz; // ワールド座標をピクセルシェーダへ引き継ぐ

// ワールド座標をビュー座標に変換
    lViewPosition.w = 1.0f;
    lViewPosition.xyz = mul(lWorldPosition, g_base.viewMatrix);
    //ret.vwPos.xyz = lViewPosition.xyz;
    
// ビュー座標を射影座標に変換
    ret.svPos = mul(lViewPosition, g_base.projectionMatrix);
    
 //   // ライトのビュー座標をライトの射影座標に変換
 //   float4 lLViewPosition = mul(g_light_viewmatrix, ret.worldPos);
 
	//// ライトのビュー座標をライトの射影座標に変換
 //   ret.lightAtPos = mul(g_light_projectionMatrix, lLViewPosition).xyz;
    
// 頂点座標変換 +++++++++++++++++++++++++++++++++++++( 終了 )
    // その他、ピクセルシェーダへ引継&初期化 ++++++++++++( 開始 )
    
// UV座標
    ret.uv.x = VSInput.uv0.x * g_uv_scale;
    ret.uv.y = VSInput.uv0.y * g_uv_scale;
    
// 法線
    ret.normal = VSInput.norm; // 法線をローカル空間からワールド空間へ変換
    ret.normal = normalize(mul(VSInput.norm, (float3x3) g_base.localWorldMatrix));
    
// ディフューズカラー
    ret.diffuse = VSInput.diffuse;
    
//// ライト方向(ローカル)
//    ret.lightDir = float3(0.0f, 0.0f, 0.0f);
    
//// ライトから見た座標
    //ライトから見た座標を計算してピクセルシェーダへ渡す
    float4 lightSpacePos = mul(float4(ret.worldPos, 1.0f), g_LightVPMatrix);
    
    // 透視投影に変更した時のためW除算をしておく
    ret.lightAtPos = lightSpacePos.xyz / lightSpacePos.w;
    
// その他、ピクセルシェーダへ引継&初期化 ++++++++++++( 終了 )
// 出力パラメータを返す
    return ret;
}