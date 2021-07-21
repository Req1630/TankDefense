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
float OutShadowColor( float4 pos, int index );

//-------------------------------------------------
//	ピクセルシェーダ.
//-------------------------------------------------
PS_OUTPUT PS_Main( VS_OUTPUT input )
{
	float4 texColor		= g_Texture.Sample(g_SamLinear, input.Tex);		// テクスチャ色.
	float4 normColor	= g_NormTexture.Sample(g_SamLinear, input.Tex);	// 法線色.
	float4 finalColor	= texColor;	// 最終色.
	
	////////////////////////////////////////////////.
	// 影の計算.
	float dist = input.Pos.w;
	float shadowColor = 1.0f;
	for( int i = 0; i < SHADOW_CASCADED_NUM; i++ ){
		if( dist < g_SpritPositon[i].x || i == SHADOW_CASCADED_NUM-1 ){
			shadowColor = OutShadowColor( input.ShadowPos[i], i );
			break;
		}
	}
	finalColor.rgb *= shadowColor;
	
	////////////////////////////////////////////////.
	// 法線の取得.
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
	float3 n = normColor.xyz;
	float3 v = normalize(g_vCamPos.xyz - input.PosW.xyz);
	float3 l = normalize(input.LightDir);
	float d = length(input.LightDir);
	
	float3 r = 2.0f * n * dot(n, l) - l;
	float3 ambient	= (g_vAmbient.rgb*0.5f) * (texColor.rgb*0.5f);
	float3 diffuse	= saturate(dot(l, n)) + g_vDiffuse.rgb;
	float3 specular	= pow(saturate(dot(r, v)), 2.0f ) + g_vSpecular.rgb;
	
	////////////////////////////////////////////////.
	// ハーフランバート.
	float lightIntensity = saturate(dot(bumpNormal, input.LightDir)) * g_vIntensity.x;
	lightIntensity = lightIntensity * 0.5f + 0.5f;
	lightIntensity = lightIntensity * lightIntensity;
	
	finalColor.rgb *= saturate(ambient+diffuse+specular) * lightIntensity * g_vColor.xyz;
	finalColor.a *= g_vColor.a;
	
	////////////////////////////////////////////////.
	// 法線をテクスチャ用に変換.
	// -1 ~ 1 を 0 ~ 1 に変換.
	bumpNormal = normalize(bumpNormal)*0.5+0.5f;
	float z = input.Pos.z/input.Pos.w;
	
	////////////////////////////////////////////////.
	// 最終描画.
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
float OutShadowColor( float4 pos, int index )
{
	float2 cord	= pos.xy	/ pos.w;
	float z		= pos.z		/ pos.w;
	float depthColor = g_ShadowTexture[index].SampleCmpLevelZero( g_ShadowSamLinear, cord, z - 0.01f ).r;
	
	return saturate( depthColor + 0.5f );
};