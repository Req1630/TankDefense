#include "RenderTexture.hlsli"
#include "FXAA.hlsl"	// �A���`�G�C���A�X���g�p����ׂɕK�v.

#define BLOOM_SAMPLE_NUM (5)

//-----------------------------------------------.
// �e�N�X�`��.
//-----------------------------------------------.
Texture2D g_Texture							: register(t0);	// �F���.
Texture2D g_Back							: register(t1);	// �F���.
Texture2D g_TextureBloom[BLOOM_SAMPLE_NUM]	: register(t2);	// �u���[�����.

//-----------------------------------------------.
// �T���v��.
//-----------------------------------------------.
SamplerState g_SamLinear : register(s0);

// �g�[���}�b�v.
// https://knarkowicz.wordpress.com/2016/01/06/aces-filmic-tone-mapping-curve/
float3 ACESFilm(float3 x);

float4 PS_Main( VS_OUTPUT input ) : SV_Target
{
	// �`�悳�ꂽ���f���Ȃǂ̐F�����擾.
	FxaaTex tex = { g_SamLinear, g_Texture };
	float4 color = float4(FxaaPixelShader(input.Tex, tex, float2(g_vPixelSize.x, g_vPixelSize.y)), 1.0f);
	
	// �u���[���̃T���v������.
	for( int i = 0; i < BLOOM_SAMPLE_NUM; i++ ){
		color.rgb += g_TextureBloom[i].Sample(g_SamLinear, input.Tex).rgb;
	}
	
	// �摜��A�G�t�F�N�g�Ȃǂ�`��.
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