
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
};

// 頂点シェーダー.
VS_OUTPUT VS_Main(float4 Pos : POSITION )
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.Pos	= mul(Pos, g_mWVP);
	output.PosW	= Pos.xyz;
	return output;
}

// ピクセルシェーダー.
PS_OUTPUT PS_Main(VS_OUTPUT input )
{
	float alpha = lerp(0.7f, 0.0f, input.PosW.y / 20.0f);
	float4 color = g_vColor;
	
	color.a *= (1.0f - frac(input.PosW.y * 0.5f + g_vUV.x * 5.0f)) * alpha;
	
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