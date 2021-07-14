#include "RenderTexture.hlsli"
#include "FXAA.hlsl"	// アンチエイリアスを使用する為に必要.

#define BLOOM_SAMPLE_NUM (5)

//-----------------------------------------------.
// テクスチャ.
//-----------------------------------------------.
Texture2D g_Texture							: register(t0);	// 色情報.
Texture2D g_Back							: register(t1);	// 色情報.
Texture2D g_TextureBloom[BLOOM_SAMPLE_NUM]	: register(t2);	// ブルーム情報.

//-----------------------------------------------.
// サンプラ.
//-----------------------------------------------.
SamplerState g_SamLinear : register(s0);

// トーンマップ.
// https://knarkowicz.wordpress.com/2016/01/06/aces-filmic-tone-mapping-curve/
float3 ACESFilm(float3 x);

float4 PS_Main( VS_OUTPUT input ) : SV_Target
{
	// 描画されたモデルなどの色情報を取得.
	FxaaTex tex = { g_SamLinear, g_Texture };
	float4 color = float4(FxaaPixelShader(input.Tex, tex, float2(g_vPixelSize.x, g_vPixelSize.y)), 1.0f);
	
	// ブルームのサンプル数文.
	for( int i = 0; i < BLOOM_SAMPLE_NUM; i++ ){
		color.rgb += g_TextureBloom[i].Sample(g_SamLinear, input.Tex).rgb;
	}
	
	// 画像や、エフェクトなどを描画.
	float4 t = g_Back.Sample(g_SamLinear, input.Tex);
	if( t.a > 0.0f ) color.rgb = t.rgb;
	
	color.rgb = ACESFilm(color.rgb);
	
	return color;
}

float3 ACESFilm( float3 x )
{
	const float a = 2.51f;
	const float b = 0.03f;
	const float c = 2.43f;
	const float d = 0.59f;
	const float e = 0.14f;
	
	return saturate((x*(a*x+b)) / (x*(c*x+d)+e));
}