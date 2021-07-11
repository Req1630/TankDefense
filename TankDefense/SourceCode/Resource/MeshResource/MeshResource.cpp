#include "MeshResource.h"
#include "..\..\Global.h"
#include "..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"
#include "..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\Common\DebugText\DebugText.h"

CMeshResorce::CMeshResorce()
	: m_StaticMeshList		()
	, m_SkinMeshList		()
	, m_HasFinishedLoading	( false )
{
}

CMeshResorce::~CMeshResorce()
{
	m_SkinMeshList.clear();
	m_StaticMeshList.clear();
}

//-------------------------------------.
// �C���X�^���X�̎擾.
//-------------------------------------.
CMeshResorce* CMeshResorce::GetInstance()
{
	static std::unique_ptr<CMeshResorce> pInstance =
		std::make_unique<CMeshResorce>();
	return pInstance.get();
}

//-------------------------------------.
// ���f���̓ǂݍ���.
//-------------------------------------.
HRESULT CMeshResorce::Load( HWND hWnd, ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11, LPDIRECT3DDEVICE9 pDevice9 )
{
	if( FAILED( GetInstance()->ModelLoad( hWnd, pDevice11, pContext11, pDevice9 ) )) return E_FAIL;
	return S_OK;
}

//-------------------------------------.
// �X�^�e�B�b�N���b�V���̓ǂݍ���.
//-------------------------------------.
CDX9StaticMesh* CMeshResorce::GetStatic( const std::string& name )
{
	// �w�肵�����f����Ԃ�.
	for( auto& m : GetInstance()->m_StaticMeshList ){
		if( m.first == name ) return m.second.get();
	}

	// ���f���̓Ǎ����I�����ĂȂ��̂� null ��Ԃ�.
	if( GetInstance()->m_HasFinishedLoading == false ) return nullptr;

	ERROR_MESSAGE( name + " mesh not found" );
	return nullptr;
}

//-------------------------------------.
// �X�L�����b�V���̓ǂݍ���.
//-------------------------------------.
CDX9SkinMesh* CMeshResorce::GetSkin( const std::string& name )
{
	// �w�肵�����f����Ԃ�.
	for( auto& m : GetInstance()->m_SkinMeshList ){
		if( m.first == name ) return m.second.get();
	}

	// ���f���̓Ǎ����I�����ĂȂ��̂� null ��Ԃ�.
	if( GetInstance()->m_HasFinishedLoading == false ) return nullptr;

	ERROR_MESSAGE( name + " mesh not found" );
	return nullptr;
}

//-------------------------------------.
// ���f���̓ǂݍ���.
//-------------------------------------.
HRESULT CMeshResorce::ModelLoad( HWND hWnd, ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11, LPDIRECT3DDEVICE9 pDevice9 )
{
	std::unique_lock<std::mutex> lock( m_Mutex );
	auto eachLoad = [&]( const fs::directory_entry& entry )
	{
		const std::string exe		= entry.path().extension().string();	// �g���q.
		const std::string filePath	= entry.path().string();				// �t�@�C���p�X.
		const std::string fileName	= entry.path().stem().string();			// �t�@�C����.
		const std::string errorMsg	= fileName+" : X �t�@�C���ǂݍ��ݎ��s";	// ���炩���߃G���[���b�Z�[�W��ݒ肷��.

		if( exe != ".x" && exe != ".X" ) return;	// �g���q�� "X�t�@�C��" ����Ȃ���ΏI��.
		
		// "_s" �̓X�L�����b�V��.
		if( fileName.find("_s") == std::string::npos ){
			// "_s" ��������Ȃ��̂� �X�^�e�B�b�N���b�V��.
			m_StaticMeshList[fileName] = std::make_unique<CDX9StaticMesh>();
			if( FAILED( m_StaticMeshList[fileName]->Init( hWnd, pDevice11, pContext11, pDevice9, filePath.c_str() )))
				throw errorMsg.c_str();
		} else {
			// �X�L�����b�V��.
			m_SkinMeshList[fileName] = std::make_unique<CDX9SkinMesh>();
			if( FAILED( m_SkinMeshList[fileName]->Init( hWnd, pDevice11, pContext11, pDevice9, filePath.c_str() )))
				throw errorMsg.c_str();
		}
	};

	CLog::Print( "------ ���b�V���ǂݍ��݊J�n -------" );
	CDebugText::PushLog( "Mesh Load Begin" );

	try {
		fs::recursive_directory_iterator dir_itr( FILE_PATH ), end_itr;
		std::for_each( dir_itr, end_itr, eachLoad );
	} catch( const fs::filesystem_error& e ){
		// �G���[���b�Z�[�W��\��.
		ERROR_MESSAGE( e.path1().string().c_str() );
		return E_FAIL;
	} catch( const char* e ){
		// �G���[���b�Z�[�W��\��.
		ERROR_MESSAGE( e );
		return E_FAIL;
	}

	CDebugText::PushLog( "Mesh Load End" );
	CLog::Print( "------ ���b�V���ǂݍ��ݏI�� -------" );

	// �Ǎ����I������̂� true �ɂ���.
	m_HasFinishedLoading = true;
	return S_OK;
}