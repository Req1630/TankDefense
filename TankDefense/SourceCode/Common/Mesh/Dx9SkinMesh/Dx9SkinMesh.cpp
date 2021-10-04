/***************************************************************************************************
*	SkinMeshCode Version 2.00
*	LastUpdate	: 2019/10/09.
*		�p�[�T�[�N���X�ƃX�L�����b�V���N���X��ʃt�@�C���ɕ���.
**/
#include "Dx9SkinMesh.h"
#include "..\..\Shader\Shader.h"
#include "..\..\..\Object\CameraBase\CameraManager\CameraManager.h"
#include "..\..\..\Object\LightBase\LightManager\LightManager.h"
#include "..\..\RenderingTextuer\CascadedShadowMap\CascadedShadowMap.h"

//�V�F�[�_��(�f�B���N�g�����܂�)
const char SHADER_VS_NAME[] = "Data\\Shader\\SkinMesh.hlsl";
const char SHADER_PS_NAME[] = "Data\\Shader\\MeshPS.hlsl";

/******************************************************************************************************************************************
*
*	�ȍ~�A�X�L�����b�V���N���X.
*
**/
//�R���X�g���N�^.
CDX9SkinMesh::CDX9SkinMesh()
	: m_hWnd(nullptr)
	, m_pDevice9(nullptr)
	, m_pSampleLinear(nullptr)
	, m_pVertexShader(nullptr)
	, m_pPixelShader(nullptr)
	, m_pVertexLayout(nullptr)
	, m_pCBufferPerMesh(nullptr)
	, m_pCBufferPerMaterial(nullptr)
	, m_pCBufferPerFrame(nullptr)
	, m_pCBufferPerBone(nullptr)
	, m_mWorld()
	, m_mRotation()
	, m_mView()
	, m_mProj()
	, m_CameraPos()
	, m_CameraLookPos()
	, m_dAnimSpeed(0.0001f)	//��悸�A���̒l.
	, m_dAnimTime()
	, m_IsChangeAnim(false)
	, m_pReleaseMaterial(nullptr)
	, m_pD3dxMesh(nullptr)
	, m_FilePath()
	, m_iFrame()
	, m_ShadowDepth(0.5f)
	, m_ShadowRenderFunc()
{
	m_pShadowMap = CCascadedShadowMap::GetInstance();
	m_ShadowRenderFunc = [&](SKIN_PARTS_MESH* pMesh)
	{ 
		//�A�j���[�V�����t���[����i�߂� �X�L�����X�V.
		D3D11_MAPPED_SUBRESOURCE pData;
		if( SUCCEEDED(
			m_pContext11->Map(
			m_pCBufferPerBone, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData ) ) )
		{
			CBUFFER_PER_BONES cb;
			for( int i=0; i<pMesh->iNumBone; i++ )
			{
				D3DXMATRIX mat = GetCurrentPoseMatrix( pMesh, i );
				D3DXMatrixTranspose( &mat, &mat );
				cb.mBone[i] = mat;
			}
			memcpy_s( pData.pData, pData.RowPitch, (void*)&cb, sizeof( cb ) );
			m_pContext11->Unmap(m_pCBufferPerBone, 0 );
		}
		m_pContext11->VSSetConstantBuffers(	1, 1, &m_pCBufferPerBone);
		m_pContext11->PSSetConstantBuffers(	1, 1, &m_pCBufferPerBone);


		//���_�ޯ̧���.
		UINT stride = sizeof( MY_SKINVERTEX );
		UINT offset = 0;
		m_pContext11->IASetVertexBuffers(
			0, 1, &pMesh->pVertexBuffer, &stride, & offset );
		//����è�ށE���ۼް���.
		m_pContext11->IASetPrimitiveTopology(
			D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//�����̐������A���ꂼ��̑����̲��ޯ���ޯ̧��`��.
		for( DWORD i=0; i<pMesh->dwNumMaterial; i++ )
		{
			m_pContext11->IASetIndexBuffer(
				pMesh->ppIndexBuffer[i], DXGI_FORMAT_R32_UINT, 0 );
			//����è��(��غ��)�������ݸ�.
			m_pContext11->DrawIndexed( pMesh->pMaterial[i].dwNumFace * 3, 0, 0 );
		}
	};
}


//�f�X�g���N�^.
CDX9SkinMesh::~CDX9SkinMesh()
{
	//�������.
	Release();

	//�V�F�[�_��T���v���֌W.
	SAFE_RELEASE( m_pSampleLinear );
	SAFE_RELEASE( m_pVertexShader );
	SAFE_RELEASE( m_pPixelShader );
	SAFE_RELEASE( m_pVertexLayout );

	//�R���X�^���g�o�b�t�@�֌W.
	SAFE_RELEASE(m_pCBufferPerBone);
	SAFE_RELEASE(m_pCBufferPerFrame);
	SAFE_RELEASE(m_pCBufferPerMaterial);
	SAFE_RELEASE(m_pCBufferPerMesh);

	m_pReleaseMaterial = nullptr;

	SAFE_RELEASE(m_pD3dxMesh);

	//Dx9 �f�o�C�X�֌W.
	m_pDevice9 = nullptr;

	//Dx11 �f�o�C�X�֌W.
	m_pContext11 = nullptr;
	m_pDevice11 = nullptr;

	m_hWnd = nullptr;
}


//������.
HRESULT CDX9SkinMesh::Init(
	HWND hWnd, 
	ID3D11Device* pDevice11,
	ID3D11DeviceContext* pContext11,
	LPDIRECT3DDEVICE9 pDevice9,
	const char* fileName )
{
	m_hWnd = hWnd;
	m_pDevice9 = pDevice9;
	if( FAILED( SetDevice( pContext11 ))) return E_FAIL;
	//�V�F�[�_�̍쐬.
	if( FAILED( InitShader() ) ) return E_FAIL;
	
	//���f���ǂݍ���.
	if( FAILED(LoadXMesh(fileName)))return E_FAIL;
	
	std::string msg = fileName;
	msg += " �ǂݍ��� : ����";
	CLog::Print( std::string( msg.c_str() ).c_str() );

	return S_OK;
}

//�V�F�[�_������.
HRESULT	CDX9SkinMesh::InitShader()
{
	//D3D11�֘A�̏�����
	ID3DBlob *pCompiledShader = nullptr;
	ID3DBlob *pErrors = nullptr;
	UINT	uCompileFlag = 0;
#ifdef _DEBUG
	uCompileFlag = D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION;
#endif//#ifdef _DEBUG

	//�u���u����o�[�e�b�N�X�V�F�[�_�[�쐬.
	if( FAILED(
		shader::InitShader(
			SHADER_VS_NAME,
			"VS_Main",
			"vs_5_0",
			uCompileFlag,
			&pCompiledShader,
			&pErrors ) ) )
	{
		ERROR_MESSAGE( shader::GetBlobErrorMsg( pErrors ) );
		return E_FAIL;
	}
	SAFE_RELEASE( pErrors );

	if( FAILED( shader::CreateVertexShader( m_pDevice11, pCompiledShader, &m_pVertexShader ) ) )
	{
		SAFE_RELEASE(pCompiledShader);
		ERROR_MESSAGE("�o�[�e�b�N�X�V�F�[�_�[�쐬���s");
		return E_FAIL;
	}
	//���_�C���v�b�g���C�A�E�g���`	
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }, 
		{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BINORMAL",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BONE_INDEX",	0, DXGI_FORMAT_R32G32B32A32_UINT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BONE_WEIGHT",0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = sizeof( layout ) / sizeof( layout[0] );

	//���_�C���v�b�g���C�A�E�g���쐬
	if( FAILED(
		shader::CreateInputLayout(
		m_pDevice11,
		layout, 
		numElements, 
		pCompiledShader,
		&m_pVertexLayout ) ) )
	{
		return FALSE;
	}

	//�u���u����s�N�Z���V�F�[�_�[�쐬
	if(FAILED(
		shader::InitShader(
			SHADER_PS_NAME,
			"PS_Main", 
			"ps_5_0",
			uCompileFlag,
			&pCompiledShader, 
			&pErrors ) ) )
	{
		ERROR_MESSAGE( shader::GetBlobErrorMsg( pErrors ) );
		return E_FAIL;
	}
	SAFE_RELEASE( pErrors );
	if( FAILED( shader::CreatePixelShader( m_pDevice11, pCompiledShader, &m_pPixelShader ) ) )
	{
		SAFE_RELEASE( pCompiledShader );
		ERROR_MESSAGE("�s�N�Z���V�F�[�_�[�쐬���s");
		return E_FAIL;
	}
	SAFE_RELEASE( pCompiledShader );

	//�R���X�^���g�o�b�t�@(���b�V������).
	if (FAILED(CreateCBuffer(&m_pCBufferPerMesh, sizeof(CBUFFER_PER_MESH))))	{	return E_FAIL;	}
	//�R���X�^���g�o�b�t�@(���b�V������).
	if (FAILED(CreateCBuffer(&m_pCBufferPerMaterial, sizeof(CBUFFER_PER_MATERIAL)))){	return E_FAIL;	}
	//�R���X�^���g�o�b�t�@(���b�V������).
	if (FAILED(CreateCBuffer(&m_pCBufferPerFrame, sizeof(CBUFFER_PER_FRAME))))	{	return E_FAIL;	}
	//�R���X�^���g�o�b�t�@(���b�V������).
	if (FAILED(CreateCBuffer(&m_pCBufferPerBone, sizeof(CBUFFER_PER_BONES))))	{	return E_FAIL;	}

	//�e�N�X�`���[�p�T���v���[�쐬.
	if (FAILED(CreateLinearSampler(&m_pSampleLinear))) {
		return E_FAIL;
	}

	return S_OK;
}



//X�t�@�C������X�L���֘A�̏���ǂݏo���֐�.
HRESULT CDX9SkinMesh::ReadSkinInfo(
	MYMESHCONTAINER* pContainer, MY_SKINVERTEX* pvVB, SKIN_PARTS_MESH* pParts )
{
	//X�t�@�C�����璊�o���ׂ����́A
	//�u���_���Ƃ��ްݲ��ޯ���v�u���_���Ƃ̃{�[���E�F�C�g�v.
	//�u�o�C���h�s��v�u�|�[�Y�s��v��4����.

	int i, k, m, n;	//�e�탋�[�v�ϐ�.
	int iNumVertex	= 0;	//���_��.
	int iNumBone	= 0;	//�{�[����.

	//���_��.
	iNumVertex	= m_pD3dxMesh->GetNumVertices( pContainer );
	//�{�[����.
	iNumBone	= m_pD3dxMesh->GetNumBones( pContainer );

	//���ꂼ��̃{�[���ɉe�����󂯂钸�_�𒲂ׂ�.
	//��������t�ɁA���_�x�[�X�Ń{�[���C���f�b�N�X�E�d�݂𐮓ڂ���.
	for( i=0; i<iNumBone; i++ )
	{
		//���̃{�[���ɉe�����󂯂钸�_��.
		int iNumIndex
			= m_pD3dxMesh->GetNumBoneVertices( pContainer, i);

		int*	pIndex = new int[iNumIndex]();
		double*	pWeight= new double[iNumIndex]();

		for( k=0; k<iNumIndex; k++ )
		{
			pIndex[k]
				= m_pD3dxMesh->GetBoneVerticesIndices( pContainer, i, k );
			pWeight[k]
				= m_pD3dxMesh->GetBoneVerticesWeights( pContainer, i, k );
		}

		//���_������C���f�b�N�X�����ǂ��āA���_�T�C�h�Ő�������.
		for( k=0; k<iNumIndex; k++ )
		{
			//X�t�@�C����CG�\�t�g���{�[��4�{�ȓ��Ƃ͌���Ȃ�.
			//5�{�ȏ�̏ꍇ�́A�d�݂̑傫������4�{�ɍi��.

			//�E�F�C�g�̑傫�����Ƀ\�[�g(�o�u���\�[�g).
			for( m=4; m>1; m-- )
			{
				for( n=1; n<m; n++ )
				{
					if( pvVB[pIndex[k]].bBoneWeight[n-1] < pvVB[pIndex[k]].bBoneWeight[n] )
					{
						float tmp = pvVB[pIndex[k]].bBoneWeight[n-1];
						pvVB[pIndex[k]].bBoneWeight[n-1] = pvVB[pIndex[k]].bBoneWeight[n];
						pvVB[pIndex[k]].bBoneWeight[n]	= tmp;
						int itmp = pvVB[pIndex[k]].bBoneIndex[n-1];
						pvVB[pIndex[k]].bBoneIndex[n-1] = pvVB[pIndex[k]].bBoneIndex[n];
						pvVB[pIndex[k]].bBoneIndex[n]	= itmp;
					}
				}
			}
			//�\�[�g��́A�Ō�̗v�f�Ɉ�ԏ������E�F�C�g�������Ă�͂�.
			bool flag = false;
			for( m=0; m<4; m++ )
			{
				if( pvVB[pIndex[k]].bBoneWeight[m] == 0 )
				{
					flag = true;
					pvVB[pIndex[k]].bBoneIndex[ m] = i;
					pvVB[pIndex[k]].bBoneWeight[m] = (float)pWeight[k];
					break;
				}
			}
			if( flag == false )
			{
				pvVB[pIndex[k]].bBoneIndex[ 3] = i;
				pvVB[pIndex[k]].bBoneWeight[3] = (float)pWeight[k];
				break;
			}
		}
		//�g���I���΍폜����.
		delete[] pIndex;
		delete[] pWeight;
	}

	//�{�[������.
	pParts->iNumBone	= iNumBone;
	pParts->pBoneArray	= new BONE[iNumBone]();
	//�|�[�Y�s��𓾂�(�����|�[�Y).
	for( i=0; i<pParts->iNumBone; i++ )
	{
		pParts->pBoneArray[i].mBindPose
			= m_pD3dxMesh->GetBindPose( pContainer, i );
	}

	return S_OK;
}

//X����X�L�����b�V�����쐬����.
//	���Ӂj�f�ށiX)�̂ق��́A�O�p�|���S���ɂ��邱��.
HRESULT CDX9SkinMesh::LoadXMesh( const char* fileName )
{
	//�t�@�C�������p�X���Ǝ擾.
	strcpy_s( m_FilePath, sizeof( m_FilePath ), fileName);

	//X�t�@�C���ǂݍ���.
	m_pD3dxMesh = new D3DXPARSER();
	m_pD3dxMesh->LoadMeshFromX( m_pDevice9, fileName);


	//�S�Ẵ��b�V�����쐬����.
	BuildAllMesh( m_pD3dxMesh->m_pFrameRoot );

	return S_OK;
}


//Direct3D�̃C���f�b�N�X�o�b�t�@�[�쐬.
HRESULT CDX9SkinMesh::CreateIndexBuffer( DWORD dwSize, int* pIndex, ID3D11Buffer** ppIndexBuffer )
{
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = dwSize;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = pIndex;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;
	if( FAILED( m_pDevice11->CreateBuffer( &bd, &InitData, ppIndexBuffer ) ) )
	{
		return FALSE;
	}
	
	return S_OK;
}

//�����_�����O.
void CDX9SkinMesh::Render( SAnimationController* pAC, const D3DXMATRIX* pRot )
{
	m_mView		= CCameraManager::GetViewMatrix();
	m_mProj		= CCameraManager::GetProjMatrix();
	m_CameraPos = CCameraManager::GetPosition();
	m_CameraLookPos = CCameraManager::GetLookPosition();

	// �A�j���[�V�����̍X�V.
	AnimUpdate( pAC );

	D3DXMATRIX m;
	D3DXMatrixIdentity( &m );
	m_pD3dxMesh->UpdateFrameMatrices( m_pD3dxMesh->m_pFrameRoot, &m );
	DrawFrame( m_pD3dxMesh->m_pFrameRoot, pRot );
}


//�S�Ẵ��b�V�����쐬����.
void CDX9SkinMesh::BuildAllMesh( D3DXFRAME* pFrame )
{
	if( pFrame && pFrame->pMeshContainer )
	{
		CreateAppMeshFromD3DXMesh( pFrame );
	}

	//�ċA�֐�.
	if( pFrame->pFrameSibling != nullptr )
	{
		BuildAllMesh( pFrame->pFrameSibling );
	}
	if( pFrame->pFrameFirstChild != nullptr )
	{
		BuildAllMesh( pFrame->pFrameFirstChild );
	}
}

//���b�V���쐬.
HRESULT CDX9SkinMesh::CreateAppMeshFromD3DXMesh( LPD3DXFRAME p )
{
	MYFRAME* pFrame = (MYFRAME*)p;

//	LPD3DXMESH pD3DXMesh = pFrame->pMeshContainer->MeshData.pMesh;//D3DXү��(��������E�E�E).
	m_pMeshForRay= pFrame->pMeshContainer->MeshData.pMesh; //D3DXү��(��������E�E�E).
	MYMESHCONTAINER* pContainer = (MYMESHCONTAINER*)pFrame->pMeshContainer;

	//�A�v�����b�V��(�E�E�E�����Ƀ��b�V���f�[�^���R�s�[����).
	SKIN_PARTS_MESH* pAppMesh = new SKIN_PARTS_MESH();
	pAppMesh->bTex = false;

	//���O�ɒ��_���A�|���S�������𒲂ׂ�.
	pAppMesh->dwNumVert	= m_pD3dxMesh->GetNumVertices( pContainer );
	pAppMesh->dwNumFace	= m_pD3dxMesh->GetNumFaces( pContainer );
	pAppMesh->dwNumUV	= m_pD3dxMesh->GetNumUVs( pContainer );
	//Direct3D�ł�UV�̐��������_���K�v.
	if( pAppMesh->dwNumVert < pAppMesh->dwNumUV ){
		//���L���_���ŁA���_������Ȃ��Ƃ�.
		MessageBox( NULL,
			"Direct3D�́AUV�̐��������_���K�v�ł�(UV��u���ꏊ���K�v�ł�)�e�N�X�`���͐������\���Ȃ��Ǝv���܂�",
			"Error", MB_OK );
		return E_FAIL;
	}
	//�ꎞ�I�ȃ������m��(���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@).
	MY_SKINVERTEX* pvVB = new MY_SKINVERTEX[pAppMesh->dwNumVert]();
	int* piFaceBuffer	= new int[pAppMesh->dwNumFace*3]();
	//3���_�|���S���Ȃ̂ŁA1�t�F�C�X=3���_(3�C���f�b�N�X).

	//���_�ǂݍ���.
	for( DWORD i=0; i<pAppMesh->dwNumVert; i++ ){
		D3DXVECTOR3 v	= m_pD3dxMesh->GetVertexCoord( pContainer, i );
		pvVB[i].vPos.x	= v.x;
		pvVB[i].vPos.y	= v.y;
		pvVB[i].vPos.z	= v.z;
	}
	//�|���S�����(���_�C���f�b�N�X)�ǂݍ���.
	for( DWORD i=0; i<pAppMesh->dwNumFace*3; i++ ){
		piFaceBuffer[i] = m_pD3dxMesh->GetIndex( pContainer, i );
	}
	//�@���ǂݍ���.
	for( DWORD i=0; i<pAppMesh->dwNumVert; i++ ){
		D3DXVECTOR3 v	= m_pD3dxMesh->GetNormal( pContainer, i );
		pvVB[i].vNorm.x	= v.x;
		pvVB[i].vNorm.y	= v.y;
		pvVB[i].vNorm.z	= v.z;
	}
	//�e�N�X�`�����W�ǂݍ���.
	for( DWORD i=0; i<pAppMesh->dwNumVert; i++ ){
		D3DXVECTOR2 v	= m_pD3dxMesh->GetUV( pContainer, i );
		pvVB[i].vTex.x	= v.x;
		pvVB[i].vTex.y	= v.y;
	}
	//tangent.
	for( DWORD i=0; i<pAppMesh->dwNumVert; i++ ){
		D3DXVECTOR3 v	= m_pD3dxMesh->GetTangent( pContainer, i );
		pvVB[i].Tangent.x	= v.x;
		pvVB[i].Tangent.y	= v.y;
		pvVB[i].Tangent.z	= v.z;
	}
	//binorm.
	for( DWORD i=0; i<pAppMesh->dwNumVert; i++ ){
		D3DXVECTOR3 v	= m_pD3dxMesh->GetBinormal( pContainer, i );
		pvVB[i].Binormal.x	= v.x;
		pvVB[i].Binormal.y	= v.y;
		pvVB[i].Binormal.z	= v.z;
	}
	//�}�e���A���ǂݍ���.
	pAppMesh->dwNumMaterial	= m_pD3dxMesh->GetNumMaterials( pContainer );
	pAppMesh->pMaterial		= new MY_SKINMATERIAL[pAppMesh->dwNumMaterial]();

	//�}�e���A���̐������C���f�b�N�X�o�b�t�@���쐬.
	pAppMesh->ppIndexBuffer = new ID3D11Buffer*[pAppMesh->dwNumMaterial]();
	//�|���Z�ł͂Ȃ��uID3D11Buffer*�v�̔z��Ƃ����Ӗ�.
	for( DWORD i=0; i<pAppMesh->dwNumMaterial; i++ )
	{
		//����(�A���r�G���g).
		pAppMesh->pMaterial[i].Ka.x	= m_pD3dxMesh->GetAmbient( pContainer, i ).y;
		pAppMesh->pMaterial[i].Ka.y	= m_pD3dxMesh->GetAmbient( pContainer, i ).z;
		pAppMesh->pMaterial[i].Ka.z	= m_pD3dxMesh->GetAmbient( pContainer, i ).w;
		pAppMesh->pMaterial[i].Ka.w	= m_pD3dxMesh->GetAmbient( pContainer, i ).x;
		//�g�U���ˌ�(�f�B�t���[�Y).
		pAppMesh->pMaterial[i].Kd.x	= m_pD3dxMesh->GetDiffuse( pContainer, i ).y;
		pAppMesh->pMaterial[i].Kd.y	= m_pD3dxMesh->GetDiffuse( pContainer, i ).z;
		pAppMesh->pMaterial[i].Kd.z	= m_pD3dxMesh->GetDiffuse( pContainer, i ).w;
		pAppMesh->pMaterial[i].Kd.w	= m_pD3dxMesh->GetDiffuse( pContainer, i ).x;
		//���ʔ��ˌ�(�X�y�L����).
		pAppMesh->pMaterial[i].Ks.x	= m_pD3dxMesh->GetSpecular( pContainer, i ).y;
		pAppMesh->pMaterial[i].Ks.y	= m_pD3dxMesh->GetSpecular( pContainer, i ).z;
		pAppMesh->pMaterial[i].Ks.z	= m_pD3dxMesh->GetSpecular( pContainer, i ).w;
		pAppMesh->pMaterial[i].Ks.w	= m_pD3dxMesh->GetSpecular( pContainer, i ).x;

		//�e�N�X�`��(�f�B�t���[�Y�e�N�X�`���̂�).
		char* name = m_pD3dxMesh->GetTexturePath( pContainer, i );
		if( name ){
			char* ret = strrchr( m_FilePath, '\\' );
			if( ret != NULL ){
				int check = ret - m_FilePath;
				char path[512];
				strcpy_s( path, 512, m_FilePath );
				path[check+1] = '\0';

				strcat_s( path, sizeof( path ), name );
				strcpy_s( pAppMesh->pMaterial[i].TextureName,
					sizeof( pAppMesh->pMaterial[i].TextureName ),
					path );
			}
		}
		//�e�N�X�`�����쐬.
		if( lstrlen(pAppMesh->pMaterial[i].TextureName) > 0){
			if( FAILED( D3DX11CreateShaderResourceViewFromFile(
				m_pDevice11,
				pAppMesh->pMaterial[i].
				TextureName,
				NULL, 
				NULL, 
				&pAppMesh->pMaterial[i].pTexture, 
				NULL )))
			{
				std::string text = pAppMesh->pMaterial[i].TextureName;
				ERROR_MESSAGE( text + " �e�N�X�`���Ǎ� : ���s" );
				return E_FAIL;
			}
			// �@���e�N�X�`�����擾.
			std::string normTexName = pAppMesh->pMaterial[i].TextureName;
			size_t normlIndex = normTexName.find(".");
			normTexName.insert( normlIndex , "_norm" );
			if( FAILED( D3DX11CreateShaderResourceViewFromFile(
				m_pDevice11, 
				normTexName.c_str(),	// �e�N�X�`���t�@�C����.
				nullptr, 
				nullptr,
				&pAppMesh->pMaterial[i].pNormTexture,		// (out)�e�N�X�`���I�u�W�F�N�g.
				nullptr ))){
				pAppMesh->pMaterial[i].pNormTexture = nullptr;
			}
		} else {
			std::string text = m_FilePath;
			// �e�N�X�`���쐬.
			if( FAILED( D3DX11CreateShaderResourceViewFromFile(
				m_pDevice11, 
				"Data\\Mesh\\noTex.png",	// �e�N�X�`���t�@�C����.
				nullptr, 
				nullptr,
				&pAppMesh->pMaterial[i].pTexture,	// (out)�e�N�X�`���I�u�W�F�N�g.
				nullptr ))){
				ERROR_MESSAGE( text + " �e�N�X�`���Ǎ� : ���s" );
				return E_FAIL;
			}
			text += "�Ƀe�N�X�`�����Ȃ����ߕʂ̃e�N�X�`�����������݂܂��B";
			CLog::Print( text.c_str() );
		}
		//���̃}�e���A���ł���C���f�b�N�X�z����̊J�n�C���f�b�N�X�𒲂ׂ�.
		//����ɃC���f�b�N�X�̌��𒲂ׂ�.
		int iCount = 0;
		int* pIndex = new int[pAppMesh->dwNumFace*3]();
			//�Ƃ肠�����A���b�V�����̃|���S�����Ń������m��.
			//(�����̂ۂ育�񂮂�[�Ղ͕K������ȉ��ɂȂ�).

		for( DWORD k=0; k<pAppMesh->dwNumFace; k++ )
		{
			//���� i==k �Ԗڂ̃|���S���̃}�e���A���ԍ��Ȃ�.
			if( i == m_pD3dxMesh->GeFaceMaterialIndex( pContainer, k ) )
			{
				//k�Ԗڂ̃|���S������钸�_�̃C���f�b�N�X.
				pIndex[iCount]
					= m_pD3dxMesh->GetFaceVertexIndex( pContainer, k, 0 );	//1��.
				pIndex[iCount+1]
					= m_pD3dxMesh->GetFaceVertexIndex( pContainer, k, 1 );	//2��.
				pIndex[iCount+2]
					= m_pD3dxMesh->GetFaceVertexIndex( pContainer, k, 2 );	//3��.
				iCount += 3;
			}
		}
		if( iCount > 0 ){
			//�C���f�b�N�X�o�b�t�@�쐬.
			CreateIndexBuffer( iCount*sizeof(int),
				pIndex, &pAppMesh->ppIndexBuffer[i] );
		}
		else{
			//������̏����ɕs����o������.
			//�J�E���g����0�ȉ��̏ꍇ�́A�C���f�b�N�X�o�b�t�@�� nullptr �ɂ��Ă���.
			pAppMesh->ppIndexBuffer[i] = nullptr;
		}

		//���̃}�e���A�����̃|���S����.
		pAppMesh->pMaterial[i].dwNumFace = iCount / 3;
		//�s�v�ɂȂ����̂ō폜.
		delete[] pIndex;
	}

	//�X�L����񂠂�H
	if( pContainer->pSkinInfo == nullptr ){
	/*	char strDbg[128];
		sprintf_s( strDbg, "ContainerName:[%s]", pContainer->Name );
		MessageBox( nullptr, strDbg, "Not SkinInfo", MB_OK );*/
		pAppMesh->bEnableBones = false;
	}
	else{
		//�X�L�����(�W���C���g�A�E�F�C�g)�ǂݍ���.
		ReadSkinInfo( pContainer, pvVB, pAppMesh );
	}

	//�o�[�e�b�N�X�o�b�t�@���쐬.
	D3D11_BUFFER_DESC bd;
	bd.Usage	= D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof( MY_SKINVERTEX ) * ( pAppMesh->dwNumVert );
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags= 0;
	bd.MiscFlags	= 0;
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = pvVB;

	HRESULT hRslt = S_OK;
	if( FAILED(
		m_pDevice11->CreateBuffer(
			&bd, &InitData, &pAppMesh->pVertexBuffer ) ) )
	{
		hRslt = E_FAIL;
	}

	//�p�[�c���b�V���ɐݒ�.
	pFrame->pPartsMesh = pAppMesh;
	m_pReleaseMaterial = pAppMesh;

	//�ꎞ�I�ȓ��ꕨ�͕s�v�Ȃ�̂ō폜.
	if( piFaceBuffer ){
		delete[] piFaceBuffer;
	}
	if( pvVB ){
		delete[] pvVB;
	}

	return hRslt;
}


//�{�[�������̃|�[�Y�ʒu�ɃZ�b�g����֐�.
void CDX9SkinMesh::SetNewPoseMatrices(
	SKIN_PARTS_MESH* pParts, int frame, MYMESHCONTAINER* pContainer )
{
	//�]�ރt���[����Update���邱��.
	//���Ȃ��ƍs�񂪍X�V����Ȃ�.
	//m_pD3dxMesh->UpdateFrameMatrices(
	// m_pD3dxMesh->m_pFrameRoot)�������_�����O���Ɏ��s���邱��.

	//�܂��A�A�j���[�V�������ԂɌ��������s����X�V����̂�D3DXMESH�ł�
	//�A�j���[�V�����R���g���[����(����)����Ă������̂Ȃ̂ŁA
	//�A�j���[�V�����R���g���[�����g���ăA�j����i�s�����邱�Ƃ��K�v.
	//m_pD3dxMesh->m_pAnimController->AdvanceTime(...)��.
	//�����_�����O���Ɏ��s���邱��.

	if( pParts->iNumBone <= 0 ){
		//�{�[���� 0�@�ȉ��̏ꍇ.
	}

	for( int i=0; i<pParts->iNumBone; i++ )
	{
		pParts->pBoneArray[i].mNewPose
			= m_pD3dxMesh->GetNewPose( pContainer, i );
	}
}


//����(���݂�)�|�[�Y�s���Ԃ��֐�.
D3DXMATRIX CDX9SkinMesh::GetCurrentPoseMatrix( SKIN_PARTS_MESH* pParts, int index )
{
	D3DXMATRIX ret =
		pParts->pBoneArray[index].mBindPose * pParts->pBoneArray[index].mNewPose;
	return ret;
}


//�t���[���̕`��.
VOID CDX9SkinMesh::DrawFrame( LPD3DXFRAME p, const D3DXMATRIX* pRot )
{
	MYFRAME*			pFrame	= (MYFRAME*)p;
	SKIN_PARTS_MESH*	pPartsMesh	= pFrame->pPartsMesh;
	MYMESHCONTAINER*	pContainer	= (MYMESHCONTAINER*)pFrame->pMeshContainer;

	if( pPartsMesh != nullptr )
	{
		DrawPartsMesh(
			pPartsMesh, 
			pFrame->CombinedTransformationMatrix,
			pContainer,
			pRot );
	}

	//�ċA�֐�.
	//(�Z��)
	if( pFrame->pFrameSibling != nullptr )
	{
		DrawFrame( pFrame->pFrameSibling, pRot );
	}
	//(�e�q)
	if( pFrame->pFrameFirstChild != nullptr )
	{
		DrawFrame( pFrame->pFrameFirstChild, pRot );
	}
}

//�p�[�c���b�V����`��.
void CDX9SkinMesh::DrawPartsMesh( SKIN_PARTS_MESH* pMesh, D3DXMATRIX World, MYMESHCONTAINER* pContainer, const D3DXMATRIX* pRot )
{
	//���[���h�s��.
	m_mWorld = m_Tranceform.GetWorldMatrix( pRot );

	std::function<void()> func = [&]()
	{
		m_ShadowRenderFunc(pMesh);
	};

	//���_�C���v�b�g���C�A�E�g���Z�b�g.
	m_pContext11->IASetInputLayout(	m_pVertexLayout );
	SetNewPoseMatrices( pMesh, m_iFrame, pContainer );

	// �e�������e�N�X�`���ɕ`��.
	if( m_pShadowMap->Render( true, m_mWorld, func ) == true ) return;

	D3D11_MAPPED_SUBRESOURCE pData;

	//�g�p����V�F�[�_�̃Z�b�g.
	m_pContext11->VSSetShader( m_pVertexShader, nullptr, 0 );

	//------------------------------------------------.
	//	�R���X�^���g�o�b�t�@�ɏ��𑗂�(�{�[��).
	//------------------------------------------------.
	if( SUCCEEDED(
		m_pContext11->Map(
			m_pCBufferPerBone, 0,
			D3D11_MAP_WRITE_DISCARD, 0, &pData ) ) )
	{
		CBUFFER_PER_BONES cb;
		for( int i=0; i<pMesh->iNumBone; i++ )
		{
			D3DXMATRIX mat = GetCurrentPoseMatrix( pMesh, i );
			D3DXMatrixTranspose( &mat, &mat );
			cb.mBone[i] = mat;
		}
		memcpy_s( pData.pData, pData.RowPitch, (void*)&cb, sizeof( cb ) );
		m_pContext11->Unmap(m_pCBufferPerBone, 0 );
	}
	m_pContext11->VSSetConstantBuffers(	3, 1, &m_pCBufferPerBone);
	m_pContext11->PSSetConstantBuffers(	3, 1, &m_pCBufferPerBone);
	
	//�o�[�e�b�N�X�o�b�t�@���Z�b�g.
	UINT stride = sizeof( MY_SKINVERTEX );
	UINT offset = 0;
	m_pContext11->IASetVertexBuffers(
		0, 1, &pMesh->pVertexBuffer, &stride, & offset );

	//�v���~�e�B�u�E�g�|���W�[���Z�b�g.
	m_pContext11->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

#if 0
	m_pContext11->PSSetShader( m_pPixelShader, nullptr, 0 );

	//------------------------------------------------.
	//	�R���X�^���g�o�b�t�@�ɏ���ݒ�(�t���[������).
	//------------------------------------------------.
	if( SUCCEEDED(
		m_pContext11->Map(
			m_pCBufferPerFrame, 0,
			D3D11_MAP_WRITE_DISCARD, 0, &pData ) ) )
	{
		CBUFFER_PER_FRAME cb;

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

		memcpy_s( pData.pData, pData.RowPitch, (void*)&cb, sizeof(cb) );
		m_pContext11->Unmap(m_pCBufferPerFrame, 0 );
	}
	m_pContext11->VSSetConstantBuffers(	2, 1, &m_pCBufferPerFrame);
	m_pContext11->PSSetConstantBuffers(	2, 1, &m_pCBufferPerFrame);
#endif 

	//------------------------------------------------.
	//	�R���X�^���g�o�b�t�@�ɏ���ݒ�(���b�V������).
	//------------------------------------------------.
	D3D11_MAPPED_SUBRESOURCE pDat;
	if (SUCCEEDED(
		m_pContext11->Map(
			m_pCBufferPerMesh, 0,
			D3D11_MAP_WRITE_DISCARD, 0, &pDat)))
	{
		CBUFFER_PER_MESH cb;
		// ���[���h�s���n��.
		cb.mW = m_mWorld;
		D3DXMatrixTranspose( &cb.mW, &cb.mW );

		// ���[���h�E�r���[�E�v���W�F�N�V�����s���n��.
		cb.mWVP = m_mWorld * m_mView * m_mProj;
		D3DXMatrixTranspose( &cb.mWVP, &cb.mWVP );

		// �F��n��.
		cb.vColor = m_Color;

		// �e�̔Z����n��.
		cb.vShadowDepth.x = 1.0f - m_ShadowDepth;

		memcpy_s(pDat.pData, pDat.RowPitch, (void*)&cb, sizeof(cb));
		m_pContext11->Unmap(m_pCBufferPerMesh, 0);
	}
	m_pContext11->VSSetConstantBuffers(0, 1, &m_pCBufferPerMesh);
	m_pContext11->PSSetConstantBuffers(0, 1, &m_pCBufferPerMesh);


	//�}�e���A���̐������A
	//���ꂼ��̃}�e���A���̃C���f�b�N�X�o�b�t�@��`��.
	for( DWORD i=0; i<pMesh->dwNumMaterial; i++ )
	{
		//�g�p����Ă��Ȃ��}�e���A���΍�.
		if( pMesh->pMaterial[i].dwNumFace == 0 )
		{
			continue;
		}
		//�C���f�b�N�X�o�b�t�@���Z�b�g.
		stride	= sizeof( int );
		offset	= 0;
		m_pContext11->IASetIndexBuffer(
			pMesh->ppIndexBuffer[i],
			DXGI_FORMAT_R32_UINT, 0 );

		//------------------------------------------------.
		//	�}�e���A���̊e�v�f�ƕϊ��s����V�F�[�_�ɓn��.
		//------------------------------------------------.
		D3D11_MAPPED_SUBRESOURCE pDat;
		if( SUCCEEDED(
			m_pContext11->Map(
				m_pCBufferPerMaterial, 0,
				D3D11_MAP_WRITE_DISCARD, 0, &pDat )))
		{
			CBUFFER_PER_MATERIAL cb;
			cb.vAmbient	= pMesh->pMaterial[i].Ka;
			cb.vDiffuse	= pMesh->pMaterial[i].Kd;
			cb.vSpecular= pMesh->pMaterial[i].Ks;
			memcpy_s( pDat.pData, pDat.RowPitch, (void*)&cb, sizeof(cb));
			m_pContext11->Unmap( m_pCBufferPerMaterial, 0 );
		}
		m_pContext11->VSSetConstantBuffers(	1, 1, &m_pCBufferPerMaterial);
		m_pContext11->PSSetConstantBuffers(	1, 1, &m_pCBufferPerMaterial);

		m_pContext11->PSSetSamplers( 0, 1, &m_pSampleLinear );
		m_pContext11->PSSetShaderResources( 0, 1, &pMesh->pMaterial[i].pTexture );
		m_pContext11->PSSetShaderResources( 1, 1, &pMesh->pMaterial[i].pNormTexture );

		//Draw.
		m_pContext11->DrawIndexed( pMesh->pMaterial[i].dwNumFace * 3, 0, 0 );
	}
	ID3D11ShaderResourceView* resetSrv = nullptr;
	m_pContext11->PSSetShaderResources( 0, 1, &resetSrv );
	m_pContext11->PSSetShaderResources( 1, 1, &resetSrv );
}



//����֐�.
HRESULT CDX9SkinMesh::Release()
{
	if( m_pD3dxMesh != nullptr ){
		//�S�Ẵ��b�V���폜.
		DestroyAllMesh( m_pD3dxMesh->m_pFrameRoot );

		//�p�[�T�[�N���X�������.
		m_pD3dxMesh->Release();
		SAFE_DELETE( m_pD3dxMesh );
	}

	return S_OK;
}


//�S�Ẵ��b�V�����폜.
void CDX9SkinMesh::DestroyAllMesh( D3DXFRAME* pFrame )
{
	if ((pFrame != nullptr) && (pFrame->pMeshContainer != nullptr))
	{
		DestroyAppMeshFromD3DXMesh( pFrame );
	}

	//�ċA�֐�.
	if( pFrame->pFrameSibling != nullptr )
	{
		DestroyAllMesh( pFrame->pFrameSibling );
	}
	if( pFrame->pFrameFirstChild != nullptr )
	{
		DestroyAllMesh( pFrame->pFrameFirstChild );
	}
}


//���b�V���̍폜.
HRESULT CDX9SkinMesh::DestroyAppMeshFromD3DXMesh( LPD3DXFRAME p )
{
	MYFRAME* pFrame = (MYFRAME*)p;

	MYMESHCONTAINER* pMeshContainerTmp = (MYMESHCONTAINER*)pFrame->pMeshContainer;

	//MYMESHCONTAINER�̒��g�̉��.
	if (pMeshContainerTmp != nullptr)
	{
		if (pMeshContainerTmp->pBoneBuffer != nullptr)
		{
			pMeshContainerTmp->pBoneBuffer->Release();
			pMeshContainerTmp->pBoneBuffer = nullptr;
		}

		if (pMeshContainerTmp->pBoneOffsetMatrices != nullptr)
		{
			delete pMeshContainerTmp->pBoneOffsetMatrices;
			pMeshContainerTmp->pBoneOffsetMatrices = nullptr;
		}
		
		if (pMeshContainerTmp->ppBoneMatrix != nullptr)
		{
			int iMax = static_cast<int>(pMeshContainerTmp->pSkinInfo->GetNumBones());

			for (int i = iMax - 1; i >= 0; i--)
			{
				if (pMeshContainerTmp->ppBoneMatrix[i] != nullptr)
				{
					pMeshContainerTmp->ppBoneMatrix[i] = nullptr;
				}
			}

			delete[] pMeshContainerTmp->ppBoneMatrix;
			pMeshContainerTmp->ppBoneMatrix = nullptr;
		}

		if (pMeshContainerTmp->ppTextures != nullptr)
		{
			int iMax = static_cast<int>(pMeshContainerTmp->NumMaterials);

			for (int i = iMax - 1; i >= 0; i--)
			{
				if (pMeshContainerTmp->ppTextures[i] != nullptr)
				{
					pMeshContainerTmp->ppTextures[i]->Release();
					pMeshContainerTmp->ppTextures[i] = nullptr;
				}
			}

			delete[] pMeshContainerTmp->ppTextures;
			pMeshContainerTmp->ppTextures = nullptr;
		}
	}

	pMeshContainerTmp = nullptr;

	//MYMESHCONTAINER�������.

	//LPD3DXMESHCONTAINER�̉��.
	if (pFrame->pMeshContainer->pAdjacency != nullptr)
	{
		delete[] pFrame->pMeshContainer->pAdjacency;
		pFrame->pMeshContainer->pAdjacency = nullptr;
	}

	if (pFrame->pMeshContainer->pEffects != nullptr)
	{
		if (pFrame->pMeshContainer->pEffects->pDefaults != nullptr)
		{
			if (pFrame->pMeshContainer->pEffects->pDefaults->pParamName != nullptr)
			{
				delete pFrame->pMeshContainer->pEffects->pDefaults->pParamName;
				pFrame->pMeshContainer->pEffects->pDefaults->pParamName = nullptr;
			}

			if (pFrame->pMeshContainer->pEffects->pDefaults->pValue != nullptr)
			{
				delete pFrame->pMeshContainer->pEffects->pDefaults->pValue;
				pFrame->pMeshContainer->pEffects->pDefaults->pValue = nullptr;
			}

			delete pFrame->pMeshContainer->pEffects->pDefaults;
			pFrame->pMeshContainer->pEffects->pDefaults = nullptr;
		}

		if (pFrame->pMeshContainer->pEffects->pEffectFilename != nullptr)
		{
			delete pFrame->pMeshContainer->pEffects->pEffectFilename;
			pFrame->pMeshContainer->pEffects->pEffectFilename = nullptr;
		}

		delete pFrame->pMeshContainer->pEffects;
		pFrame->pMeshContainer->pEffects = nullptr;
	}

	if (pFrame->pMeshContainer->pMaterials != nullptr)
	{
		int iMax = static_cast<int>(pFrame->pMeshContainer->NumMaterials);

		for (int i = iMax - 1; i >= 0; i--)
		{
			delete[] pFrame->pMeshContainer->pMaterials[i].pTextureFilename;
			pFrame->pMeshContainer->pMaterials[i].pTextureFilename = nullptr;
		}

		delete[] pFrame->pMeshContainer->pMaterials;
		pFrame->pMeshContainer->pMaterials = nullptr;
	}

	if (pFrame->pMeshContainer->pNextMeshContainer != nullptr)
	{
		//���̃��b�V���R���e�i�[�̃|�C���^�[�����̂��Ƃ�����.
		//new�ō���邱�Ƃ͂Ȃ��͂��Ȃ̂ŁA����ōs����Ǝv��.
		pFrame->pMeshContainer->pNextMeshContainer = nullptr;
	}

	if (pFrame->pMeshContainer->pSkinInfo != nullptr)
	{
		pFrame->pMeshContainer->pSkinInfo->Release();
		pFrame->pMeshContainer->pSkinInfo = nullptr;
	}

	//LPD3DXMESHCONTAINER�̉������.

	//MYFRAME�̉��.

	if (pFrame->pPartsMesh != nullptr)
	{
		//�{�[�����̉��.
		if (pFrame->pPartsMesh->pBoneArray)
		{
			delete[] pFrame->pPartsMesh->pBoneArray;
			pFrame->pPartsMesh->pBoneArray = nullptr;
		}

		if (pFrame->pPartsMesh->pMaterial != nullptr)
		{
			int iMax = static_cast<int>(pFrame->pPartsMesh->dwNumMaterial);

			for (int i = iMax - 1; i >= 0; i--)
			{
				if (pFrame->pPartsMesh->pMaterial[i].pTexture)
				{
					pFrame->pPartsMesh->pMaterial[i].pTexture->Release();
					pFrame->pPartsMesh->pMaterial[i].pTexture = nullptr;
				}
			}

			delete[] pFrame->pPartsMesh->pMaterial;
			pFrame->pPartsMesh->pMaterial = nullptr;
		}
		

		if (pFrame->pPartsMesh->ppIndexBuffer)
		{
			//�C���f�b�N�X�o�b�t�@���.
			for (DWORD i = 0; i<pFrame->pPartsMesh->dwNumMaterial; i++){
				if (pFrame->pPartsMesh->ppIndexBuffer[i] != nullptr){
					pFrame->pPartsMesh->ppIndexBuffer[i]->Release();
					pFrame->pPartsMesh->ppIndexBuffer[i] = nullptr;
				}
			}
			delete[] pFrame->pPartsMesh->ppIndexBuffer;
		}

		//���_�o�b�t�@�J��.
		pFrame->pPartsMesh->pVertexBuffer->Release();
		pFrame->pPartsMesh->pVertexBuffer = nullptr;
	}

	//�p�[�c�}�e���A���J��.
	delete[] pFrame->pPartsMesh;
	pFrame->pPartsMesh = nullptr;

	//SKIN_PARTS_MESH�������.

	//MYFRAME��SKIN_PARTS_MESH�ȊO�̃����o�[�|�C���^�[�ϐ��͕ʂ̊֐��ŉ������Ă��܂���.

	return S_OK;
}



//�A�j���[�V�����Z�b�g�̐؂�ւ�.
void CDX9SkinMesh::ChangeAnimSet( int index, SAnimationController* pAC )
{
	if( m_pD3dxMesh == nullptr )	return;
	m_pD3dxMesh->ChangeAnimSet( index, pAC == nullptr ? nullptr : pAC->pAC );
	if( pAC != nullptr ){
		pAC->IsAnimChangeBlend = false;
		pAC->AnimTime = 0.0;
	}
	m_IsChangeAnim = false;
	m_dAnimTime = 0.0;
}


//�A�j���[�V�����Z�b�g�̐؂�ւ�(�J�n�t���[���w��\��).
void CDX9SkinMesh::ChangeAnimSet_StartPos( int index, double dStartFramePos, SAnimationController* pAC )
{
	if( m_pD3dxMesh == nullptr )	return;
	m_pD3dxMesh->ChangeAnimSet_StartPos( index, dStartFramePos, pAC == nullptr ? nullptr : pAC->pAC );
}

// �A�j���[�V�������u�����h���Đ؂�ւ�.
void CDX9SkinMesh::ChangeAnimBlend( int index, int oldIndex, SAnimationController* pAC )
{
	if( m_pD3dxMesh == nullptr )	return;
	m_pD3dxMesh->ChangeAnimBlend( index, oldIndex, pAC == nullptr ? nullptr : pAC->pAC );
	if( pAC != nullptr ){
		pAC->IsAnimChangeBlend = true;
		pAC->AnimTime = 0.0;
	}
	m_IsChangeAnim = true;
	m_dAnimTime = 0.0;
}

// �A�j���[�V�����̍X�V.
void CDX9SkinMesh::AnimUpdate( SAnimationController* pAC )
{
	// �e���̕`�掞�����A�j���[�V�������X�V����.
	if( m_pShadowMap->IsEndRender() == true ) return;

	if( pAC == nullptr )
	{
		if (m_pD3dxMesh->m_pAnimController)
		{
			BlendAnimUpdate();
			m_pD3dxMesh->m_pAnimController->AdvanceTime(m_dAnimSpeed, NULL);
		}
	} else {
		pAC->BlendAnimUpdate( m_dAnimSpeed );
		pAC->pAC->AdvanceTime(m_dAnimSpeed, NULL);
	}
}

// �u�����h�A�j���[�V�����̍X�V.
void CDX9SkinMesh::BlendAnimUpdate()
{
	// �A�j���[�V�����؂�ւ��t���O������Ă���I��.
	if( m_IsChangeAnim == false ) return;

	m_dAnimTime += m_dAnimSpeed;
	float weight = static_cast<float>(m_dAnimTime) / 1.0f;	// �E�F�C�g�̌v�Z.
	m_pD3dxMesh->m_pAnimController->SetTrackWeight( 0, weight );	// �g���b�N0 �ɃE�F�C�g��ݒ�.
	m_pD3dxMesh->m_pAnimController->SetTrackWeight( 1, 1 - weight );// �g���b�N1 �ɃE�F�C�g��ݒ�.

	if( m_dAnimTime < 1.0 ) return;
	// �A�j���[�V�����^�C�������l�ɒB������.
	m_IsChangeAnim = false;	// �t���O������.
	m_pD3dxMesh->m_pAnimController->SetTrackWeight( 0, 1.0f );		// �E�F�C�g��1�ɌŒ肷��.
	m_pD3dxMesh->m_pAnimController->SetTrackEnable( 1, false );		// �g���b�N1�𖳌��ɂ���.
}

//�A�j���[�V������~���Ԃ��擾.
double CDX9SkinMesh::GetAnimPeriod( int index )
{
	if( m_pD3dxMesh == nullptr ){
		return 0.0f;
	}
	return m_pD3dxMesh->GetAnimPeriod( index );
}


//�A�j���[�V���������擾.
int CDX9SkinMesh::GetAnimMax( LPD3DXANIMATIONCONTROLLER pAC )
{
	if( m_pD3dxMesh != nullptr ){
		return m_pD3dxMesh->GetAnimMax( pAC );
	}
	return -1;
}


//�w�肵���{�[�����(�s��)���擾����֐�.
bool CDX9SkinMesh::GetMatrixFromBone(const char* sBoneName, D3DXMATRIX* pOutMat )
{
	if( m_pD3dxMesh != nullptr ){
		if( m_pD3dxMesh->GetMatrixFromBone( sBoneName, pOutMat ) ){
			return true;
		}
	}
	return false;
}
//�w�肵���{�[�����(���W)���擾����֐�.
bool CDX9SkinMesh::GetPosFromBone(const char* sBoneName, D3DXVECTOR3* pOutPos)
{
	if( m_pD3dxMesh != nullptr ){
		D3DXVECTOR3 tmpPos;
		if( m_pD3dxMesh->GetPosFromBone( sBoneName, &tmpPos ) ){
			D3DXMATRIX mWorld, mScale, mTran;
			D3DXMATRIX mRot, mYaw, mPitch, mRoll;
			D3DXMatrixScaling( &mScale, m_Tranceform.Scale.x, m_Tranceform.Scale.y, m_Tranceform.Scale.z );
			D3DXMatrixRotationY( &mYaw, m_Tranceform.Rotation.y);
			D3DXMatrixRotationX( &mPitch, m_Tranceform.Rotation.x);
			D3DXMatrixRotationZ( &mRoll, m_Tranceform.Rotation.z);
			D3DXMatrixTranslation(&mTran, tmpPos.x, tmpPos.y, tmpPos.z);

			mRot = mYaw * mPitch * mRoll;
			mWorld = mTran * mScale * mRot;

			pOutPos->x = mWorld._41 + m_Tranceform.Position.x;
			pOutPos->y = mWorld._42 + m_Tranceform.Position.y;
			pOutPos->z = mWorld._43 + m_Tranceform.Position.z;

			return true;
		}
	}
	return false;
}

//�w�肵���{�[�����(�s��)��ݒ肷��֐�.
bool CDX9SkinMesh::SetMatrixFromBone(const char* sBoneName, D3DXMATRIX inMat )
{
	if( m_pD3dxMesh != nullptr ){
		if( m_pD3dxMesh->SetMatrixFromBone( sBoneName, inMat ) ){
			return true;
		}
	}
	return false;
}
//�w�肵���{�[�����(���W)��ݒ肷��֐�.
bool CDX9SkinMesh::SetPosFromBone(const char* sBoneName, D3DXVECTOR3 inPos)
{
	if( m_pD3dxMesh != nullptr ){
		if( m_pD3dxMesh->SetPosFromBone( sBoneName, inPos ) ){
			return true;
		}
	}
	return false;
}

bool CDX9SkinMesh::GetDeviaPosFromBone(const char* sBoneName, D3DXVECTOR3* pOutPos, D3DXVECTOR3 vSpecifiedPos)
{
	if (m_pD3dxMesh != nullptr){
		D3DXMATRIX mtmp;
		if (m_pD3dxMesh->GetMatrixFromBone(sBoneName, &mtmp)){
			D3DXMATRIX mWorld, mScale, mTran, mDevia;
			D3DXMATRIX mRot, mYaw, mPitch, mRoll;
			//�����炭�{�[���̏����̌��������ʒu�ɂȂ��Ă���͂��ł�.
			D3DXMatrixTranslation(&mDevia, vSpecifiedPos.x, vSpecifiedPos.y, vSpecifiedPos.z);//���炵���������̍s����쐬.
			D3DXMatrixMultiply(&mtmp, &mDevia, &mtmp);//�{�[���ʒu�s��Ƃ��炵���������s����|�����킹��.
			D3DXVECTOR3 tmpPos = D3DXVECTOR3(mtmp._41, mtmp._42, mtmp._43);//�ʒu�̂ݎ擾.

			D3DXMatrixScaling(&mScale, m_Tranceform.Scale.x, m_Tranceform.Scale.y, m_Tranceform.Scale.z );
			D3DXMatrixRotationY(&mYaw, m_Tranceform.Rotation.y);
			D3DXMatrixRotationX(&mPitch, m_Tranceform.Rotation.x);
			D3DXMatrixRotationZ(&mRoll, m_Tranceform.Rotation.z);
			D3DXMatrixTranslation(&mTran, tmpPos.x, tmpPos.y, tmpPos.z);

			mRot = mYaw * mPitch * mRoll;
			mWorld = mTran * mScale * mRot;

			pOutPos->x = mWorld._41 + m_Tranceform.Position.x;
			pOutPos->y = mWorld._42 + m_Tranceform.Position.y;
			pOutPos->z = mWorld._43 + m_Tranceform.Position.z;

			return true;
		}
	}
	return false;
}

//�R���X�^���g�o�b�t�@�쐬�֐�.
HRESULT CDX9SkinMesh::CreateCBuffer(
	ID3D11Buffer** pConstantBuffer,
	UINT size)
{
	D3D11_BUFFER_DESC cb;

	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = size;
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;
	if (FAILED(
		m_pDevice11->CreateBuffer(&cb, NULL, pConstantBuffer)))
	{
		return E_FAIL;
	}
	return S_OK;
}

//�T���v���[�쐬�֐�.
HRESULT CDX9SkinMesh::CreateLinearSampler(ID3D11SamplerState** pSampler)
{
	//�e�N�X�`���[�p�T���v���[�쐬.
	D3D11_SAMPLER_DESC SamDesc;
	ZeroMemory(&SamDesc, sizeof(D3D11_SAMPLER_DESC));

	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	if (FAILED(
		m_pDevice11->CreateSamplerState(&SamDesc, &m_pSampleLinear)))
	{
		return E_FAIL;
	}
	return S_OK;
}