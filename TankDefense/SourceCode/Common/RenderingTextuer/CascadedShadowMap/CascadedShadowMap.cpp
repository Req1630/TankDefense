#include "CascadedShadowMap.h"
#include "..\..\..\Object\CameraBase\CameraManager\CameraManager.h"
#include "..\..\..\Object\LightBase\LightManager\LightManager.h"
#include "..\..\Shader\Shader.h"

namespace
{
	constexpr char	SHADER_NAME[]			= "Data\\Shader\\ShadowMap.hlsl";	// ピクセルシェーダー名.
	constexpr char	PS_SHADER_ENTRY_NAME[]	= "PS_Main";	// ピクセルシェーダーエントリー名.
	
	const D3DXVECTOR3 VIEW_UP_VECTOR	= { 0.0f, 1.0f, 0.0f };	// 上方向ベクトル.
	const D3DXMATRIX SHADOW_BIAS = 
	{
		0.5f,  0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f,
		0.0f,  0.0f, 1.0f, 0.0f,
		0.5f,  0.5f, 0.0f, 1.0f 
	};
};

CCascadedShadowMap::CCascadedShadowMap()
	: m_pSkinVertexShader		( nullptr )
	, m_pConstantBufferFrame	( nullptr )
	, m_ShadowMatrix			( CASCADED_NUM )
	, m_SplitPos				( CASCADED_NUM )
	, m_ProjMatrix				()
	, m_ViewMatrix				()
	, m_MaxClipDistance			( 1000.0f )
	, m_MinClipDistance			( 0.01f )
	, m_AspectWindow			( 0.0f )
	, m_FovCamera				( 0.0f )
	, m_IsEndRender				( false )
{
}

CCascadedShadowMap::~CCascadedShadowMap()
{
	SAFE_RELEASE(m_pSkinVertexShader);
	SAFE_RELEASE(m_pConstantBufferFrame);
}

//------------------------------------.
// インスタンスの取得.
//------------------------------------.
CCascadedShadowMap* CCascadedShadowMap::GetInstance()
{
	static std::unique_ptr<CCascadedShadowMap> pInstance = 
		std::make_unique<CCascadedShadowMap>();
	return pInstance.get();
}

//------------------------------------.
// 初期化.
//------------------------------------.
HRESULT CCascadedShadowMap::Init( ID3D11DeviceContext* pContext11 )
{
	if( FAILED( InitBase( pContext11 ) ))		return E_FAIL;
	if( FAILED( InitBufferTex() ))				return E_FAIL;
	if( FAILED( InitVertexShader() ))			return E_FAIL;
	if( FAILED( InitPixelShader( SHADER_NAME, PS_SHADER_ENTRY_NAME ) )) return E_FAIL;
	if( FAILED( shader::CreateConstantBuffer( m_pDevice11, &m_pConstantBufferFrame, sizeof(C_CBUFFER) ))) return E_FAIL;

	m_AspectWindow = static_cast<float>(m_WndWidth)/static_cast<float>(m_WndHeight);
	m_FovCamera = static_cast<float>(3.141519/4.0);

	return S_OK;
}

//------------------------------------.
// 更新.
//------------------------------------.
void CCascadedShadowMap::Update()
{
	m_IsEndRender = false;

	D3DXVECTOR3 pos = CLightManager::GetPosition();
	D3DXVECTOR3 lookPos = CLightManager::GetLookPosition();

	// ビュー(カメラ)変換.
	D3DXMatrixLookAtLH( 
		&m_ViewMatrix,		// (out)viewMarix.
		&pos,				// カメラ座標.
		&lookPos,			// カメラ注視座標.
		&VIEW_UP_VECTOR );	// カメラベクトル.

	const float size = D3DXVec3Length( &(lookPos-pos) );
	m_MinClipDistance = size * 0.5f;
	m_MaxClipDistance = size + m_MinClipDistance;

	// プロジェクション(射影)変換.
	D3DXMatrixOrthoLH(
		&m_ProjMatrix,				// (out)ProjMatrix.
		size,						// 視野角.
		size,						// 画面アスペクト比.
		m_MinClipDistance,			// 最小描画距離.
		m_MaxClipDistance );		// 最大描画距離.

	// 平行分割処理.
	std::vector<float> spritPos( CASCADED_NUM + 1 );
	ComputeSplitPositions( CASCADED_NUM, 0.84f, 0.01f, 1000.0f, spritPos );

	// カスケード処理.
	for( int i = 0; i < CASCADED_NUM; i++ ){
		// ライトのビュー射影行列.
		m_ShadowMatrix[i] = m_ViewMatrix * m_ProjMatrix;

		// 分割した視錘台の8角をもとめて，
		//	ライトのビュー射影空間でAABBを求める.
		SBBox box = CalculateFrustum(
			spritPos[ i + 0 ],
			spritPos[ i + 1 ],
			m_ShadowMatrix[ i ] );

		// クロップ行列を求める.
		D3DXMATRIX crop = CreateCropMatrix( box );

		// シャドウマップ行列と分割位置を設定.
		m_ShadowMatrix[i]	= m_ShadowMatrix[i] * crop;
		m_SplitPos[i]		= spritPos[i+1];
	}

	this->SetBuffer();
	m_pContext11->PSSetShader( m_pPixelShader, nullptr, 0 );	// ピクセルシェーダ.

	m_pContext11->VSSetConstantBuffers( 0, 1, &m_pConstantBufferFrame );	// 頂点シェーダ.
	m_pContext11->PSSetConstantBuffers( 0, 1, &m_pConstantBufferFrame );	// ピクセルシェーダー.
}

//------------------------------------.
// 描画.
//------------------------------------.
bool CCascadedShadowMap::Render( const bool& isSkin, const D3DXMATRIX& mW, std::function<void()>& func )
{
	if( m_IsEndRender == true ) return false;

	// 使用するシェーダのセット.
	m_pContext11->VSSetShader( isSkin == true ? m_pSkinVertexShader : m_pVertexShader, nullptr, 0 );	// 頂点シェーダ.

	for( int i = 0; i < CASCADED_NUM; i++ ){
		// レンダーターゲットの設定.
		m_pContext11->OMSetRenderTargets( 1, &m_pRenderTargetViewList[i], CDirectX11::GetDepthSV() );
		// デプスステンシルバッファ.
		m_pContext11->ClearDepthStencilView( CDirectX11::GetDepthSV(), D3D11_CLEAR_DEPTH, 1.0f, 0 );

		// シェーダーのコンスタントバッファに各種データを渡す.
		D3D11_MAPPED_SUBRESOURCE pData;
		if( SUCCEEDED( m_pContext11->Map( m_pConstantBufferFrame, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData ))){
			C_CBUFFER cb;

			cb.ShadowVP = mW * m_ShadowMatrix[i];
			D3DXMatrixTranspose( &cb.ShadowVP, &cb.ShadowVP );

			// メモリ領域をコピー.
			memcpy_s( pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb) );
			m_pContext11->Unmap( m_pConstantBufferFrame, 0 );
		}

		func();
	}
	return true;
}

//------------------------------------.
// バッファの設定.
//------------------------------------.
void CCascadedShadowMap::SetBuffer()
{
	// G-Bufferテクスチャのクリア.
	for( auto& rtv : m_pRenderTargetViewList ){
		m_pContext11->ClearRenderTargetView( rtv, CLEAR_BACK_COLOR );
	}
}

//------------------------------------.
// テクスチャの初期化.
//------------------------------------.
HRESULT CCascadedShadowMap::InitBufferTex()
{
	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width				= m_WndWidth;								// 幅.
	texDesc.Height				= m_WndHeight;								// 高さ.
	texDesc.MipLevels			= 1;								// ミップマップレベル:1.
	texDesc.ArraySize			= 1;								// 配列数:1.
	texDesc.Format				= DXGI_FORMAT_R32_FLOAT;			// 32ビットフォーマット.
	texDesc.SampleDesc.Count	= 1;								// マルチサンプルの数.
	texDesc.SampleDesc.Quality	= 0;								// マルチサンプルのクオリティ.
	texDesc.Usage				= D3D11_USAGE_DEFAULT;				// 使用方法:デフォルト.
	texDesc.BindFlags			= D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;	// レンダーターゲット、シェーダーリソース.
	texDesc.CPUAccessFlags		= 0;								// CPUからはアクセスしない.
	texDesc.MiscFlags			= 0;								// その他の設定なし.

	m_pRenderTargetViewList.resize( CASCADED_NUM );
	m_pShaderResourceViewList.resize( CASCADED_NUM );
	m_pTexture2DList.resize( CASCADED_NUM );

	for( int i = 0; i < CASCADED_NUM; i++ ){
		if( FAILED( CreateBufferTex(
			texDesc,
			&m_pRenderTargetViewList[i],
			&m_pShaderResourceViewList[i],
			&m_pTexture2DList[i] ))) return E_FAIL;
	}
	return S_OK;
}

//------------------------------------.
// 頂点シェーダーの作成.
//------------------------------------.
HRESULT CCascadedShadowMap::InitVertexShader()
{
	ID3DBlob* pCompiledShader = nullptr;
	ID3DBlob* pErrors = nullptr;
	UINT uCompileFlag = 0;
#ifdef _DEBUG
	uCompileFlag = D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION;
#endif	// #ifdef _DEBUG

	// HLSLからバーテックスシェーダーのブロブを作成.
	if( FAILED(
		shader::InitShader(
		SHADER_NAME,		// シェーダーファイル名.
		"VS_Main",			// シェーダーエントリーポイント.
		"vs_5_0",			// シェーダーモデル.
		uCompileFlag,		// シェーダーコンパイルフラグ.
		&pCompiledShader,	// ブロブを格納するメモリへのポインタ.
		&pErrors ))) {		// エラーと警告一覧を格納するメモリへのポインタ.
		ERROR_MESSAGE( shader::GetBlobErrorMsg( pErrors ) );
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);

	// 上記で作成したブロブから「頂点シェーダー」を作成.
	if( FAILED( shader::CreateVertexShader(
		m_pDevice11,
		pCompiledShader, 
		&m_pVertexShader ))) {
		ERROR_MESSAGE("頂点シェーダー作成 : 失敗");
		return E_FAIL;
	}

	// HLSLからバーテックスシェーダーのブロブを作成.
	if( FAILED(
		shader::InitShader(
		SHADER_NAME,		// シェーダーファイル名.
		"VS_SkinMain",		// シェーダーエントリーポイント.
		"vs_5_0",			// シェーダーモデル.
		uCompileFlag,		// シェーダーコンパイルフラグ.
		&pCompiledShader,	// ブロブを格納するメモリへのポインタ.
		&pErrors ))) {		// エラーと警告一覧を格納するメモリへのポインタ.
		ERROR_MESSAGE( shader::GetBlobErrorMsg( pErrors ) );
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);

	// 上記で作成したブロブから「頂点シェーダー」を作成.
	if( FAILED( shader::CreateVertexShader(
		m_pDevice11,
		pCompiledShader, 
		&m_pSkinVertexShader ))) {
		ERROR_MESSAGE("頂点シェーダー作成 : 失敗");
		return E_FAIL;
	}
	return S_OK;
}

//------------------------------------.
// 平行分割処理.
//------------------------------------.
void CCascadedShadowMap::ComputeSplitPositions(
	const int splitCount, const float lamda, 
	const float nearClip, const float farClip,
	std::vector<float>& spritPos )
{
	// 分割数が１の場合は，普通のシャドウマップと同じ.
	if ( splitCount == 1 )
	{
		m_SplitPos[0] = nearClip;
		m_SplitPos[1] = farClip;
		return;
	}

	float inv_m = 1.0f / float( splitCount );    // splitCountがゼロでないことは保証済み.

	// ゼロ除算対策.
	_ASSERT( nearClip != 0.0f );

	// (f/n)を計算.
	float f_div_n = farClip / nearClip;

	// (f-n)を計算.
	float f_sub_n = farClip - nearClip;

	// 実用分割スキームを適用.
	// ※ GPU Gems 3, Chapter 10. Parallel-Split Shadow Maps on Programmable GPUs.
	//    http://http.developer.nvidia.com/GPUGems3/gpugems3_ch10.html を参照.
	for( int i=1; i<splitCount + 1; ++i )
	{
		// 対数分割スキームで計算.
		float Ci_log = nearClip * powf( f_div_n, inv_m * i );

		// 一様分割スキームで計算.
		float Ci_uni = nearClip + f_sub_n * i * inv_m;

		// 上記の２つの演算結果を線形補間する.
		spritPos[i] = lamda * Ci_log + Ci_uni * ( 1.0f - lamda );
	}

	// 最初は, ニア平面までの距離を設定.
	spritPos[ 0 ] = nearClip;

	// 最後は, ファー平面までの距離を設定.
	spritPos[ splitCount ] = farClip;
}

//------------------------------------.
// 分割した視錘台の8角をもとめて，
//	ライトのビュー射影空間でAABBを求める.
//------------------------------------.
CCascadedShadowMap::SBBox CCascadedShadowMap::CalculateFrustum( float nearClip, float farClip, D3DXMATRIX& viewProj )
{
	D3DXVECTOR3 Dir = CCameraManager::GetLookPosition() - CCameraManager::GetPosition();
	D3DXVECTOR3 vZ;
	D3DXVec3Normalize( &vZ,&Dir );
	D3DXVECTOR3 vX;
	D3DXVec3Cross( &vX, &D3DXVECTOR3(0.0f, 1.0f, 0.0f), &vZ );
	D3DXVec3Normalize( &vX, &vX );
	D3DXVECTOR3 vY;
	D3DXVec3Cross( &vY, &vZ, &vX );
	D3DXVec3Normalize( &vY, &vY );

	float nearPlaneHalfHeight = tanf( m_FovCamera * 0.5f ) * nearClip;
	float nearPlaneHalfWidth  = nearPlaneHalfHeight * m_AspectWindow;

	float farPlaneHalfHeight = tanf( m_FovCamera * 0.5f ) * farClip;
	float farPlaneHalfWidth  = farPlaneHalfHeight * m_AspectWindow;

	D3DXVECTOR3 nearPlaneCenter = CCameraManager::GetPosition() + vZ * nearClip;
	D3DXVECTOR3 farPlaneCenter  = CCameraManager::GetPosition() + vZ * farClip;;

	D3DXVECTOR3 corners[8];

	corners[0] = D3DXVECTOR3( nearPlaneCenter - vX * nearPlaneHalfWidth - vY * nearPlaneHalfHeight );
	corners[1] = D3DXVECTOR3( nearPlaneCenter - vX * nearPlaneHalfWidth + vY * nearPlaneHalfHeight );
	corners[2] = D3DXVECTOR3( nearPlaneCenter + vX * nearPlaneHalfWidth + vY * nearPlaneHalfHeight );
	corners[3] = D3DXVECTOR3( nearPlaneCenter + vX * nearPlaneHalfWidth - vY * nearPlaneHalfHeight );

	corners[4] = D3DXVECTOR3( farPlaneCenter - vX * farPlaneHalfWidth - vY * farPlaneHalfHeight );
	corners[5] = D3DXVECTOR3( farPlaneCenter - vX * farPlaneHalfWidth + vY * farPlaneHalfHeight );
	corners[6] = D3DXVECTOR3( farPlaneCenter + vX * farPlaneHalfWidth + vY * farPlaneHalfHeight );
	corners[7] = D3DXVECTOR3( farPlaneCenter + vX * farPlaneHalfWidth - vY * farPlaneHalfHeight );


	D3DXVECTOR3 point;
	D3DXVec3TransformCoord( &point, &corners[0], &viewProj );
	D3DXVECTOR3 mini = point;
	D3DXVECTOR3 maxi = point;
	for( int i=1; i<8; ++i )
	{
		D3DXVec3TransformCoord( &point, &corners[i], &viewProj );
		D3DXVec3Minimize( &mini, &point, &mini );
		D3DXVec3Maximize( &maxi, &point, &maxi );
	}

	return SBBox( mini, maxi );
}

//------------------------------------.
// クロップ行列を求める.
//------------------------------------.
D3DXMATRIX CCascadedShadowMap::CreateCropMatrix( SBBox& box )
{
	/* ほぼ単位キューブクリッピングと同じ処理 */
	float scaleX  = 1.0f;
	float scaleY  = 1.0f;
	float scaleZ  = 1.0f;
	float offsetX = 0.0f;
	float offsetY = 0.0f;
	float offsetZ = 0.0f;

	D3DXVECTOR3 mini = box.Min;
	D3DXVECTOR3 maxi = box.Max;

	scaleX = 2.0f / ( maxi.x - mini.x );
	scaleY = 2.0f / ( maxi.y - mini.y );

	offsetX = -0.5f * ( maxi.x + mini.x ) * scaleX;
	offsetY = -0.5f * ( maxi.y + mini.y ) * scaleY;

	// 1.0を下回るとシャドウマップに移る部分が小さくなってしまうので，
	// 制限をかける.
	scaleX = max( 1.0f, scaleX );
	scaleY = max( 1.0f, scaleY );

	return D3DXMATRIX(
		scaleX,  0.0f,    0.0f,    0.0f,
		0.0f,    scaleY,  0.0f,    0.0f,
		0.0f,    0.0f,    scaleZ,  0.0f,
		offsetX, offsetY, offsetZ, 1.0f );
}
