#include "RenderTexture.hlsli"

//-----------------------------------------------.
// �e�N�X�`��.
//-----------------------------------------------.
Texture2D g_Texture : register(t0); // �F���.

//-----------------------------------------------.
// �T���v��.
//-----------------------------------------------.
SamplerState g_SamLinear : register(s0);


float4 PS_Main(VS_OUTPUT input) : SV_Target
{
	return g_Texture.Sample( g_SamLinear, input.Tex );
}