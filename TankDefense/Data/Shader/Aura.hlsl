//-----------------------------------------------.
// テクスチャ.
//-----------------------------------------------.
Texture2D g_Texture : register(t0); // 色情報.
//-----------------------------------------------.
// サンプラ.
//-----------------------------------------------.
SamplerState g_SamLinear : register(s0);

struct PS_OUTPUT
{
	float4 Back		: SV_Target0;
	float4 Color	: SV_Target1;
	float4 Normal	: SV_Target2;
	float4 WorldPos	: SV_Target3;
	float4 ZDepth	: SV_Target4;
	float4 Diffuse	: SV_Target5;
};

// グローバル.
cbuffer global
{
	matrix g_mWVP;
	matrix g_mW;
	float4 g_vColor;
	float4 g_vUV;
};

struct VS_OUTPUT
{
	float4	Pos			: SV_Position;
	float3	PosW		: TEXCOORD0;
	float2	Tex			: TEXCOORD1;
};

// 頂点シェーダー.
VS_OUTPUT VS_Main(float4 Pos : POSITION, float2 tex : TEXCOORD )
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.Pos	= mul(Pos, g_mWVP);
	output.PosW	= mul(Pos, g_mW);
	output.Tex = tex;
	if (tex.x != 0.0f || tex.y != 0.0f )
	{
		output.Tex.x += g_vUV.x;
		output.Tex.y += g_vUV.y;
	}
	return output;
}

// ピクセルシェーダー.
PS_OUTPUT PS_Main(VS_OUTPUT input )
{
	float a = g_Texture.Sample(g_SamLinear, input.Tex).r;
	float v = input.PosW.y / 10.0f;
	float alpha = lerp( 1.0f, 0.0f, v );
	float4 color = g_vColor;
	color.a *= alpha * alpha * a;
	clip(color.a);
	
	
	float z = input.Pos.z / input.Pos.w;
	PS_OUTPUT output = (PS_OUTPUT) 0;
	output.Color	= color;
	output.Normal	= float4(0.0f, 0.0f, 0.0f, 0.0f);
	output.WorldPos = float4(0.0f, 0.0f, 0.0f, 0.0f);
	output.ZDepth	= float4(0.0f, 0.0f, 0.0f, 0.0f);
	output.Diffuse	= color;
	output.Back		= float4(color.rgb, 0.0f);
	
	return output;

}