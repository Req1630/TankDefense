#include "Bloom.h"
#include "..\..\Shader\Shader.h"
#include "..\..\..\Object\CameraBase\CameraManager\CameraManager.h"
#include "..\..\..\Object\LightBase\LightManager\LightManager.h"
#include "..\DownSampling\DownSampling.h"

namespace
{
	const char*	SHADER_NAME			= "Data\\Shader\\RenderTexture\\Bloom\\Bloom.hlsl";	// ピクセルシェーダー名.
	const char*	SHADER_ENTRY_NAME	= "PS_Main";		// ピクセルシェーダーエントリー名.
	const int	BLUR_SAMPLE_NUM		= 5;				// ぼかしのサンプル数.
	const int	BUFFER_COUNT_MAX	= BLUR_SAMPLE_NUM;	// バッファーの最大数.
};

CBloomRender::CBloomRender()
	: m_pDownSampling	( nullptr )
{
	m_pDownSampling = std::make_unique<CDownSamplingRender>();
}

CBloomRender::~CBloomRender()
{
}

//------------------------------------.
// 初期化.
//------------------------------------.
HRESULT CBloomRender::Init( ID3D11DeviceContext* pContext11 )
{
	if( FAILED( InitBase( pContext11 ) ))		return E_FAIL;
	if( FAILED( InitBufferTex() ))				return E_FAIL;
	if( FAILED( InitPixelShader( SHADER_NAME, SHADER_ENTRY_NAME ) )) return E_FAIL;
	if( FAILED( CBloomRender::InitVertexBuffer() ))		return E_FAIL;
	if( FAILED( CBloomRender::InitConstantBuffer() ))	return E_FAIL;
	if( FAILED( m_pDownSampling->Init( pContext11 ) ))	return E_FAIL;

	return S_OK;
}

//------------------------------------.
// 描画.
//------------------------------------.
void CBloomRender::Render( const int& srvCount, const std::vector<ID3D11ShaderResourceView*>& srvList )
{
	m_pDownSampling->Render( srvCount, srvList );

	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	// 使用するシェーダのセット.
	m_pContext11->VSSetShader( m_pVertexShader, nullptr, 0 );	// 頂点シェーダ.
	m_pContext11->PSSetShader( m_pPixelShader, nullptr, 0 );	// ピクセルシェーダ.
	m_pContext11->PSSetSamplers( 0, 1, &m_pSampleLinear );		// サンプラのセット.

	UINT width	= m_WndWidth	/ 2;
	UINT height	= m_WndHeight	/ 2;
	float deviation = 2.5f;
	float m = 1.0f;
	ID3D11ShaderResourceView* srv = m_pDownSampling->GetShaderResourceViewList()[0];
	for( int  i = 0; i < BLUR_SAMPLE_NUM*2; i+=2 ){
		// ビューポートの設定.
		D3D11_VIEWPORT vp;
		vp.Width	= (FLOAT)width;
		vp.Height	= (FLOAT)height;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0.0f;
		vp.TopLeftY = 0.0f;

		m_pContext11->RSSetViewports( 1, &vp );

		//=======================================.
		// 横方向にブラーをかけて描画.
		// シェーダーのコンスタントバッファに各種データを渡す.
		D3D11_MAPPED_SUBRESOURCE pData;
		// バッファ内のデータの書き換え開始時にMap.
		if( SUCCEEDED( m_pContext11->Map( m_pConstantBufferFrame, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData ))){
			C_BUFFER_PER_FRAME cb = 
				CalcBlurParam( { static_cast<float>(width),static_cast<float>(height) }, { 0.0f, 1.0f }, deviation, m );

			// メモリ領域をコピー.
			memcpy_s( pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb) );
			m_pContext11->Unmap( m_pConstantBufferFrame, 0 );
		}
		Render( i, srv );

		srv = m_pShaderResourceViewList[i];

		//=======================================.
		// 縦方向にブラーをかけて描画.
		// バッファ内のデータの書き換え開始時にMap.
		if( SUCCEEDED( m_pContext11->Map( m_pConstantBufferFrame, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData ))){
			C_BUFFER_PER_FRAME cb = 
				CalcBlurParam( { static_cast<float>(width),static_cast<float>(height) }, { 1.0f, 0.0f }, deviation, m );

			// メモリ領域をコピー.
			memcpy_s( pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb) );
			m_pContext11->Unmap( m_pConstantBufferFrame, 0 );
		}
		Render( i+1, srv );

		width	>>= 1;	if( width	<= 0 ) width	= 1;
		height	>>= 1;	if( height	<= 0 ) height	= 1;
		m *= 2.0f;

		srv = m_pShaderResourceViewList[i+1];
	}

	// ビューポートの設定.
	D3D11_VIEWPORT vp;
	vp.Width	= (FLOAT)m_WndWidth;
	vp.Height	= (FLOAT)m_WndHeight;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;

	m_pContext11->RSSetViewports( 1, &vp );
}

//------------------------------------.
// バッファの設定.
//------------------------------------.
void CBloomRender::SetBuffer()
{
	CRenderTexture::SetBuffer( BUFFER_COUNT_MAX );
}

//------------------------------------.
// テクスチャの初期化.
//------------------------------------.
HRESULT CBloomRender::InitBufferTex()
{
	UINT width	= m_WndWidth / 2;
	UINT height	= m_WndHeight / 2;

	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width				= width;							// 幅.
	texDesc.Height				= height;							// 高さ.
	texDesc.MipLevels			= 1;								// ミップマップレベル:1.
	texDesc.ArraySize			= 1;								// 配列数:1.
	texDesc.Format				= DXGI_FORMAT_R16G16B16A16_FLOAT;	// 32ビットフォーマット.
	texDesc.SampleDesc.Count	= 1;								// マルチサンプルの数.
	texDesc.SampleDesc.Quality	= 0;								// マルチサンプルのクオリティ.
	texDesc.Usage				= D3D11_USAGE_DEFAULT;				// 使用方法:デフォルト.
	texDesc.BindFlags			= D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;	// レンダーターゲット、シェーダーリソース.
	texDesc.CPUAccessFlags		= 0;								// CPUからはアクセスしない.
	texDesc.MiscFlags			= 0;								// その他の設定なし.

	m_pRenderTargetViewList.resize( BUFFER_COUNT_MAX*2 );
	m_pShaderResourceViewList.resize( BUFFER_COUNT_MAX*2 );
	m_pTexture2DList.resize( BUFFER_COUNT_MAX*2 );

	for( int i = 0; i < BUFFER_COUNT_MAX*2; i+=2 ){
		if( FAILED( CreateBufferTex(
			texDesc,
			&m_pRenderTargetViewList[i],
			&m_pShaderResourceViewList[i],
			&m_pTexture2DList[i] ))) return E_FAIL;
		if( FAILED( CreateBufferTex(
			texDesc,
			&m_pRenderTargetViewList[i+1],
			&m_pShaderResourceViewList[i+1],
			&m_pTexture2DList[i+1] ))) return E_FAIL;

		texDesc.Width	>>= 1;	if( texDesc.Width	<= 0 ) texDesc.Width	= 1;
		texDesc.Height	>>= 1;	if( texDesc.Height	<= 0 ) texDesc.Height	= 1;
	}

	return S_OK;
}

//------------------------------------.
// 頂点バッファーの作成.
//------------------------------------.
HRESULT CBloomRender::InitVertexBuffer()
{
	UINT width	= m_WndWidth / 2;
	UINT height	= m_WndHeight / 2;
	m_pVertexBuffers.resize( BLUR_SAMPLE_NUM*2 );
	for( int i = 0; i < BUFFER_COUNT_MAX*2; i += 2 ){
		// 板ポリ(四角形)の頂点を作成.
		VERTEX vertices[]=
		{
			// 頂点座標(x,y,z)				 
			D3DXVECTOR3( 0.0f,						static_cast<float>(height),	0.0f ),	D3DXVECTOR2( 0.0f, 1.0f ),
			D3DXVECTOR3( 0.0f,						0.0f,						0.0f ),	D3DXVECTOR2( 0.0f, 0.0f ),
			D3DXVECTOR3( static_cast<float>(width),	static_cast<float>(height),	0.0f ),	D3DXVECTOR2( 1.0f, 1.0f ),
			D3DXVECTOR3( static_cast<float>(width),	0.0f,						0.0f ),	D3DXVECTOR2( 1.0f, 0.0f ),
		};
		// 最大要素数を算出する.
		UINT uVerMax = sizeof(vertices) / sizeof(vertices[0]);

		// バッファ構造体.
		D3D11_BUFFER_DESC bd;
		bd.Usage				= D3D11_USAGE_DEFAULT;		// 使用方法(デフォルト).
		bd.ByteWidth			= sizeof(VERTEX) * uVerMax;	// 頂点のサイズ.
		bd.BindFlags			= D3D11_BIND_VERTEX_BUFFER;	// 頂点バッファとして扱う.
		bd.CPUAccessFlags		= 0;	// CPUからはアクセスしない.
		bd.MiscFlags			= 0;	// その他のフラグ(未使用).
		bd.StructureByteStride	= 0;	// 構造体のサイズ(未使用).

		// サブリソースデータ構造体.
		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = vertices;	// 板ポリの頂点をセット.

		// 頂点バッファの作成.
		if( FAILED( m_pDevice11->CreateBuffer(
			&bd, 
			&InitData, 
			&m_pVertexBuffers[i]))){
			ERROR_MESSAGE( "頂点ﾊﾞｯﾌｧ作成失敗" );
			return E_FAIL;
		}
		// 頂点バッファの作成.
		if( FAILED( m_pDevice11->CreateBuffer(
			&bd, 
			&InitData, 
			&m_pVertexBuffers[i+1]))){
			ERROR_MESSAGE( "頂点ﾊﾞｯﾌｧ作成失敗" );
			return E_FAIL;
		}

		width	>>= 1;	if( width	<= 0 ) width	= 1;
		height	>>= 1;	if( height	<= 0 ) height	= 1;
	}
	return S_OK;
}

//------------------------------------.
// コンスタントバッファの作成.
//------------------------------------.
HRESULT CBloomRender::InitConstantBuffer()
{

	// コンスタントバッファの作成.
	if( FAILED( shader::CreateConstantBuffer( m_pDevice11, &m_pConstantBufferFrame, sizeof(C_BUFFER_PER_FRAME) ))) return  E_FAIL;

	UINT width = m_WndWidth / 2;
	UINT height = m_WndHeight / 2;
	m_pConstantBuffersInit.resize( BLUR_SAMPLE_NUM*2 );
	for( int  i = 0; i < BLUR_SAMPLE_NUM*2; i+=2 ){
		// コンスタントバッファの作成.
		shader::CreateConstantBuffer( m_pDevice11, &m_pConstantBuffersInit[i], sizeof(C_BUFFER_PER_INIT) );
		shader::CreateConstantBuffer( m_pDevice11, &m_pConstantBuffersInit[i+1], sizeof(C_BUFFER_PER_INIT) );

		// シェーダーのコンスタントバッファに各種データを渡す.
		D3D11_MAPPED_SUBRESOURCE pData;
		// バッファ内のデータの書き換え開始時にMap.
		C_BUFFER_PER_INIT cb;	// コンスタントバッファ.
		D3DXMatrixIdentity( &cb.mW );
		D3DXMatrixTranspose( &cb.mW, &cb.mW );
		// ビューポートの幅,高さを渡す.
		cb.vViewPort.x = static_cast<float>(width);
		cb.vViewPort.y = static_cast<float>(height);
		// ピクセルのサイズを渡す.
		cb.vPixelSize.x = 1.0f / static_cast<float>(width);
		cb.vPixelSize.y = 1.0f / static_cast<float>(height);
		// ウィンドウ比率を渡す.
		cb.vWndRatio.x = static_cast<float>(m_WndWidth/width);
		cb.vWndRatio.y = static_cast<float>(m_WndHeight/height);

		if( FAILED( m_pContext11->Map( m_pConstantBuffersInit[i], 0, D3D11_MAP_WRITE_DISCARD, 0, &pData ))) return  E_FAIL;
		memcpy_s( pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb) );
		m_pContext11->Unmap( m_pConstantBuffersInit[i], 0 );

		if( FAILED( m_pContext11->Map( m_pConstantBuffersInit[i+1], 0, D3D11_MAP_WRITE_DISCARD, 0, &pData ))) return  E_FAIL;
		memcpy_s( pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb) );
		m_pContext11->Unmap( m_pConstantBuffersInit[i+1], 0 );

		width	>>= 1;	if( width	<= 0 ) width	= 1;
		height	>>= 1;	if( height	<= 0 ) height	= 1;
	}

	return S_OK;
}

//------------------------------------.
// 描画.
//------------------------------------.
void CBloomRender::Render(
	const int& index,
	ID3D11ShaderResourceView* pSrv )
{
	// ターゲットビューのクリア.
	m_pContext11->ClearRenderTargetView( m_pRenderTargetViewList[index], CLEAR_BACK_COLOR );
	// レンダーターゲットの設定.
	m_pContext11->OMSetRenderTargets( 1, &m_pRenderTargetViewList[index], CDirectX11::GetDepthSV() );
	// デプスステンシルバッファ.
	m_pContext11->ClearDepthStencilView( CDirectX11::GetDepthSV(), D3D11_CLEAR_DEPTH, 1.0f, 0 );

	m_pContext11->VSSetConstantBuffers( 0, 1, &m_pConstantBuffersInit[index] );	// 頂点シェーダ.
	m_pContext11->PSSetConstantBuffers( 0, 1, &m_pConstantBuffersInit[index] );	// ピクセルシェーダー.

	m_pContext11->VSSetConstantBuffers( 1, 1, &m_pConstantBufferFrame );	// 頂点シェーダ.
	m_pContext11->PSSetConstantBuffers( 1, 1, &m_pConstantBufferFrame );	// ピクセルシェーダー.

	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	m_pContext11->IASetVertexBuffers( 0, 1, &m_pVertexBuffers[index], &stride, &offset );

	m_pContext11->PSSetShaderResources( 0, 1, &pSrv );
	m_pContext11->Draw( 4, 0 );

	std::vector<ID3D11ShaderResourceView*> resetSrvList(1);
	m_pContext11->PSSetShaderResources( 0, 1, &resetSrvList[0] );
}

//------------------------------------.
// ガウスの重みを計算.
//------------------------------------.
float CBloomRender::GaussianDistriBution( const D3DXVECTOR2& pos, const float& rho )
{
	return exp( -( pos.x * pos.x + pos.y * pos.y ) / ( 2.0f * rho * rho ));
}

//------------------------------------.
// ブラーのパラメータを計算.
//------------------------------------.
CBloomRender::C_BUFFER_PER_FRAME CBloomRender::CalcBlurParam( 
	const D3DXVECTOR2& size, 
	const D3DXVECTOR2& dir,
	const float& deviation,
	const float& multiply )
{
	C_BUFFER_PER_FRAME param;

	const float tu = 1.0f / size.x;
	const float tv = 1.0f / size.y;

	param.Offset[0].x = 0.0f;
	param.Offset[0].y = 0.0f;
	param.Offset[0].z = GaussianDistriBution( { 0.0f, 0.0f }, deviation ) * multiply;

	float totalWeight = param.Offset[0].z;

	for( int i = 1; i < param.HALF_NUM_OFFSET; i++ ){
		param.Offset[i].x = dir.x * i * tu;
		param.Offset[i].y = dir.y * i * tv;
		param.Offset[i].z = GaussianDistriBution( dir * static_cast<float>(i), deviation ) * multiply;

		totalWeight += param.Offset[0].z * 2.0f;
	}

	for( int i = 0; i < param.HALF_NUM_OFFSET; i++ ){
		param.Offset[i].z /= totalWeight;
	}

	for( int i = param.HALF_NUM_OFFSET; i < param.NUM_OFFSET-1; i++ ){
		param.Offset[i].x = -param.Offset[i-7].x;
		param.Offset[i].y = -param.Offset[i-7].y;
		param.Offset[i].z =  param.Offset[i-7].z;
	}

	return param;
}
