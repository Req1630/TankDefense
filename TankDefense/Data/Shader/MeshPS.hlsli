
//定義.
#define MAX_BONE_MATRICES	(255)	// ボーンの最大数.
#define SHADOW_CASCADED_NUM	(4)		// 影の分割数.

// テクスチャ.
Texture2D		g_Texture		: register(t0);
Texture2D		g_NormTexture	: register(t1);
Texture2D		g_ShadowTexture[SHADOW_CASCADED_NUM]	: register(t2);
// サンプラ.
SamplerState	g_SamLinear			: register(s0);
SamplerComparisonState	g_ShadowSamLinear	: register(s1);

//コンスタントバッファ(メッシュごと).
cbuffer per_mesh		: register(b0)
{
	matrix	g_mW;		// ワールド行列.
	matrix	g_mWVP;		// ワールドから射影までの変換行列.
	float4	g_vColor;	// 色.
};
// マテリアル毎に変更.
cbuffer per_material : register(b1)
{
	float4	g_vAmbient;		// アンビエント色(環境色).
	float4	g_vDiffuse;		// ディフューズ色(拡散反射色).
	float4	g_vSpecular;	// スペキュラ色(鏡面反射色).
};
// フレーム毎に変更.
cbuffer per_frame : register(b2)
{
	float4	g_vCamPos;		// カメラ位置(視点位置).
	float4	g_vLightDir;	// ライトの方向ベクトル.
	float4	g_vIntensity;	// ライトの強さ.
};
//ボーンのポーズ行列が入る.
cbuffer per_bones : register(b3)
{
	matrix g_mConstBoneWorld[MAX_BONE_MATRICES];
};
//影の情報が入る.
cbuffer per_shadow : register(b4)
{
	matrix	g_mShadowVP[SHADOW_CASCADED_NUM];
	float4	g_SpritPositon[SHADOW_CASCADED_NUM];
};

// 頂点シェーダーの出力パラメーター.
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