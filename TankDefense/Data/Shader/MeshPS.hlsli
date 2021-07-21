
//��`.
#define MAX_BONE_MATRICES	(255)	// �{�[���̍ő吔.
#define SHADOW_CASCADED_NUM	(4)		// �e�̕�����.

// �e�N�X�`��.
Texture2D		g_Texture		: register(t0);
Texture2D		g_NormTexture	: register(t1);
Texture2D		g_ShadowTexture[SHADOW_CASCADED_NUM]	: register(t2);
// �T���v��.
SamplerState	g_SamLinear			: register(s0);
SamplerComparisonState	g_ShadowSamLinear	: register(s1);

//�R���X�^���g�o�b�t�@(���b�V������).
cbuffer per_mesh		: register(b0)
{
	matrix	g_mW;		// ���[���h�s��.
	matrix	g_mWVP;		// ���[���h����ˉe�܂ł̕ϊ��s��.
	float4	g_vColor;	// �F.
};
// �}�e���A�����ɕύX.
cbuffer per_material : register(b1)
{
	float4	g_vAmbient;		// �A���r�G���g�F(���F).
	float4	g_vDiffuse;		// �f�B�t���[�Y�F(�g�U���ːF).
	float4	g_vSpecular;	// �X�y�L�����F(���ʔ��ːF).
};
// �t���[�����ɕύX.
cbuffer per_frame : register(b2)
{
	float4	g_vCamPos;		// �J�����ʒu(���_�ʒu).
	float4	g_vLightDir;	// ���C�g�̕����x�N�g��.
	float4	g_vIntensity;	// ���C�g�̋���.
};
//�{�[���̃|�[�Y�s�񂪓���.
cbuffer per_bones : register(b3)
{
	matrix g_mConstBoneWorld[MAX_BONE_MATRICES];
};
//�e�̏�񂪓���.
cbuffer per_shadow : register(b4)
{
	matrix	g_mShadowVP[SHADOW_CASCADED_NUM];
	float4	g_SpritPositon[SHADOW_CASCADED_NUM];
};

// ���_�V�F�[�_�[�̏o�̓p�����[�^�[.
struct VS_OUTPUT
{
	float4	Pos			: SV_Position;
	float3	Binormal	: BINORMAL0;
	float3	Tangent		: TANGENT0;
	float3	PosW		: TEXCOORD0;
	float3	LightDir	: TEXCOORD1;
	float3	Normal		: TEXCOORD2;
	float3	EyeVector	: TEXCOORD3;
	float2	Tex			: TEXCOORD4;
	float4	ShadowPos[SHADOW_CASCADED_NUM]	: TEXCOORD5;
};