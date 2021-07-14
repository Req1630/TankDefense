#include "RenderingTextuerManager.h"
#include "..\GBuffer\GBuffer.h"
#include "..\Lighting\Lighting.h"
#include "..\OutLine\OutLine.h"
#include "..\Bloom\Bloom.h"
#include "..\..\Shader\Shader.h"
#include "..\..\..\Utility\ImGuiManager\ImGuiManager.h"
#include "..\..\..\Utility\BitFlagManager\BitFlagManager.h"

namespace
{
	constexpr char	SHADER_NAME[]		= "Data\\Shader\\RenderTexture\\RenderTexture_PS.hlsl";
	constexpr char	LAST_SHADER_NAME[]	= "Data\\Shader\\RenderTexture\\RenderTexture_PS.hlsl";
	constexpr char	SHADER_ENTRY_NAME[]	= "PS_Main";
	constexpr int	BUFFER_COUNT_MAX	= 1;
};

CRenderingTexterManager::CRenderingTexterManager()
	: m_pLastPixelShader	( nullptr )
	, m_pGBuffer			( nullptr )
	, m_pLighting			( nullptr )
	, m_pOutLine			( nullptr )
	, m_pBloom				( nullptr )
	, m_RenderFlag			( 0 )
{
	m_pGBuffer	= std::make_unique<CGBufferRender>();
	m_pLighting	= std::make_unique<CLightingRender>();
	m_pOutLine	= std::make_unique<COutLineRender>();
	m_pBloom	= std::make_unique<CBloomRender>();
	bit::OnBitFlag( &m_RenderFlag, ERenderFlag_OllOn );
}

CRenderingTexterManager::~CRenderingTexterManager()
{
}

// 初期化.
HRESULT CRenderingTexterManager::Init( ID3D11DeviceContext* pContext11 )
{
	if( FAILED( InitBase( pContext11 ) ))								return E_FAIL;
	if( FAILED( InitPixelShader( SHADER_NAME, SHADER_ENTRY_NAME )) )	return E_FAIL;
	if( FAILED( InitLastPixelShader() ))								return E_FAIL;
	if( FAILED( InitBufferTex() ))										return E_FAIL;

	if( FAILED( m_pGBuffer->Init( pContext11 ) ))	return E_FAIL;
	if( FAILED( m_pLighting->Init( pContext11 ) ))	return E_FAIL;
	if( FAILED( m_pOutLine->Init( pContext11 ) ))	return E_FAIL;
	if( FAILED( m_pBloom->Init( pContext11 ) ))		return E_FAIL;

	return S_OK;
}

// 解放.
void CRenderingTexterManager::Release()
{
	for( auto& r : m_pRenderTargetViewList )	SAFE_RELEASE( r );
	for( auto& s : m_pShaderResourceViewList )	SAFE_RELEASE( s );
	for( auto& t : m_pTexture2DList )			SAFE_RELEASE( t );

	SAFE_RELEASE( m_pLastPixelShader );
	SAFE_RELEASE( m_pVertexShader );
	SAFE_RELEASE( m_pPixelShader );
	SAFE_RELEASE( m_pVertexLayout );
	SAFE_RELEASE( m_pConstantBufferInit );
	SAFE_RELEASE( m_pVertexBuffer );

	m_pContext11 = nullptr;
	m_pDevice11 = nullptr;
}

// 描画関数.
void CRenderingTexterManager::Render( std::function<void()>& modelRender )
{
	m_pGBuffer->SetBuffer();

	modelRender();	// モデルの描画.

	// ここから背面を描画させない.
	SetRasterizerState( ERS_STATE::Back );

	m_pContext11->IASetInputLayout( m_pVertexLayout );
	m_pContext11->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );

	ID3D11ShaderResourceView* pLastSRV = m_pGBuffer->GetShaderResourceViewList()[m_pGBuffer->EGBufferNo_COLOR];

	// ライトの描画.
	if( bit::IsBitFlag( m_RenderFlag, ERenderFlag_Light ) == true ){
		m_pLighting->Render( m_pGBuffer->GetSRVCount(), m_pGBuffer->GetShaderResourceViewList() );
		pLastSRV = m_pLighting->GetShaderResourceViewList()[0];
	}
	
	// ブルームの描画.
	if( bit::IsBitFlag( m_RenderFlag, ERenderFlag_Bloom ) == true ){
		std::vector<ID3D11ShaderResourceView*> srvList( 1, pLastSRV );
		m_pBloom->Render( 1, srvList );
	}

	// 輪郭線の描画.
	if( bit::IsBitFlag( m_RenderFlag, ERenderFlag_OutLine ) == true ){
		std::vector<ID3D11ShaderResourceView*> srvList = m_pGBuffer->GetShaderResourceViewList();
		srvList[m_pGBuffer->EGBufferNo_COLOR] = pLastSRV;
		m_pOutLine->Render( m_pGBuffer->GetSRVCount(), srvList );
		pLastSRV = m_pOutLine->GetShaderResourceViewList()[0];
	}

	SynthesizeTexture( pLastSRV );
	pLastSRV = m_pShaderResourceViewList[0];

	LastRender( pLastSRV );

	// 背面非表示を元に戻す.
	SetRasterizerState( ERS_STATE::None );

	CImGuiManager::PushRenderProc( 
		[&]()
		{
			ImGui::Begin( "RenderingTexture" );
			const float size = ImGui::GetWindowWidth() / static_cast<float>(m_WndWidth)*0.9f;
			const ImVec2 imageSize = { static_cast<float>(m_WndWidth)*size, static_cast<float>(m_WndHeight)*size };
			if( ImGui::TreeNode("G-Buffer") ){
				for( int i = 0; i <  m_pGBuffer->GetSRVCount(); i++ ){
					ImGui::Image( m_pGBuffer->GetShaderResourceViewList()[i], imageSize );
				}
				ImGui::TreePop();
			}
			if( ImGui::TreeNode("Lighting") ){
				ImGui::Image( m_pLighting->GetShaderResourceViewList()[0], imageSize );
				ImGui::TreePop();
			}
			if( ImGui::TreeNode("OutLine") ){
				for( int i = 0; i <  m_pOutLine->GetSRVCount(); i++ ){
					ImGui::Image( m_pOutLine->GetShaderResourceViewList()[i], imageSize );
				}
				ImGui::TreePop();
			}
			if( ImGui::TreeNode("Bloom") ){
				for( int i = 0; i <  m_pBloom->GetSRVCount(); i++ ){
					ImGui::Image( m_pBloom->GetShaderResourceViewList()[i], imageSize );
				}
				ImGui::TreePop();
			}
			ImGui::End();

			ImGui::Begin( "RenderFlag" );
			ImGui::CheckboxFlags( u8"ライト描画", &m_RenderFlag, ERenderFlag_Light );
			ImGui::CheckboxFlags( u8"輪郭線描画", &m_RenderFlag, ERenderFlag_OutLine );
			ImGui::CheckboxFlags( u8"Bloom描画", &m_RenderFlag, ERenderFlag_Bloom );
			ImGui::End();
		});
}

// サイズ変更.
HRESULT CRenderingTexterManager::ResizeTexture()
{
	if( m_WndWidth	== CDirectX11::GetWndWidth() && 
		m_WndHeight	== CDirectX11::GetWndHeight() ) return S_OK;

	CRenderTexture* pTex[] =
		{
			this,
			m_pGBuffer.get(),
			m_pLighting.get(),
			m_pOutLine.get(),
			m_pBloom.get(),
		};
	for( auto& t : pTex ){
		if( FAILED( t->Resize() ) ) return E_FAIL;
	}
	return S_OK;
}

// 各画像を合成する.
void CRenderingTexterManager::SynthesizeTexture( ID3D11ShaderResourceView* pSRV )
{
	SetBuffer();

	// 使用するシェーダのセット.
	m_pContext11->VSSetShader( m_pVertexShader, nullptr, 0 );	// 頂点シェーダ.
	m_pContext11->PSSetShader( m_pPixelShader, nullptr, 0 );	// ピクセルシェーダ.
	m_pContext11->PSSetSamplers( 0, 1, &m_pSampleLinear );		// サンプラのセット.

	m_pContext11->VSSetConstantBuffers( 0, 1, &m_pConstantBufferInit );	// 頂点シェーダ.
	m_pContext11->PSSetConstantBuffers( 0, 1, &m_pConstantBufferInit );	// ピクセルシェーダー.

	// メインテクスチャを設定.
	m_pContext11->PSSetShaderResources( 0, 1, &pSRV );
	m_pContext11->PSSetShaderResources( 1, 1, &m_pGBuffer->GetShaderResourceViewList()[0] );

	if( bit::IsBitFlag( m_RenderFlag, ERenderFlag_Bloom ) == true ){
		// Bloom用のテクスチャを設定.
		m_pContext11->PSSetShaderResources( 2, m_pBloom->GetSRVCount(), &m_pBloom->GetShaderResourceViewList()[0] );
	}

	m_pContext11->Draw( 4, 0 );

	std::vector<ID3D11ShaderResourceView*> resetSrvList(m_pBloom->GetSRVCount());
	m_pContext11->PSSetShaderResources( 1, m_pBloom->GetSRVCount(), &resetSrvList[0] );
	
}

// 最終描画.
void CRenderingTexterManager::LastRender( ID3D11ShaderResourceView* pSRV )
{
	CDirectX11::SetBackBuffer();

	m_pContext11->VSSetShader( m_pVertexShader, nullptr, 0 );	// 頂点シェーダ.
	m_pContext11->PSSetShader( m_pLastPixelShader, nullptr, 0 );	// ピクセルシェーダ.
	m_pContext11->PSSetSamplers( 0, 1, &m_pSampleLinear );		// サンプラのセット.

	// メインテクスチャを設定.
	m_pContext11->PSSetShaderResources( 0, 1, &pSRV );

	m_pContext11->Draw( 4, 0 );

	std::vector<ID3D11ShaderResourceView*> resetSrvList( BUFFER_COUNT_MAX );
	m_pContext11->PSSetShaderResources( 0, BUFFER_COUNT_MAX, &resetSrvList[0] );
}

// バッファの設定.
void CRenderingTexterManager::SetBuffer()
{
	CRenderTexture::SetBuffer( BUFFER_COUNT_MAX );
}

// テクスチャの初期化.
HRESULT CRenderingTexterManager::InitBufferTex()
{
	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width				= m_WndWidth ;						// 幅.
	texDesc.Height				= m_WndHeight;						// 高さ.
	texDesc.MipLevels			= 1;								// ミップマップレベル:1.
	texDesc.ArraySize			= 1;								// 配列数:1.
	texDesc.Format				= DXGI_FORMAT_R16G16B16A16_FLOAT;	// 32ビットフォーマット.
	texDesc.SampleDesc.Count	= 1;								// マルチサンプルの数.
	texDesc.SampleDesc.Quality	= 0;								// マルチサンプルのクオリティ.
	texDesc.Usage				= D3D11_USAGE_DEFAULT;				// 使用方法:デフォルト.
	texDesc.BindFlags			= D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;	// レンダーターゲット、シェーダーリソース.
	texDesc.CPUAccessFlags		= 0;								// CPUからはアクセスしない.
	texDesc.MiscFlags			= 0;								// その他の設定なし.

	m_pRenderTargetViewList.resize( BUFFER_COUNT_MAX );
	m_pShaderResourceViewList.resize( BUFFER_COUNT_MAX );
	m_pTexture2DList.resize( BUFFER_COUNT_MAX );

	for( int i = 0; i < BUFFER_COUNT_MAX; i++ ){
		if( FAILED( CreateBufferTex(
			texDesc,
			&m_pRenderTargetViewList[i],
			&m_pShaderResourceViewList[i],
			&m_pTexture2DList[i] ))) return E_FAIL;
	}

	return S_OK;
}

// ピクセルシェーダーの初期化.
HRESULT CRenderingTexterManager::InitLastPixelShader()
{
	ID3DBlob* pCompiledShader = nullptr;
	ID3DBlob* pErrors = nullptr;
	UINT uCompileFlag = 0;
#ifdef _DEBUG
	uCompileFlag =
		D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION;
#endif	// #ifdef _DEBUG

	// HLSLからピクセルシェーダーのブロブを作成.
	if( FAILED(
		shader::InitShader(
		LAST_SHADER_NAME,	// シェーダーファイル名.
		SHADER_ENTRY_NAME,	// シェーダーエントリーポイント.
		"ps_5_0",			// シェーダーモデル.
		uCompileFlag,		// シェーダーコンパイルフラグ.
		&pCompiledShader,	// ブロブを格納するメモリへのポインタ.
		&pErrors ))) {		// エラーと警告一覧を格納するメモリへのポインタ.
		ERROR_MESSAGE( shader::GetBlobErrorMsg( pErrors ) );
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);

	// 上記で作成したブロブから「ピクセルシェーダー」を作成.
	if( FAILED( shader::CreatePixelShader(
		m_pDevice11,
		pCompiledShader, 
		&m_pLastPixelShader ))) {
		ERROR_MESSAGE("ピクセルシェーダー作成 : 失敗");
		return E_FAIL;
	}

	return S_OK;
}