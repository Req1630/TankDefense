#ifndef RENDERING_TEXTURE_MANAGER_H
#define RENDERING_TEXTURE_MANAGER_H

#include "..\RenderingTextuer.h"
#include <functional>

class CGBufferRender;
class CLightingRender;
class COutLineRender;
class CBloomRender;
class CCascadedShadowMap;

/********************************************
*	レンダリングテクスチャの管理クラス.
**/
class CRenderingTexterManager : public CRenderTexture
{
	// 描画フラグ.
	enum enRenderFlag : unsigned int
	{
		ERenderFlag_None,

		ERenderFlag_Light	= 1 << 0,
		ERenderFlag_OutLine	= 1 << 1,
		ERenderFlag_Bloom	= 1 << 2,

		ERenderFlag_OllOn	= 0xffffffff,
		ERenderFlag_Max		= ERenderFlag_OllOn,
	} typedef ERenderFlag;
public:
	CRenderingTexterManager();
	~CRenderingTexterManager();

	// 初期化.
	virtual HRESULT Init( ID3D11DeviceContext* pContext11 ) override;
	// 解放.
	virtual void Release() override;

	// 描画関数.
	void Render( 
		std::function<void()>& mdoelRender, 
		std::function<void()>& sprite3DRender );

	// サイズ変更.
	HRESULT ResizeTexture();

private:
	// 各画像を合成する.
	void SynthesizeTexture( ID3D11ShaderResourceView* pSRV );
	// 最終描画.
	void LastRender( ID3D11ShaderResourceView* pSRV );

	// バッファの設定.
	virtual void SetBuffer() override;
	// テクスチャの初期化.
	virtual HRESULT InitBufferTex() override;
	// シェーダーリソースビューの数を取得.
	virtual int GetSRVCount()  override { return 0; }

	// ピクセルシェーダーの初期化.
	HRESULT InitLastPixelShader();

private:
	ID3D11PixelShader*					m_pLastPixelShader;	// ピクセルシェーダー.
	std::unique_ptr<CGBufferRender>		m_pGBuffer;
	std::unique_ptr<CLightingRender>	m_pLighting;
	std::unique_ptr<COutLineRender>		m_pOutLine;
	std::unique_ptr<CBloomRender>		m_pBloom;
	CCascadedShadowMap*					m_pShadowMap;	// シングルトン用.
	unsigned int						m_RenderFlag;
};

#endif	// #ifndef RENDERING_TEXTURE_MANAGER_H.