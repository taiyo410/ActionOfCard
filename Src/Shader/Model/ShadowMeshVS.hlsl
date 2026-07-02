// VS/PS共通
#define VertexToPixelShadow_DEFINE
#include "../Common/VertexToPixelHeader.hlsli"
 
// IN
#include "../Common/Vertex/VertexInputType.hlsli"
#define VERTEX_INPUT DX_MV1_VERTEX_TYPE_NMAP_1FRAME
 
// OUT
#define VS_OUTPUT VertexToPixelShadow
#include "../Common/Vertex/VertexShader3DHeader.hlsli"

VS_OUTPUT main(VS_INPUT VSInput)
{
 
	VS_OUTPUT ret;
 
	// 頂点座標変換 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 開始 )
    float4 lLocalPosition;
    float4 lWorldPosition;
    float4 lViewPosition;
 
	// float3 → float4
    lLocalPosition.xyz = VSInput.pos;
    lLocalPosition.w = 1.0f;
 
	// ローカル座標をワールド座標に変換
    lWorldPosition.w = 1.0f;
    lWorldPosition.xyz = mul(lLocalPosition, g_base.localWorldMatrix);
 
	// ワールド座標をビュー座標に変換
    lViewPosition.w = 1.0f;
    lViewPosition.xyz = mul(lWorldPosition, g_base.viewMatrix);
 
	// ビュー座標を射影座標に変換
    ret.svPos = mul(lViewPosition, g_base.projectionMatrix);
    ret.svPosRead = ret.svPos;
	// 法線をローカル空間からワールド空間へ変換
    ret.normal = normalize(mul(VSInput.norm, (float3x3) g_base.localWorldMatrix));
 
	// UV
    ret.uv.xy = VSInput.uv0.xy;
 
	// 出力パラメータを返す
    return ret;
 
}