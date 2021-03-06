#include "MeshPS.hlsli"

struct PS_OUTPUT
{
	float4 Back		: SV_Target0;
	float4 Color	: SV_Target1;
	float4 Normal	: SV_Target2;
	float4 WorldPos	: SV_Target3;
	float4 ZDepth	: SV_Target4;
	float4 Diffuse	: SV_Target5;
};

// 影の計算.
float OutShadowColor( float4 pos, int index, float shadowPow );


static const float DITHER_PATTERN[4][4] = 
	{
		{ 0.00f, 0.32f, 0.08f, 0.40f, },
		{ 0.48f, 0.16f, 0.56f, 0.24f, },
		{ 0.12f, 0.44f, 0.04f, 0.36f, },
		{ 0.60f, 0.28f, 0.52f, 0.20f, },
	};

//-------------------------------------------------
//	ピクセルシェーダ.
//-------------------------------------------------
PS_OUTPUT PS_Main( VS_OUTPUT input )
{
	float4 texColor		= g_Texture.Sample(g_SamLinear, input.Tex);		// テクスチャ色.
	float4 normColor	= g_NormTexture.Sample(g_SamLinear, input.Tex);	// 法線色.
	float4 finalColor	= texColor;	// 最終色.
	finalColor.rgb	*= g_vColor.xyz;
	finalColor.a	*= g_vColor.a;
	
	////////////////////////////////////////////////.
	// ディザ抜き計算.
	////////////////////////////////////////////////.
	/*	
	// 現在輪郭線を描画しているため,
	//	ディザ抜きを行うと真っ黒になるのでコメントアウト.
	const int	pt_x	= (int)fmod(input.Pos.x, 4.0f);
	const int	pt_y	= (int)fmod(input.Pos.y, 4.0f);
	const float	dither	= DITHER_PATTERN[pt_y][pt_x];
	clip( finalColor.a - dither );
	*/
	
	////////////////////////////////////////////////.
	// 影の計算.
	////////////////////////////////////////////////.
	for( int i = 0; i < SHADOW_CASCADED_NUM; i++ ){
		if( input.Pos.w < g_SpritPositon[i].x || i == SHADOW_CASCADED_NUM-1 ){
			finalColor.rgb *= OutShadowColor( input.ShadowPos[i], i, g_vShadowDepth.x );
			break;
		}
	}
	
	////////////////////////////////////////////////.
	// 法線の取得.
	////////////////////////////////////////////////.
	float3 bumpNormal = float3( 0.0f, 0.0f, 0.0f );
	// テクスチャの法線が何もない場合、ポリゴンの法線を取得する、
	//	ある場合は、色をベクトルへ変換する.
	if( length(normColor) == 0.0f ){
		bumpNormal = input.Normal;
	} else {
		normColor.xyz = normalize(normColor * 2.0f - 1.0f).xyz;
		bumpNormal = (normColor.x * input.Tangent) + (normColor.y * input.Binormal) + (normColor.z * input.Normal);
	}
	
	////////////////////////////////////////////////.
	// ライティングの計算.
	////////////////////////////////////////////////.
	float3 n = normalize(bumpNormal.xyz);
	float3 v = normalize(g_vCamPos.xyz - input.PosW.xyz);
	float3 l = normalize(input.LightDir);
	
	float3 NDotL = saturate(dot(n, l));
	float3 NDotV = saturate(dot(n, v));
	
	float3 r = 2.0f * n * NDotL - l;
	float3 ambient	= (g_vAmbient.rgb*0.5f) * (texColor.rgb*0.5f);
	float3 diffuse	= saturate(dot(l, n)) + g_vDiffuse.rgb;
	float3 specular	= pow(saturate(dot(r, v)), 2.0f ) + g_vSpecular.rgb;
	finalColor.rgb	*= saturate(ambient+diffuse+specular);
	/*
	// リムライト.
	float rimPow = 5.0f;
	float3 rimColor	= float3(1.0f,0.0f,0.0f) * pow(1.0f-NDotV), rimPow);
	finalColor.rgb	*= saturate(ambient+diffuse+specular)+rimColor;
	*/
	
	////////////////////////////////////////////////.
	// ハーフランバート.
	////////////////////////////////////////////////.
	float lightIntensity = NDotL * g_vIntensity.x;
	lightIntensity = lightIntensity * 0.5f + 0.5f;
	lightIntensity = lightIntensity * lightIntensity;
	finalColor.rgb	*= lightIntensity;

	// 法線をテクスチャ用に変換.
	// -1 ~ 1 を 0 ~ 1 に変換.
	bumpNormal = n*0.5+0.5f;
	// 深度値を取得.
	float z = input.Pos.z/input.Pos.w;
	
	////////////////////////////////////////////////.
	// 最終描画.
	////////////////////////////////////////////////.
	PS_OUTPUT output = (PS_OUTPUT)0;
	output.Color	= finalColor;
	output.Normal	= float4(bumpNormal, 1.0f);
	output.WorldPos	= float4(input.PosW, 1.0f);
	output.ZDepth	= float4(z, z, z, 1.0f );
	output.Diffuse	= texColor;
	output.Back		= float4(finalColor.rgb, 0.0f);
	
	return output;
}

// 影の計算.
float OutShadowColor( float4 pos, int index, float shadowPow )
{
	float2	cord	= pos.xy	/ pos.w;
	float	z		= pos.z		/ pos.w;
	float	depthColor = g_ShadowTexture[index].SampleCmpLevelZero( g_ShadowSamLinear, cord, z - 0.01f ).r;
	return	saturate( depthColor + shadowPow );
};