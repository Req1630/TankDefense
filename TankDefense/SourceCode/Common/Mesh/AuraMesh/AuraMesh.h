#ifndef AURA_MESH_H
#define AURA_MESH_H

#include "..\..\Common.h"

/********************************************
* オーラメッシュ.
**/
class CAuraMesh : public CCommon
{
	// コンスタントバッファのアプリ側の定義.
	//※シェーダー内のコンスタントバッファと一致している必要あり.
	struct C_BUFFER
	{
		D3DXMATRIX	mWVP;		// ワールド,ビュー,プロジェクションの合成変換行列.
		D3DXMATRIX	mW;			// ワールド行列.
		D3DXVECTOR4	vColor;		// カラー(RGBAの型に合わせる).
		D3DXVECTOR4	vUV;		// UV.
	};
	// 頂点の構造体.
	struct VERTEX
	{
		D3DXVECTOR3 Pos;	// 頂点座標.
		D3DXVECTOR2 UV;		// テクスチャ座標.
	};
public:
	CAuraMesh();
	virtual ~CAuraMesh();

	// 初期化.
	HRESULT Init();
	// 解放関数.
	void Release();

	// 更新.
	void Update();
	// 描画.
	void Render();


private:
	// モデルの作成.
	HRESULT InitModel();
	// シェーダ作成.
	HRESULT InitShader();
	// テクスチャ作成.
	HRESULT CreateTexture( const char* texturePath );

private:
	ID3D11Buffer*		m_pVertexBuffer;	// 頂点バッファ.
	ID3D11Buffer*		m_pIndexBuffer;		// インデックスバッファ.

	ID3D11VertexShader*	m_pVertexShader;	// 頂点シェーダー.
	ID3D11PixelShader*	m_pPixelShader;		// ピクセルシェーダー.
	ID3D11InputLayout*	m_pVertexLayout;	// 頂点レイアウト.
	ID3D11Buffer*		m_pConstantBuffer;	// コンスタントバッファ.

	ID3D11ShaderResourceView*	m_pTexture;
	ID3D11SamplerState*			m_pSampleLinear;

	float				m_ScrollTime;
};

#endif	// #ifndef AURA_MESH_H.