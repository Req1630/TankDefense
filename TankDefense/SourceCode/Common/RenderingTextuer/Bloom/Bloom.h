#ifndef BLOOM_RENDER_H
#define BLOOM_RENDER_H

#include "..\RenderingTextuer.h"

class CDownSamplingRender;

/***********************************************
*	ブルーム描画クラス.
*	川瀬式グレアエフェクトを使用.
*	参考サイト(http://project-asura.com/program/d3d11/d3d11_010.html).
**/
class CBloomRender : public CRenderTexture
{
	struct C_BUFFER_PER_FRAME
	{
		static const int NUM_OFFSET			= 16;
		static const int HALF_NUM_OFFSET	= 8;

		D3DXVECTOR4 Offset[NUM_OFFSET];	// オフセット値.
	};
public:
	CBloomRender();
	virtual ~CBloomRender();

	// 初期化.
	virtual HRESULT Init( ID3D11DeviceContext* pContext11 ) override;

	// 描画.
	void Render( const int& srvCount, const std::vector<ID3D11ShaderResourceView*>& srvList );

	// バッファの設定.
	virtual void SetBuffer() override;
	// シェーダーリソースビューの数を取得.
	virtual int GetSRVCount() override { return m_pShaderResourceViewList.size(); }

protected:
	// テクスチャの初期化.
	virtual HRESULT InitBufferTex() override;

private:
	// 頂点バッファーの作成.
	virtual HRESULT InitVertexBuffer() override;
	// コンスタントバッファの作成.
	virtual HRESULT InitConstantBuffer() override;

	// 描画.
	void Render( 
		const int& index,
		ID3D11ShaderResourceView* pSrv );

	// ガウスの重みを計算.
	float GaussianDistriBution( const D3DXVECTOR2& pos, const float& rho );
	// ブラーのパラメータを計算.
	C_BUFFER_PER_FRAME CalcBlurParam( 
		const D3DXVECTOR2& size, 
		const D3DXVECTOR2& dir,
		const float& deviation,
		const float& multiply );

private:
	std::unique_ptr<CDownSamplingRender>	m_pDownSampling;		// ダウンサンプル.
	std::vector<ID3D11Buffer*>				m_pConstantBuffersInit;	// コンスタントバッファ.
	std::vector<ID3D11Buffer*>				m_pVertexBuffers;		// 頂点バッファ.
	ID3D11Buffer*							m_pConstantBufferFrame;	// コンスタントバッファ.
};

#endif	// #ifndef BLOOM_RENDER_H.