#include "AuraMesh.h"
#include "..\..\Shader\Shader.h"
#include "..\..\..\Object\CameraBase\CameraManager\CameraManager.h"
#include "..\..\..\Utility\DebugConsole\DebugConsole.h"

namespace
{
	constexpr char SHADER_NAME[]	= "Data\\Shader\\Aura.hlsl";	// シェーダー名.
};

CAuraMesh::CAuraMesh()
	: m_pVertexBuffer	( nullptr )
	, m_pIndexBuffer	( nullptr )
	, m_pVertexShader	( nullptr )
	, m_pPixelShader	( nullptr )
	, m_pVertexLayout	( nullptr )
	, m_pConstantBuffer	( nullptr )
	, m_ScrollTime		( 0.0f )
{
}

CAuraMesh::~CAuraMesh()
{
	Release();
}

// 初期化.
HRESULT CAuraMesh::Init()
{
	// パラメータの初期化.
	if( FAILED( SetDevice( CDirectX11::GetContext() )) ) return E_FAIL;
	if( FAILED( InitModel()))	return E_FAIL;	// モデルの読み込み.
	if( FAILED( InitShader()))	return E_FAIL;	// シェーダーの初期化.
	if( FAILED( shader::CreateConstantBuffer( m_pDevice11, &m_pConstantBuffer, sizeof(C_BUFFER) ))) return E_FAIL;

	return S_OK;
}

//解放関数.
void CAuraMesh::Release()
{
	SAFE_RELEASE( m_pVertexBuffer );
	SAFE_RELEASE( m_pIndexBuffer );
	
	SAFE_RELEASE( m_pVertexShader );
	SAFE_RELEASE( m_pPixelShader );
	SAFE_RELEASE( m_pVertexLayout );
	SAFE_RELEASE( m_pConstantBuffer );

	m_pContext11 = nullptr;
	m_pDevice11 = nullptr;
}

// 更新.
void CAuraMesh::Update()
{
	m_ScrollTime -= GetDeltaTime() * 0.25f;
}

// 描画.
void CAuraMesh::Render()
{
	D3DXMATRIX mWorld = m_Tranceform.GetWorldMatrix();

	// シェーダのコンスタントバッファに各種データを渡す.
	D3D11_MAPPED_SUBRESOURCE pData;
	// バッファ内のデータの書き換え開始時にMap.
	if( SUCCEEDED( m_pContext11->Map(
		m_pConstantBuffer, 0,
		D3D11_MAP_WRITE_DISCARD,
		0, &pData)))
	{
		C_BUFFER cb;

		cb.mW = mWorld;
		D3DXMatrixTranspose( &cb.mW, &cb.mW );//行列を転置する.
		cb.mWVP = mWorld * CCameraManager::GetViewProjMatrix();
		D3DXMatrixTranspose( &cb.mWVP, &cb.mWVP );//行列を転置する.

		cb.vColor	= { 0.7f, 0.2f, 0.2f, 0.7f };
		cb.vUV.x	= m_ScrollTime;
		cb.vUV.y	= m_ScrollTime;

		memcpy_s( pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));

		// バッファ内のデータの書き換え終了時にUnmap.
		m_pContext11->Unmap( m_pConstantBuffer, 0 );
	}

	// 使用するシェーダのセット.
	m_pContext11->VSSetShader( m_pVertexShader, nullptr, 0 );
	m_pContext11->PSSetShader( m_pPixelShader, nullptr, 0 );

	// このコンスタントバッファをどのシェーダで使用するか？.
	m_pContext11->VSSetConstantBuffers( 0, 1, &m_pConstantBuffer );
	m_pContext11->PSSetConstantBuffers( 0, 1, &m_pConstantBuffer );

	// 頂点バッファをセット.
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	m_pContext11->IASetVertexBuffers( 0, 1, &m_pVertexBuffer, &stride, &offset );
	m_pContext11->IASetIndexBuffer( m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0 );

	// 頂点インプットレイアウトをセット.
	m_pContext11->IASetInputLayout( m_pVertexLayout );
	m_pContext11->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	SetBlend( true );
	SetRasterizerState( ERS_STATE::Front );
	m_pContext11->DrawIndexed( 30, 0, 0 );
	SetRasterizerState( ERS_STATE::Back );
	m_pContext11->DrawIndexed( 30, 0, 0 );
	SetRasterizerState( ERS_STATE::None );
	SetBlend( false );
}

// メッシュ読み込み.
HRESULT CAuraMesh::InitModel()
{
	const float boxSize = 1.0f;
	const float height = 20.0f;
	//板ﾎﾟﾘ(四角形)の頂点を作成.
	VERTEX vertices[]=
	{
		//頂点座標(x,y,z)				 
		D3DXVECTOR3( boxSize, 0.0f,		-boxSize ),
		D3DXVECTOR3(-boxSize, 0.0f,		 boxSize ),
		D3DXVECTOR3(-boxSize, 0.0f,		-boxSize ),
		D3DXVECTOR3( boxSize, 0.0f,		 boxSize ),

		D3DXVECTOR3( boxSize, height,	-boxSize ),
		D3DXVECTOR3( boxSize, 0.0f,		 boxSize ),
		D3DXVECTOR3( boxSize, 0.0f,		-boxSize ),
		D3DXVECTOR3( boxSize, height,	 boxSize ),

		D3DXVECTOR3( boxSize, height,	 boxSize ),
		D3DXVECTOR3(-boxSize, 0.0f,		 boxSize ),
		D3DXVECTOR3( boxSize, 0.0f,		 boxSize ),
		D3DXVECTOR3(-boxSize, height,	 boxSize ),

		D3DXVECTOR3(-boxSize, height,	 boxSize ),
		D3DXVECTOR3(-boxSize, 0.0f,		-boxSize ),
		D3DXVECTOR3(-boxSize, 0.0f,		 boxSize ),
		D3DXVECTOR3(-boxSize, height,	-boxSize ),

		D3DXVECTOR3(-boxSize, height,	-boxSize ),
		D3DXVECTOR3( boxSize, 0.0f,		-boxSize ),
		D3DXVECTOR3(-boxSize, 0.0f,		-boxSize ),
		D3DXVECTOR3( boxSize, height,	-boxSize ),

	};
	WORD indexList[]{
		0,	1,	2,		1,	0,	3,
		4,	5,	6,		5,	4,	7,
		8,	9,	10,		9,	8,	11,
		12,	13,	14,		13,	12,	15,
		16,	17,	18,		17,	16,	19,
	};

	// 最大要素数を算出する.
	UINT uVerMax = sizeof(vertices)/sizeof(vertices[0]);
	UINT uIndexMax = sizeof(indexList)/sizeof(indexList[0]);

	// バッファ構造体.
	D3D11_BUFFER_DESC bd;
	bd.Usage				= D3D11_USAGE_DEFAULT;		// 使用方法(デフォルト).
	bd.ByteWidth			= sizeof(VERTEX)*uVerMax;	// 頂点のサイズ.
	bd.BindFlags			= D3D11_BIND_VERTEX_BUFFER;	// 頂点バッファとして扱う.
	bd.CPUAccessFlags		= 0;						// CPUからはアクセスしない.
	bd.MiscFlags			= 0;						// その他のフラグ(未使用).
	bd.StructureByteStride	= 0;						// 構造体のサイズ(未使用).

	//ｻﾌﾞﾘｿｰｽﾃﾞｰﾀ構造体.
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertices;	//板ﾎﾟﾘの頂点をｾｯﾄ.

	//頂点ﾊﾞｯﾌｧの作成.
	if( FAILED( m_pDevice11->CreateBuffer(
		&bd, &InitData, &m_pVertexBuffer ) ) ){
		_ASSERT_EXPR( false, L"頂点ﾊﾞｯﾌｧ作成失敗" );
		return E_FAIL;
	}

	D3D11_BUFFER_DESC id;
	id.Usage				= D3D11_USAGE_DEFAULT;	   // 使用方法(デフォルト).
	id.ByteWidth			= sizeof(WORD)*uIndexMax;  // 頂点のサイズ.
	id.BindFlags			= D3D11_BIND_INDEX_BUFFER; // 頂点バッファとして扱う.
	id.CPUAccessFlags		= 0;					   // CPUからはアクセスしない.
	id.MiscFlags			= 0;					   // その他のフラグ(未使用).
	id.StructureByteStride	= 0;					   // 構造体のサイズ(未使用).

	D3D11_SUBRESOURCE_DATA IndexData;
	IndexData.pSysMem			= indexList;
	IndexData.SysMemPitch		= 0;
	IndexData.SysMemSlicePitch	= 0;

	if( FAILED( m_pDevice11->CreateBuffer(
		&id, &IndexData, &m_pIndexBuffer ))){
		_ASSERT_EXPR( false, L"インデックスﾊﾞｯﾌｧ作成失敗" );
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CAuraMesh::InitShader()
{
	ID3DBlob* pCompiledShader = nullptr;
	ID3DBlob* pErrors = nullptr;
	UINT uCompileFlag = 0;
#ifdef _DEBUG
	uCompileFlag =
		D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION;
#endif//#ifdef _DEBUG

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

	// 頂点インプットレイアウトを定義.
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		shader::GetPositionInputElement(),
	};

	// 頂点インプットレイアウトの配列要素数を算出.
	UINT numElements = sizeof(layout) / sizeof(layout[0]);

	// 頂点インプットレイアウトを作成.
	if( FAILED(
		shader::CreateInputLayout(
			m_pDevice11,
			layout,
			numElements,
			pCompiledShader,
			&m_pVertexLayout))) {
		ERROR_MESSAGE("頂点インプットレイアウト作成 : 失敗");
		return E_FAIL;
	}
	SAFE_RELEASE(pCompiledShader);

	// HLSLからピクセルシェーダーのブロブを作成.
	if( FAILED(
		shader::InitShader(
			SHADER_NAME,		// シェーダーファイル名.
			"PS_Main",			// シェーダーエントリーポイント.
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
		&m_pPixelShader ))) {
		ERROR_MESSAGE("ピクセルシェーダー作成 : 失敗");
		return E_FAIL;
	}
	SAFE_RELEASE(pCompiledShader);

	return S_OK;
}