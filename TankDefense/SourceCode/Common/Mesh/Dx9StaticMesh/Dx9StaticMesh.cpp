#include "Dx9StaticMesh.h"
#include "..\..\..\Object\CameraBase\CameraManager\CameraManager.h"
#include "..\..\..\Object\LightBase\LightManager\LightManager.h"
#include "..\..\RenderingTextuer\CascadedShadowMap\CascadedShadowMap.h"
#include "..\..\Shader\Shader.h"

// �V�F�[�_�[�t�@�C����.
const char SHADER_VS_NAME[] = "Data\\Shader\\Mesh.hlsl";
const char SHADER_PS_NAME[] = "Data\\Shader\\MeshPS.hlsl";

CDX9StaticMesh::CDX9StaticMesh()
	: m_hWnd				( nullptr )
	, m_pDevice9			( nullptr )
	, m_pVertexShader		( nullptr )
	, m_pVertexLayout		( nullptr )
	, m_pPixelShader		( nullptr )
	, m_pCBufferPerMesh		( nullptr )
	, m_pCBufferPerMaterial	( nullptr )
	, m_pCBufferPerFrame	( nullptr )
	, m_pVertexBuffer		( nullptr )
	, m_ppIndexBuffer		( nullptr )
	, m_pSampleLinear		( nullptr )
	, m_pMesh				( nullptr )
	, m_pMeshForRay			( nullptr )
	, m_NumMaterials		( 0 )
	, m_pMaterials			( nullptr )
	, m_NumAttr				( 0 )
	, m_AttrID				()
	, m_ShadowDepth			( 0.5f )
	, m_ShadowRenderFunc	()
{
	m_pShadowMap = CCascadedShadowMap::GetInstance();
	m_ShadowRenderFunc = [&]()
	{ 
		// ���_�o�b�t�@���Z�b�g.
		UINT stride = m_pMesh->GetNumBytesPerVertex();
		UINT offset = 0;
		m_pContext11->IASetVertexBuffers(
			0, 1, &m_pVertexBuffer, &stride, &offset);
		// �v���~�e�B�u�E�g�|���W�[���Z�b�g.
		m_pContext11->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
		// �����̐������A���ꂼ��̑����̃C���f�b�N�X�o�b�t�@��`��.
		for( DWORD No = 0; No < m_NumAttr; No++ ){
			// �C���f�b�N�X�o�b�t�@���Z�b�g.
			m_pContext11->IASetIndexBuffer( m_ppIndexBuffer[No], DXGI_FORMAT_R32_UINT, 0 );
			// �|���S���������_�����O.
			m_pContext11->DrawIndexed( m_pMaterials[m_AttrID[No]].dwNumFace*3, 0, 0 );
		}
	};
}

CDX9StaticMesh::~CDX9StaticMesh()
{
	Release();
}

// �������֐�.
HRESULT CDX9StaticMesh::Init(
	HWND hWnd, 
	ID3D11Device* pDevice11,
	ID3D11DeviceContext* pContext11, 
	LPDIRECT3DDEVICE9 pDevice9,
	const char* fileName )
{
	m_hWnd = hWnd;
	m_pDevice9 = pDevice9;
	if( FAILED( SetDevice( pContext11 ) )) return E_FAIL;
	if( FAILED( LoadXMesh(fileName) )) return E_FAIL; 
	if( FAILED( InitShader() )) return E_FAIL;

	std::string msg = fileName;
	msg += " �ǂݍ��� : ����";
	CLog::Print( std::string( msg.c_str() ).c_str() );

	return S_OK;
}

// ���b�V���ǂݍ���.
HRESULT CDX9StaticMesh::LoadXMesh(const char* fileName)
{
	// �}�e���A���o�b�t�@.
	LPD3DXBUFFER pD3DXMtrlBuffer = nullptr;

	// X�t�@�C���̃��[�h.
	if( FAILED( D3DXLoadMeshFromX(
		fileName,				// �t�@�C����.
		D3DXMESH_SYSTEMMEM		// �V�X�e���������ɓǂݍ���.
			| D3DXMESH_32BIT,	// 32bit.
		m_pDevice9, 
		nullptr,
		&pD3DXMtrlBuffer,		// (out)�}�e���A�����.
		nullptr,
		&m_NumMaterials,		// (out)�}�e���A����.
		&m_pMesh ))){			// (out)���b�V���I�u�W�F�N�g.
		ERROR_MESSAGE("X�t�@�C���Ǎ� : ���s");
		return E_FAIL;
	}

	// X�t�@�C���̃��[�h(���C�Ƃ̔���p).
	if( FAILED( D3DXLoadMeshFromX(
		fileName,				// �t�@�C����.
		D3DXMESH_SYSTEMMEM,		// �V�X�e���������ɓǂݍ���.
		m_pDevice9, 
		nullptr,
		&pD3DXMtrlBuffer,		// (out)�}�e���A�����.
		nullptr,
		&m_NumMaterials,		// (out)�}�e���A����.
		&m_pMeshForRay ))){		// (out)���b�V���I�u�W�F�N�g.
		ERROR_MESSAGE("X�t�@�C���Ǎ� : ���s");
		return E_FAIL;
	}

	D3D11_BUFFER_DESC		bd;			// Dx11�o�b�t�@�\����.
	D3D11_SUBRESOURCE_DATA	InitData;	// �������f�[�^.

	// �ǂݍ��񂾏�񂩂�K�v�ȏ��𔲂��o��.
	D3DXMATERIAL* d3dxMaterials = static_cast<D3DXMATERIAL*>(pD3DXMtrlBuffer->GetBufferPointer());
	// �}�e���A�������̗̈���m��.
	m_pMaterials	= new MY_MATERIAL[m_NumMaterials]();
	m_ppIndexBuffer	= new ID3D11Buffer*[m_NumMaterials]();
	// �}�e���A�������J��Ԃ�.
	for( DWORD No = 0; No < m_NumMaterials; No++ ){
		// �C���f�b�N�X�o�b�t�@�̏�����.
		m_ppIndexBuffer[No] = nullptr;

		// �}�e���A�����̃R�s�[.
		// �A���r�G���g.
		m_pMaterials[No].Ambient.x = d3dxMaterials[No].MatD3D.Ambient.r;
		m_pMaterials[No].Ambient.y = d3dxMaterials[No].MatD3D.Ambient.g;
		m_pMaterials[No].Ambient.z = d3dxMaterials[No].MatD3D.Ambient.b;
		m_pMaterials[No].Ambient.w = d3dxMaterials[No].MatD3D.Ambient.a;
		// �f�B�t���[�Y.
		m_pMaterials[No].Diffuse.x = d3dxMaterials[No].MatD3D.Diffuse.r;
		m_pMaterials[No].Diffuse.y = d3dxMaterials[No].MatD3D.Diffuse.g;
		m_pMaterials[No].Diffuse.z = d3dxMaterials[No].MatD3D.Diffuse.b;
		m_pMaterials[No].Diffuse.w = d3dxMaterials[No].MatD3D.Diffuse.a;
		// �X�y�L����.
		m_pMaterials[No].Specular.x = d3dxMaterials[No].MatD3D.Specular.r;
		m_pMaterials[No].Specular.y = d3dxMaterials[No].MatD3D.Specular.g;
		m_pMaterials[No].Specular.z = d3dxMaterials[No].MatD3D.Specular.b;
		m_pMaterials[No].Specular.w = d3dxMaterials[No].MatD3D.Specular.a;

		// (���̖ʂ�)�e�N�X�`�����\���Ă��邩�H.
		if( d3dxMaterials[No].pTextureFilename != nullptr &&
			lstrlen(d3dxMaterials[No].pTextureFilename) > 0 ){

			char path[128] = "";
			int path_count = lstrlen(fileName);
			for( int k = path_count; k >= 0; k-- ){
				if (fileName[k] == '\\' ){
					for( int j = 0; j <= k; j++ ) path[j] = fileName[j];
					path[k + 1] = '\0';
					break;
				}
			}
			// �p�X�ƃe�N�X�`���t�@�C������A��.
			strcat_s( path, sizeof(path), d3dxMaterials[No].pTextureFilename );

			// �e�N�X�`���t�@�C�������R�s�[.
			strcpy_s( m_pMaterials[No].szTextureName, sizeof(m_pMaterials[No].szTextureName), path );

			// �e�N�X�`���쐬.
			if( FAILED( D3DX11CreateShaderResourceViewFromFile(
				m_pDevice11, 
				m_pMaterials[No].szTextureName,	// �e�N�X�`���t�@�C����.
				nullptr, 
				nullptr,
				&m_pMaterials[No].pTexture,		// (out)�e�N�X�`���I�u�W�F�N�g.
				nullptr ))){
				std::string text = m_pMaterials[No].szTextureName;
				ERROR_MESSAGE( text + " �e�N�X�`���Ǎ� : ���s" );
				return E_FAIL;
			}

			// �@���e�N�X�`�����擾.
			std::string normTexName = m_pMaterials[No].szTextureName;
			size_t i = normTexName.find(".");
			normTexName.insert( i , "_norm" );
			if( FAILED( D3DX11CreateShaderResourceViewFromFile(
				m_pDevice11, 
				normTexName.c_str(),	// �e�N�X�`���t�@�C����.
				nullptr, 
				nullptr,
				&m_pMaterials[No].pNormTexture,		// (out)�e�N�X�`���I�u�W�F�N�g.
				nullptr ))){
				m_pMaterials[No].pNormTexture = nullptr;
			}
		} else {
			std::string text = fileName;
			// �e�N�X�`���쐬.
			if( FAILED( D3DX11CreateShaderResourceViewFromFile(
				m_pDevice11, 
				"Data\\Mesh\\noTex.png",	// �e�N�X�`���t�@�C����.
				nullptr, 
				nullptr,
				&m_pMaterials[No].pTexture,	// (out)�e�N�X�`���I�u�W�F�N�g.
				nullptr ))){
				ERROR_MESSAGE( text + " �e�N�X�`���Ǎ� : ���s" );
				return E_FAIL;
			}
			text += "�Ƀe�N�X�`�����Ȃ����ߕʂ̃e�N�X�`�����������݂܂��B";
			CLog::Print( text.c_str() );
		}
	}

	//------------------------------------------------
	//	�C���f�b�N�X�o�b�t�@�쐬.
	//------------------------------------------------
	// ���b�V���̑������𓾂�.
	// �������ŃC���f�b�N�X�o�b�t�@����ׂ����}�e���A�����Ƃ̃C���f�b�N�X�o�b�t�@�𕪗��ł���.
	D3DXATTRIBUTERANGE* pAttrTable = nullptr;

	// ���b�V���̖ʂ���ђ��_�̏��ԕύX�𐧌䂵�A�p�t�H�[�}���X���œK������.
	// D3DXMESHOPT_COMPACT : �ʂ̏��Ԃ�ύX���A�g�p����Ă��Ȃ����_�Ɩʂ��폜����.
	// D3DXMESHOPT_ATTRSORT : �p�t�H�[�}���X���グ��ׁA�ʂ̏��Ԃ�ύX���čœK�����s��.
	m_pMesh->OptimizeInplace( D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT, nullptr, nullptr, nullptr, nullptr);
	// �����e�[�u���̎擾.
	m_pMesh->GetAttributeTable( nullptr, &m_NumAttr );
	pAttrTable = new D3DXATTRIBUTERANGE[m_NumAttr];
	if( FAILED( m_pMesh->GetAttributeTable( pAttrTable, &m_NumAttr ) )){
		ERROR_MESSAGE( "����ð��َ擾���s" );
		return E_FAIL;
	}

	// ������Lock���Ȃ��Ǝ��o���Ȃ�.
	int* pIndex = nullptr;
	m_pMesh->LockIndexBuffer( D3DLOCK_READONLY, (void**)&pIndex );
	// �������Ƃ̃C���f�b�N�X�o�b�t�@���쐬.
	for( DWORD No = 0; No < m_NumAttr; No++ ){
		m_AttrID[No] = pAttrTable[No].AttribId;
		// Dx9�̃C���f�b�N�X�o�b�t�@����̏��ŁADx11�̃C���f�b�N�X�o�b�t�@���쐬.
		bd.Usage			= D3D11_USAGE_DEFAULT;
		bd.ByteWidth		= sizeof(int)*pAttrTable[No].FaceCount * 3;//�ʐ��~3�Œ��_��.
		bd.BindFlags		= D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags	= 0;
		bd.MiscFlags		= 0;
		// �傫���C���f�b�N�X�o�b�t�@���̃I�t�Z�b�g(�~3����).
		InitData.pSysMem = &pIndex[pAttrTable[No].FaceStart*3];

		if( FAILED( m_pDevice11->CreateBuffer( &bd, &InitData, &m_ppIndexBuffer[No] ) )){
			ERROR_MESSAGE( "�C���f�b�N�X�o�b�t�@�쐬 : ���s" );
			return E_FAIL;
		}
		// �ʂ̐����R�s�[.
		m_pMaterials[m_AttrID[No]].dwNumFace = pAttrTable[No].FaceCount;
	}
	// �����e�[�u���̍폜.
	delete[] pAttrTable;
	// �g�p�ς݂̃C���f�b�N�X�̉��.
	m_pMesh->UnlockIndexBuffer();
	// �s�v�ɂȂ����}�e���A���o�b�t�@�����.
	SAFE_RELEASE( pD3DXMtrlBuffer );

	//------------------------------------------------
	//	���_�o�b�t�@�̍쐬.
	//------------------------------------------------
	// Dx9�̏ꍇ�Amap�ł͂Ȃ�Lock�Œ��_�o�b�t�@����f�[�^�����o��.
	LPDIRECT3DVERTEXBUFFER9 pVB = nullptr;
	m_pMesh->GetVertexBuffer(&pVB);
	DWORD dwStride = m_pMesh->GetNumBytesPerVertex();
	BYTE* pVertices = nullptr;
	VERTEX* pVertex = nullptr;
	if( SUCCEEDED( pVB->Lock(0, 0, (VOID**)&pVertices, 0) )){
		pVertex = (VERTEX*)pVertices;
		// Dx9�̒��_�o�b�t�@����̏��ŁADx11���_�o�b�t�@���쐬.
		bd.Usage			= D3D11_USAGE_DEFAULT;
		bd.ByteWidth		= m_pMesh->GetNumBytesPerVertex()*m_pMesh->GetNumVertices();
		bd.BindFlags		= D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags	= 0;
		bd.MiscFlags		= 0;
		InitData.pSysMem	= pVertex;
		if( FAILED( m_pDevice11->CreateBuffer( &bd, &InitData, &m_pVertexBuffer ) )){
			ERROR_MESSAGE( "���_�o�b�t�@�쐬 : ���s" );
			return E_FAIL;
		}
		pVB->Unlock();
	}
	SAFE_RELEASE(pVB);	// ���_�o�b�t�@���.


	// �e�N�X�`���p�̃T���v���\����.
	D3D11_SAMPLER_DESC samDesc;
	ZeroMemory( &samDesc, sizeof(samDesc) );
	samDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;//�Ʊ̨��(���`���).
	samDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	// �T���v���쐬.
	if( FAILED( m_pDevice11->CreateSamplerState( &samDesc, &m_pSampleLinear ))){
		ERROR_MESSAGE( "�T���v���쐬 : ���s" );
		return E_FAIL;
	}


	return S_OK;
}

// ����֐�.
void CDX9StaticMesh::Release()
{
	// �C���f�b�N�X�o�b�t�@���.
	if( m_ppIndexBuffer != nullptr ){
		for( int No = m_NumMaterials-1; No >= 0; No-- ){
			if( m_ppIndexBuffer[No] != nullptr ) SAFE_RELEASE( m_ppIndexBuffer[No] );
		}
		SAFE_DELETE_ARRAY( m_ppIndexBuffer );
	}
	SAFE_DELETE_ARRAY( m_pMaterials );
	SAFE_RELEASE( m_pMeshForRay );
	SAFE_RELEASE( m_pMesh );
	SAFE_RELEASE( m_pSampleLinear );
	SAFE_RELEASE( m_pVertexBuffer );
	SAFE_RELEASE( m_pCBufferPerMaterial );
	SAFE_RELEASE( m_pCBufferPerMesh );
	SAFE_RELEASE( m_pPixelShader );
	SAFE_RELEASE( m_pVertexLayout );
	SAFE_RELEASE( m_pVertexShader );

	m_pDevice9		= nullptr;
	m_pContext11	= nullptr;
	m_pDevice11		= nullptr;
	m_hWnd			= nullptr;
}

//===========================================================
//	HLSĻ�ق�ǂݍ��ݼ���ނ��쐬����.
//	HLSL: High Level Shading Language �̗�.
//===========================================================
HRESULT CDX9StaticMesh::InitShader()
{
	ID3DBlob* pCompiledShader = nullptr;
	ID3DBlob* pErrors = nullptr;
	UINT uCompileFlag = 0;
#ifdef _DEBUG
	uCompileFlag = D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION;
#endif	// #ifdef _DEBUG

	// HLSL���璸�_�V�F�[�_�[�̃u���u���쐬.
	if( FAILED(
		shader::InitShader(
			SHADER_VS_NAME, 
			"VS_Main",
			"vs_5_0",
			uCompileFlag, 
			&pCompiledShader, 
			&pErrors ))){
		ERROR_MESSAGE( shader::GetBlobErrorMsg( pErrors ) );
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);

	// ��L�ō쐬�����u���u����u���_�V�F�[�_�[�v���쐬.
	if( FAILED(
		shader::CreateVertexShader( m_pDevice11, pCompiledShader, &m_pVertexShader ))){
		ERROR_MESSAGE( "���_�V�F�[�_�[�쐬���s" );
		return E_FAIL;
	}

	// ���_�C���v�b�g���C�A�E�g���`.
	// ���_�C���v�b�g���C�A�E�g�̔z��v�f�����Z�o.
	UINT numElements = 0;
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"NORMAL",  0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,   0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"TANGENT", 0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"BINORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
	};
	numElements = sizeof(layout) / sizeof(layout[0]);	// �v�f���Z�o.

	// ���_�C���v�b�g���C�A�E�g���쐬.
	if( FAILED(
		shader::CreateInputLayout( 
			m_pDevice11, 
			layout, 
			numElements, 
			pCompiledShader, 
			&m_pVertexLayout ))){
		ERROR_MESSAGE( "���_�C���v�b�g���C�A�E�g�쐬���s" );
		return E_FAIL;
	}
	SAFE_RELEASE(pCompiledShader);

	// HLSL����s�N�Z���V�F�[�_�[�̃u���u���쐬.
	if( FAILED(
		shader::InitShader(
			SHADER_PS_NAME,
			"PS_Main",
			"ps_5_0",
			uCompileFlag,
			&pCompiledShader,
			&pErrors ))){
		ERROR_MESSAGE( shader::GetBlobErrorMsg( pErrors ) );
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);

	// ��L�ō쐬�����u���u����u�s�N�Z���V�F�[�_�[�v���쐬.
	if( FAILED(
		shader::CreatePixelShader( m_pDevice11, pCompiledShader, &m_pPixelShader ))){
		ERROR_MESSAGE( "�s�N�Z���V�F�[�_�[�쐬���s" );
		return E_FAIL;
	}
	SAFE_RELEASE(pCompiledShader);

	if( FAILED( shader::CreateConstantBuffer( m_pDevice11, &m_pCBufferPerMesh,		sizeof(CBUFFER_PER_MESH) )))	return E_FAIL;
	if( FAILED( shader::CreateConstantBuffer( m_pDevice11, &m_pCBufferPerMaterial,	sizeof(CBUFFER_PER_MATERIAL) )))return E_FAIL;
	if( FAILED( shader::CreateConstantBuffer( m_pDevice11, &m_pCBufferPerFrame,		sizeof(CBUFFER_PER_FRAME) )))	return E_FAIL;

	return S_OK;
}

// �`��֐�.
void CDX9StaticMesh::Render()
{
	// ���[���h�s��.
	D3DXMATRIX mWorld = m_Tranceform.GetWorldMatrix();

	// ���_�C���v�b�g���C�A�E�g���Z�b�g.
	m_pContext11->IASetInputLayout( m_pVertexLayout );
	if( m_pShadowMap->Render( false, mWorld, m_ShadowRenderFunc ) == true ) return;

	// �g�p����V�F�[�_�[�̃Z�b�g.
	m_pContext11->VSSetShader( m_pVertexShader, nullptr, 0 );

#if 0
	m_pContext11->PSSetShader( m_pPixelShader, nullptr, 0 );

	// �V�F�[�_�[�̃R���X�^���g�o�b�t�@�Ɋe��f�[�^��n��.
	D3D11_MAPPED_SUBRESOURCE pData;
	if( SUCCEEDED( m_pContext11->Map(
		m_pCBufferPerFrame, 
		0,
		D3D11_MAP_WRITE_DISCARD,
		0, &pData ))) {
		CBUFFER_PER_FRAME cb;	// �R���X�^���g�o�b�t�@.

		// �J�����ʒu.
		D3DXVECTOR3 camPos = CCameraManager::GetPosition();
		cb.vCamPos = D3DXVECTOR4( camPos.x, camPos.y, camPos.z, 1.0f );

		//----- ���C�g��� -----.
		// ���C�g����.
		D3DXVECTOR3 lightDir = CLightManager::GetDirection();
		cb.vLightDir = D3DXVECTOR4( lightDir.x, lightDir.y, lightDir.z, 1.0f );
		// ���C�g���x(���邳).
		cb.vIntensity.x = CLightManager::GetIntensity();
		// ���C�g�����̐��K��(�m�[�}���C�Y).
		D3DXVec4Normalize( &cb.vLightDir, &cb.vLightDir );

		memcpy_s( pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb) );

		// �o�b�t�@���̃f�[�^�̏��������I������Unmap.
		m_pContext11->Unmap(m_pCBufferPerFrame, 0);
	}

	m_pContext11->VSSetConstantBuffers( 2, 1, &m_pCBufferPerFrame);
	m_pContext11->PSSetConstantBuffers( 2, 1, &m_pCBufferPerFrame);

#endif

	D3DXMATRIX mView = CCameraManager::GetViewMatrix();
	D3DXMATRIX mProj = CCameraManager::GetProjMatrix();

	RenderMesh( mWorld, mView, mProj );
}

// ���b�V���̕`��.
void CDX9StaticMesh::RenderMesh(
	D3DXMATRIX& mWorld, 
	const D3DXMATRIX& mView, 
	const D3DXMATRIX& mProj )
{
	// �V�F�[�_�[�̃R���X�^���g�o�b�t�@�Ɋe��f�[�^��n��.
	D3D11_MAPPED_SUBRESOURCE pData;
	if( SUCCEEDED( m_pContext11->Map(
		m_pCBufferPerMesh, 
		0,
		D3D11_MAP_WRITE_DISCARD,
		0, &pData ))){

		CBUFFER_PER_MESH cb;	// �R���X�^���g�o�b�t�@

		// ���[���h�s���n��.
		cb.mW = mWorld;
		D3DXMatrixTranspose( &cb.mW, &cb.mW );

		// ���[���h�E�r���[�E�v���W�F�N�V�����s���n��.
		cb.mWVP = mWorld * mView * mProj;
		D3DXMatrixTranspose( &cb.mWVP, &cb.mWVP );

		// �F��n��.
		cb.vColor = m_Color;

		// �e�̔Z����n��.
		cb.vShadowDepth.x = 1.0f - m_ShadowDepth;

		memcpy_s( pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));

		// �o�b�t�@���̃f�[�^�̏��������I������Unmap.
		m_pContext11->Unmap( m_pCBufferPerMesh, 0 );
	}


	m_pContext11->VSSetConstantBuffers( 0, 1, &m_pCBufferPerMesh );
	m_pContext11->PSSetConstantBuffers( 0, 1, &m_pCBufferPerMesh );

	// �v���~�e�B�u�E�g�|���W�[���Z�b�g.
	m_pContext11->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	// ���_�o�b�t�@���Z�b�g.
	UINT stride = m_pMesh->GetNumBytesPerVertex();
	UINT offset = 0;
	m_pContext11->IASetVertexBuffers( 0, 1, &m_pVertexBuffer, &stride, &offset );

	// �����̐������A���ꂼ��̑����̃C���f�b�N�X�o�b�t�@��`��.
	for( DWORD No = 0; No < m_NumAttr; No++ ){
		// �g�p����Ă��Ȃ��}�e���A���΍�.
		if( m_pMaterials[m_AttrID[No]].dwNumFace == 0 ) continue;
		// �C���f�b�N�X�o�b�t�@���Z�b�g.
		m_pContext11->IASetIndexBuffer( m_ppIndexBuffer[No], DXGI_FORMAT_R32_UINT, 0 );
		// �}�e���A���̊e�v�f���V�F�[�_�[�ɓn��.
		D3D11_MAPPED_SUBRESOURCE pDataMat;
		if( SUCCEEDED( m_pContext11->Map(
			m_pCBufferPerMaterial,
			0, 
			D3D11_MAP_WRITE_DISCARD, 
			0, 
			&pDataMat ))){

			CBUFFER_PER_MATERIAL cb;

			// �A���r�G���g,�f�B�t���[�Y��,�X�y�L�����V�F�[�_�[�ɓn��.
			cb.vAmbient = m_pMaterials[m_AttrID[No]].Ambient;
			cb.vDiffuse = m_pMaterials[m_AttrID[No]].Diffuse;
			cb.vSpecular = m_pMaterials[m_AttrID[No]].Specular;

			memcpy_s( pDataMat.pData, pDataMat.RowPitch, (void*)&cb, sizeof(cb) );

			m_pContext11->Unmap(m_pCBufferPerMaterial, 0);
		}

		m_pContext11->VSSetConstantBuffers( 1, 1, &m_pCBufferPerMaterial );
		m_pContext11->PSSetConstantBuffers( 1, 1, &m_pCBufferPerMaterial );

		m_pContext11->PSSetSamplers( 0, 1, &m_pSampleLinear );
		// �e�N�X�`�����V�F�[�_�[�ɓn��.
		ID3D11ShaderResourceView* pTex = { 0 };
		m_pContext11->PSSetShaderResources( 0, 1, &m_pMaterials[m_AttrID[No]].pTexture );
		m_pContext11->PSSetShaderResources( 1, 1, &m_pMaterials[m_AttrID[No]].pNormTexture );

		// �|�����S���������_�����O.
		m_pContext11->DrawIndexed( m_pMaterials[m_AttrID[No]].dwNumFace * 3, 0, 0);
		ID3D11ShaderResourceView* resetSrv = nullptr;
		m_pContext11->PSSetShaderResources( 0, 1, &resetSrv );
		m_pContext11->PSSetShaderResources( 1, 1, &resetSrv );
	}
}
