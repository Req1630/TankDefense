#include "RenderTexture.hlsli"

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


float4 PS_Main( VS_OUTPUT input ) : SV_Target
{
	float4 color = g_Texture.Sample(g_SamLinear, input.Tex);
	
	// �u���[���̃T���v������.
	for( int i = 0; i < BLOOM_SAMPLE_NUM; i++ ){
		color.rgb += g_TextureBloom[i].Sample(g_SamLinear, input.Tex).rgb;
	}
	
	// �摜��A�G�t�F�N�g�Ȃǂ�`��.
	float4 t = g_Back.Sample(g_SamLinear, input.Tex);
	if( t.a > 0.0f ) color.rgb = t.rgb;
	
	return color;
}