#include "..\RenderTexture.hlsli"

#define GBUFFER_BACK		(0)	// �F�e�N�X�`��.
#define GBUFFER_COLOR		(1)	// �F�e�N�X�`��.
#define GBUFFER_NORMAL		(2)	// �@���e�N�X�`��
#define GBUFFER_WORLD_POS	(3)	// ���[���h���W�e�N�X�`��.
#define GBUFFER_Z_DEPTH		(4)	// �[�x�l�e�N�X�`��.
#define GBUFFER_DIFFUSE		(5)	// �f�B�t�[�Y�e�N�X�`��.
#define GBUFFER_MAX			(6)	// �e�N�X�`���z��̍ő�l.

//-----------------------------------------------.
// �e�N�X�`��.
//-----------------------------------------------.
Texture2D g_GBufferColor[GBUFFER_MAX] : register(t0); // �F���.

//-----------------------------------------------.
// �T���v��.
//-----------------------------------------------.
SamplerState g_SamLinear : register(s0);

//-----------------------------------------------.
// �R���X�^���g�o�b�t�@.
//-----------------------------------------------.
cbuffer per_frame : register(b1)
{
	float4 g_SoftKneePram;
	float4 g_Intensity;
};

float4 PS_Main(VS_OUTPUT input) : SV_Target
{
	// �`�悳�ꂽ���f���Ȃǂ̐F�����擾.
	float4 color = float4( g_GBufferColor[GBUFFER_BACK].Sample(g_SamLinear, input.Tex).rgb, 1.0f );
	
	// �e�N�X�`���̖��x�𗎂Ƃ�.
	// --- softknee ---.
	// https://light11.hatenadiary.com/entry/2018/02/15/011155.
	// https://light11.hatenadiary.com/entry/2018/03/15/000022.
	
	// ���x�𗎂Ƃ����F.
	half source = max(color.r, max(color.g, color.b));
	
	half soft = clamp(source - g_SoftKneePram.y, 0, g_SoftKneePram.z);
	soft = soft * soft * g_SoftKneePram.w;
	// �v�Z��.
	//		   (min(knee*2[param.z], max(0, source-threshold+knee[param.y])))2.
	//	soft = ---------------------------------------------------------------.
	//							4 * knee + 0.00001[param.w].
	
	half contribution = max(soft, source-g_SoftKneePram.x);
	contribution /= max(source, 0.0001f);
	// �v�Z��.
	//				 (max(source - threshold[param.x], soft)).
	// contribution = ---------------------------------------.
	//							max(source, 0.0001f).
	float3 finalColor = color.rgb * contribution * g_Intensity.x;
	return float4(finalColor, 1.0f);
}