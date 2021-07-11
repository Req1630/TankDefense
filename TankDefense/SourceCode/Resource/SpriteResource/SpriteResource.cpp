#include "SpriteResource.h"
#include "..\..\Utility\FileManager\FileManager.h"
#include "..\..\Utility\DebugConsole\DebugConsole.h"
#include "..\..\Common\DebugText\DebugText.h"

CSpriteResource::CSpriteResource()
	: m_SpriteList			()
	, m_HasFinishedLoading	( false )
{
}

CSpriteResource::~CSpriteResource()
{
	m_SpriteList.clear();
}

//-------------------------------.
// �C���X�^���X�̎擾.
//-------------------------------.
CSpriteResource* CSpriteResource::GetInstance()
{
	static std::unique_ptr<CSpriteResource> pInstance = 
		std::make_unique<CSpriteResource>();
	return pInstance.get();
}

//-------------------------------.
// �X�v���C�g�̓ǂݍ���(���b�p�[).
//-------------------------------.
HRESULT CSpriteResource::Load( ID3D11DeviceContext* pContext11 )
{
	if( FAILED( GetInstance()->SpriteLoad( pContext11 ) )) return E_FAIL;
	return S_OK;
}

//-------------------------------.
// �X�v���C�g�擾�֐�.
//-------------------------------.
CSprite* CSpriteResource::GetSprite( const std::string& spriteName )
{
	// �w�肵���X�v���C�g�����擾����.
	for( auto& s : GetInstance()->m_SpriteList ){
		if( s.first == spriteName ) return s.second.get();
	}

	// �X�v���C�g�̓Ǎ����I�����ĂȂ��̂� null ��Ԃ�.
	if( GetInstance()->m_HasFinishedLoading == false ) return nullptr;

	ERROR_MESSAGE( spriteName + " �����݂��܂���" );
	return nullptr;
}

//-------------------------------.
// �X�v���C�g�����X�g�擾�֐�.
//-------------------------------.
std::vector<std::string> CSpriteResource::GetSpriteNameList()
{
	std::vector<std::string> nameList;
	// �w�肵���X�v���C�g�����擾����.
	for( auto& s : GetInstance()->m_SpriteList ){
		nameList.emplace_back( s.first );
	}
	return nameList;
}

//-------------------------------.
// �X�v���C�g�ǂݍ���.
//-------------------------------.
HRESULT CSpriteResource::SpriteLoad( ID3D11DeviceContext* pContext11 )
{
	std::unique_lock<std::mutex> lock( m_Mutex );
	auto eachLoad = [&]( const fs::directory_entry& entry )
	{
		const std::string exe		= entry.path().extension().string();	// �g���q.
		const std::string filePath	= entry.path().string();				// �t�@�C���p�X.
		const std::string fileName	= entry.path().stem().string();			// �t�@�C����.
		const std::string errorMsg	= fileName+" : �X�v���C�g�ǂݍ��ݎ��s";	// ���炩���߃G���[���b�Z�[�W��ݒ肷��.

		// "_"�����������͖̂��������.
		if( filePath.find("_") != std::string::npos ) return;
		// �w�肵���g���q�ȊO�͓ǂݍ���.
		if( exe != ".png" && exe != ".PNG" && 
			exe != ".bmp" && exe != ".BMP" &&
			exe != ".jpg" && exe != ".JPG" ) return;

		// �X�v���C�g�̍쐬.
		m_SpriteList[fileName] = std::make_unique<CSprite>();
		if( FAILED( m_SpriteList[fileName]->Init( pContext11, filePath.c_str() ) ))
			throw errorMsg;
	};

	CLog::Print( "------ �摜�ǂݍ��݊J�n -------" );
	CDebugText::PushLog( "Sprite Load Begin" );

	try {
		fs::recursive_directory_iterator dir_itr(FILE_PATH), end_itr;
		std::for_each( dir_itr, end_itr, eachLoad );
	} catch ( const fs::filesystem_error& e ){
		// �G���[���b�Z�[�W��\��.
		ERROR_MESSAGE( e.path1().string().c_str() );
		return E_FAIL;
	} catch( const std::string& e ){
		// �G���[���b�Z�[�W��\��.
		ERROR_MESSAGE( e );
		return E_FAIL;
	}

	CDebugText::PushLog( "Sprite Load End" );
	CLog::Print( "------ �摜�ǂݍ��ݏI�� -------" );

	// �Ǎ����I������̂� true �ɂ���.
	m_HasFinishedLoading = true;
	return S_OK;
}