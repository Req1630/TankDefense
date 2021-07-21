//-------------------------------------------------.
// コンスタントバッファ.
//-------------------------------------------------.
// メッシュ毎.
cbuffer per_mesh : register(b0)
{
	matrix g_LightWVP; // ライトのワールド、ビュー、プロジェクション.
};

//-------------------------------------------------.
// 構造体.
//-------------------------------------------------.
// 頂点出力用.
struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
};
// 頂点入力用(スタティックメッシュ).
struct VS_INPUT
{
	float3 Pos : POSITION;
	float3 Norm : NORMAL;
	float2 Tex : TEXCOORD;
	float3 Tangent : TANGENT;
	float3 Binorm : BINORMAL;
};
// 頂点入力用(スキンメッシュ).
struct VSSkinIn
{
	float3 Pos : POSITION; //位置.  
	float3 Norm : NORMAL; //頂点法線.
	float2 Tex : TEXCOORD; //テクスチャー座標.
	float3 Tangent : TANGENT;
	float3 Binorm : BINORMAL;
	uint4 Bones : BONE_INDEX; //ボーンのインデックス.
	float4 Weights : BONE_WEIGHT; //ボーンの重み.
};
// スキニング後の頂点・法線が入る.
struct Skin
{
	float4 Pos;
	float4 Norm;
	float4 Tangent;
	float4 Binorm;
};

// スタティックメッシュのメイン関数.
VS_OUTPUT VS_Main(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT) 0;
	
	// ライト行列と座標を計算.
	output.Pos = mul(float4(input.Pos, 1.0f), g_LightWVP);
	
	return output;
}

//-----------------------------------------.
// スキンメッシュ.
//-----------------------------------------.
//定義.
#define MAX_BONE_MATRICES (255)	// ボーン最大数.
// ボーンのポーズ行列が入る.
cbuffer per_bones : register(b1)
{
	matrix g_mConstBoneWorld[MAX_BONE_MATRICES];
};
//指定した番号のボーンのポーズ行列を返す.
//サブ関数（バーテックスシェーダーで使用）.
matrix FetchBoneMatrix(uint iBone)
{
	return g_mConstBoneWorld[iBone];
}
//頂点をスキニング（ボーンにより移動）する.
//サブ関数（バーテックスシェーダーで使用）.
Skin SkinVert(VSSkinIn Input)
{
	Skin Output = (Skin) 0;

	float4 Pos = float4(Input.Pos, 1);
	float4 Norm = float4(Input.Norm, 0);
	float4 Tangent = float4(Input.Tangent, 0);
	float4 Binorm = float4(Input.Binorm, 0);
	//ボーン0.
	uint iBone = Input.Bones.x;
	float fWeight = Input.Weights.x;
	matrix m = FetchBoneMatrix(iBone);
	Output.Pos += fWeight * mul(Pos, m);
	Output.Norm += fWeight * mul(Norm, m);
	Output.Tangent += fWeight * mul(Tangent, m);
	Output.Binorm += fWeight * mul(Binorm, m);
	//ボーン1.
	iBone = Input.Bones.y;
	fWeight = Input.Weights.y;
	m = FetchBoneMatrix(iBone);
	Output.Pos += fWeight * mul(Pos, m);
	Output.Norm += fWeight * mul(Norm, m);
	Output.Tangent += fWeight * mul(Tangent, m);
	Output.Binorm += fWeight * mul(Binorm, m);
	//ボーン2.
	iBone = Input.Bones.z;
	fWeight = Input.Weights.z;
	m = FetchBoneMatrix(iBone);
	Output.Pos += fWeight * mul(Pos, m);
	Output.Norm += fWeight * mul(Norm, m);
	Output.Tangent += fWeight * mul(Tangent, m);
	Output.Binorm += fWeight * mul(Binorm, m);
	//ボーン3.
	iBone = Input.Bones.w;
	fWeight = Input.Weights.w;
	m = FetchBoneMatrix(iBone);
	Output.Pos += fWeight * mul(Pos, m);
	Output.Norm += fWeight * mul(Norm, m);
	Output.Tangent += fWeight * mul(Tangent, m);
	Output.Binorm += fWeight * mul(Binorm, m);

	return Output;
}

// バーテックスシェーダ.
VS_OUTPUT VS_SkinMain(VSSkinIn input)
{
	VS_OUTPUT output = (VS_OUTPUT) 0;
	Skin vSkinned = SkinVert(input);

	// ライト行列と座標を計算.
	output.Pos = mul(vSkinned.Pos, g_LightWVP);
	
	return output;
}

// ピクセルシェーダー.
float4 PS_Main(VS_OUTPUT input) : SV_Target
{
	return input.Pos.z / input.Pos.w;
}