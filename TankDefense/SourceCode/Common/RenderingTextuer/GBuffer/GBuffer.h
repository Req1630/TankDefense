#ifndef G_BUFFER_RENDER_H
#define G_BUFFER_RENDER_H

#include "..\RenderingTextuer.h"

class CCascadedShadowMap;

/*********************************************
*	G-Bufferの描画クラス.
**/
class CGBufferRender : public CRenderTexture
{
public:
	// G-Bufferの種類.
	enum enGBufferNo
	{
		EGBufferNo_BACK,		// 色情報.
		EGBufferNo_COLOR,		// 色情報.
		EGBufferNo_NORMAL,		// 法線情報.
		EGBufferNo_WORLDPOS,	// ワールド座標.
		EGBufferNo_Z_DEPTH,		// 深度値情報.
		EGBufferNo_DIFFUSE,		// 色情報.

		EGBufferNo_MAX,	// 最大値.

	} typedef EGBufferNo;
public:
	CGBufferRender();
	virtual ~CGBufferRender();

	// 初期化.
	virtual HRESULT Init( ID3D11DeviceContext* pContext11 ) override;

	// バッファの設定.
	virtual void SetBuffer() override;
	// シェーダーリソースビューの数を取得.
	virtual int GetSRVCount() override { return EGBufferNo_MAX; }

protected:
	// テクスチャの初期化.
	virtual HRESULT InitBufferTex() override;

private:
	// 更新.
	void Update();

	// フレーム時渡す値の更新.
	void UpdateFrame();
	// 影情報を渡す値の更新.
	void UpdateShadow();

	// シャドウ用サンプラーの作成.
	HRESULT InitShadowSampler();

	// ディザーテクスチャ作成.
	HRESULT CreateDitherTexture();

private:
	ID3D11Buffer*				m_pConstantBufferFrame;		// コンスタントバッファ.
	ID3D11Buffer*				m_pConstantBufferShadow;	// コンスタントバッファ.
	ID3D11SamplerState*			m_pShadowMapSampler;		// シャドウマップ用サンプラー.
	CCascadedShadowMap*			m_pShadowMap;				// シングルトン(外部から取得).
	ID3D11ShaderResourceView*	m_pDitherTexture;			// 通常テクスチャ.
};

#endif	// #ifndef G_BUFFER_RENDER_H.