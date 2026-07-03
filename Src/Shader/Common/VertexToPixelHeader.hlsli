
struct VertexToPixel
{
    float4 svPos		: SV_POSITION;
    float4 diffuse		: COLOR0;
    float4 specular		: COLOR1;
    float2 uv			: TEXCOORD;
};

struct VertexToPixelLit
{
	float4 svPos		: SV_POSITION;	// 座標( プロジェクション空間 )
	float2 uv			: TEXCOORD0;	// テクスチャ座標
	//float3 vwPos		: TEXCOORD6;	// 座標( ビュー座標 )
	float3 normal       : TEXCOORD1;	// 法線( ビュー座標 )
	float4 diffuse      : COLOR0;		// ディフューズカラー
	float3 worldPos		: TEXCOORD2;		// 頂点座標(ワールド座標)
    float3 lightAtPos	: TEXCOORD3;		// ライトから見た座標(ビュー座標)
    float3 tan          : TEXCOORD4; // 接線(ローカル空間)
    float3 bin          : TEXCOORD5; // 従法線(ローカル空間)
};
struct VertexToPixelShadow
{
	float4 svPos		: SV_POSITION;	// 座標( プロジェクション空間 )
	float2 uv			: TEXCOORD0;	// テクスチャ座標
	float4 vwPos		: TEXCOORD1;	// 座標( ビュー座標 )
    float4 svPosRead	: TEXCOORD2;	// 座標( プロジェクション空間 )
    float3 normal		: TEXCOORD3;	// 法線( ビュー座標 )
    float3 tan			: TEXCOORD4;	// 接線(ローカル空間)
    float3 bin			: TEXCOORD5;	// 従法線(ローカル空間)
    float3 lightAtPos	: TEXCOORD6;	// ライトから見た座標(ビュー座標)
};

float ShadowCalculation(float3 lightAtPos, Texture2D tex, SamplerState texSampler, float3 normal, float3 lightDir, float shadowSize = 16000.0f)
{
    float blurScale = 2.0f;
    float2 depthUV;
    depthUV.x = (lightAtPos.x + 1.0f) / 2.0f;
    depthUV.y = 1.0f - (lightAtPos.y + 1.0f) / 2.0f;
 
    // シャドウマップの解像度に合わせてテクセルサイズを計算
    float2 texelSize = float2(1.0f / shadowSize, 1.0f / shadowSize);
 
    float shadowAccum = 0.0f;
    float bias = 0.001f; // ストライプ（アクネ）を防ぐためのオフセット
 
    // 3x3の範囲でサンプリング
    [unroll]
    for (int x = -1; x <= 1; ++x)
    {
        for (int y = -1; y <= 1; ++y)
        {
            // 周辺ピクセルの座標を計算
            float2 offset = float2(x, y) * texelSize * blurScale;
            float pcfDepth = tex.Sample(texSampler, depthUV + offset).r;
            // 現在の深度と比較
            // 影なら 0.5f、光が当たっていれば 1.0f を加算
            shadowAccum += (lightAtPos.z > pcfDepth + bias) ? 0.5f : 1.0f;
        }
    }
 
    // 9点の平均を返す
    return shadowAccum / 9.0f;
}

// 法線の計算    
float3 CalculateNormal(const float3 tan, const float3 bin, const float3 normal, const float3 tanNormal)
{
    const float3x3 tangentViewMat = transpose(float3x3(normalize(tan), normalize(bin), normalize(normal)));
    return normalize(mul(tangentViewMat, tanNormal));
}
