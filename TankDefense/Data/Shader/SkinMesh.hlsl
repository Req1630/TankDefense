/***************************************************************************************************
*	SkinMeshCode Version 2.00
*	LastUpdate	: 2019/10/09
*	Auther		: an.
**/

#include "MeshPS.hlsli"

//�X�L�j���O��̒��_�E�@��������.
struct Skin
{
	float4 Pos;
	float4 Norm;
	float4 Tangent;
	float4 Binorm;
};
//�o�[�e�b�N�X�o�b�t�@�[�̓���.
struct VSSkinIn
{
	float3 Pos		: POSITION;		//�ʒu.  
	float3 Norm		: NORMAL;		//���_�@��.
	float2 Tex		: TEXCOORD;		//�e�N�X�`���[���W.
	float3 Tangent	: TANGENT;
	float3 Binorm	: BINORMAL;
	uint4  Bones	: BONE_INDEX;	//�{�[���̃C���f�b�N�X.
	float4 Weights	: BONE_WEIGHT;	//�{�[���̏d��.
};

//�w�肵���ԍ��̃{�[���̃|�[�Y�s���Ԃ�.
//�T�u�֐��i�o�[�e�b�N�X�V�F�[�_�[�Ŏg�p�j.
matrix FetchBoneMatrix( uint iBone )
{
	return g_mConstBoneWorld[iBone];
}


//���_���X�L�j���O�i�{�[���ɂ��ړ��j����.
//�T�u�֐��i�o�[�e�b�N�X�V�F�[�_�[�Ŏg�p�j.
Skin SkinVert( VSSkinIn Input )
{
	Skin Output = (Skin)0;

	float4 Pos		= float4(Input.Pos,1);
	float4 Norm		= float4(Input.Norm,0);
	float4 Tangent	= float4(Input.Tangent,0);
	float4 Binorm	= float4(Input.Binorm,0);
	//�{�[��0.
	uint iBone		=Input.Bones.x;
	float fWeight	= Input.Weights.x;
	matrix m	=  FetchBoneMatrix( iBone );
	Output.Pos		+= fWeight * mul( Pos, m );
	Output.Norm		+= fWeight * mul( Norm, m );
	Output.Tangent	+= fWeight * mul( Tangent, m );
	Output.Binorm	+= fWeight * mul( Binorm, m );
	//�{�[��1.
	iBone	= Input.Bones.y;
	fWeight	= Input.Weights.y;
	m		= FetchBoneMatrix( iBone );
	Output.Pos		+= fWeight * mul( Pos, m );
	Output.Norm		+= fWeight * mul( Norm, m );
	Output.Tangent	+= fWeight * mul( Tangent, m );
	Output.Binorm	+= fWeight * mul( Binorm, m );
	//�{�[��2.
	iBone	= Input.Bones.z;
	fWeight	= Input.Weights.z;
	m		= FetchBoneMatrix( iBone );
	Output.Pos		+= fWeight * mul( Pos, m );
	Output.Norm		+= fWeight * mul( Norm, m );
	Output.Tangent	+= fWeight * mul( Tangent, m );
	Output.Binorm	+= fWeight * mul( Binorm, m );
	//�{�[��3.
	iBone	= Input.Bones.w;
	fWeight	= Input.Weights.w;
	m		= FetchBoneMatrix( iBone );
	Output.Pos		+= fWeight * mul( Pos, m );
	Output.Norm		+= fWeight * mul( Norm, m );
	Output.Tangent	+= fWeight * mul( Tangent, m );
	Output.Binorm	+= fWeight * mul( Binorm, m );

	return Output;
}

// �o�[�e�b�N�X�V�F�[�_.
VS_OUTPUT VS_Main( VSSkinIn input )
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	Skin vSkinned = SkinVert(input);

	output.Pos		= mul(vSkinned.Pos, g_mWVP);
	output.PosW		= mul(vSkinned.Pos, g_mW).xyz;
	output.Normal	= normalize(mul(vSkinned.Norm,		g_mW)).xyz;
	output.Tangent	= normalize(mul(vSkinned.Tangent,	g_mW)).xyz;
	output.Binormal	= normalize(mul(vSkinned.Binorm,	g_mW)).xyz;
	output.LightDir	= g_vLightDir.xyz;
	output.EyeVector	= normalize( g_vCamPos.xyz - output.PosW.xyz );
	output.Tex			= input.Tex;
	
	for( int i = 0; i < SHADOW_CASCADED_NUM; i++ ){
		matrix shadowMat = mul(g_mW, g_mShadowVP[i]);
		output.ShadowPos[i] = mul(vSkinned.Pos, shadowMat);
	}

	return output;
}
