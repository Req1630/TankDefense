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

//-------------------------------------------------
//	�s�N�Z���V�F�[�_.
//-------------------------------------------------
PS_OUTPUT PS_Main( VS_OUTPUT input )
{
	float4 texColor		= g_Texture.Sample(g_SamLinear, input.Tex);		// �e�N�X�`���F.
	float4 normColor	= g_NormTexture.Sample(g_SamLinear, input.Tex);	// �@���F.
	float4 finalColor	= texColor;	// �ŏI�F.
	
	// ���C�g�̐F.
	float3 lightColor	= float3( 1.0f, 1.0f, 1.0f );
	
	float3 bumpNormal = float3( 0.0f, 0.0f, 0.0f );
	// �e�N�X�`���̖@���������Ȃ��ꍇ�A�|���S���̖@�����擾����A
	//	����ꍇ�́A�F���x�N�g���֕ϊ�����.
	if( length(normColor) == 0.0f ){
		bumpNormal = input.Normal;
	} else {
		normColor.xyz = normalize(normColor * 2.0f - 1.0f).xyz;
		bumpNormal = (normColor.x * input.Tangent) + (normColor.y * input.Binormal) + (normColor.z * input.Normal);
	}
	
	float3 n = normColor.xyz;
	float3 v = normalize(g_vCamPos.xyz - input.PosW.xyz);
	float3 l = input.LightDir;
	float d = length(l);
	l = normalize(l);
	float3 r = 2.0f * n * dot(n, l) - l;
	float a = saturate( 1.0f / (1.0f + 0.1f * d + 0.1f * d * d));
	
	float3 ambient	= (g_vAmbient.rgb*0.5f) * (finalColor.rgb*0.5f);
	float3 diffuse	= saturate(dot(l, n)) + g_vDiffuse.rgb;
	float3 specular	= pow(saturate(dot(r, v)), 2.0f ) + g_vSpecular.rgb;
		
	// �n�[�t�����o�[�g.
	float lightIntensity = saturate(dot(bumpNormal, input.LightDir)) * g_vIntensity.x;
	lightIntensity = lightIntensity * 0.5f + 0.5f;
	lightIntensity = lightIntensity * lightIntensity;
	
	finalColor.rgb *= saturate(ambient+diffuse+specular) * lightIntensity * g_vColor.xyz;
//	finalColor.rgb = g_vDiffuse;
//	finalColor.xyz *= saturate(lightColor.xyz * lightIntensity) * g_vColor.xyz;
	finalColor.a *= g_vColor.a;
	
	// -1 ~ 1 �� 0 ~ 1 �ɕϊ�.
	bumpNormal = normalize(bumpNormal)*0.5+0.5f;
	float z = input.Pos.z/input.Pos.w;
	
	PS_OUTPUT output = (PS_OUTPUT)0;
	output.Color	= finalColor;
	output.Normal	= float4(bumpNormal, 1.0f);
	output.WorldPos	= float4(input.PosW, 1.0f);
	output.ZDepth	= float4(z, z, z, 1.0f );
	output.Diffuse	= texColor;
	output.Back		= float4(finalColor.rgb, 0.0f);
	
	return output;
}