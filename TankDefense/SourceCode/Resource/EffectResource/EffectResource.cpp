#include "EffectResource.h"
#include "..\..\Common\Effect\Effect.h"
#include "..\..\Common\DebugText\DebugText.h"

CEffectResource::CEffectResource()
	: m_EffectList			()
	, m_HasFinishedLoading	( false )
{
}

CEffectResource::~CEffectResource()
{
	Release();
}

//----------------------------.
// �C���X�^���X�̎擾.
//----------------------------.
CEffectResource* CEffectResource::GetInstance()
{
	static std::unique_ptr<CEffectResource> pInstance =
		std::make_unique<CEffectResource>();
	return pInstance.get();
}

//----------------------------.
// �ǂݍ���(���b�p�[).
//----------------------------.
void CEffectResource::Load( ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11 )
{
	// �G�t�F�N�g�̓ǂݍ���.
	GetInstance()->EffectLoad( pDevice11, pContext11 );
}

//----------------------------.
// ���.
//----------------------------.
void CEffectResource::Release()
{
	GetInstance()->m_EffectList.clear();
}

//----------------------------.
// �G�t�F�N�g�擾�֐�.
//----------------------------.
CEffect* CEffectResource::GetEffect( const std::string& fileName )
{
	// �������G�t�F�N�g���擾.
	for( auto& m : GetInstance()->m_EffectList ){
		if( m.first == fileName ) return m.second.get();
	}

	// �Ǎ����I����Ă��Ȃ������� null ��Ԃ�.
	if( GetInstance()->m_HasFinishedLoading == false ) return nullptr;

	ERROR_MESSAGE( fileName + " effects not found" );
	return nullptr;
}

//----------------------------.
// �G�t�F�N�g�̓ǂݍ���.
//----------------------------.
void CEffectResource::EffectLoad(
	ID3D11Device* pDevice11,
	ID3D11DeviceContext* pContext11 )
{
	std::unique_lock<std::mutex> lock( m_Mutex );
	auto eachLoad = [&]( const fs::directory_entry& entry )
	{
		const std::string exe		= entry.path().extension().string();	// �g���q.
		const std::string filePath	= entry.path().string();				// �t�@�C���p�X.
		const std::string fileName	= entry.path().stem().string();			// �t�@�C����.

		if( exe != ".efk" ) return;	// �g���q�� ".efk" ����Ȃ���ΏI��.

		// �G�t�F�N�g��ǂݍ��݃��X�g�Ɋi�[����.
		m_EffectList[fileName] = 
			std::make_unique<CEffect>( pDevice11, pContext11, filePath );
	};

	CLog::Print( "------ �G�t�F�N�g�ǂݍ��݊J�n -------" );
	CDebugText::PushLog( "Effect Load Begin " );

	try {
		fs::recursive_directory_iterator dir_itr(FILE_PATH), end_itr;
		std::for_each( dir_itr, end_itr, eachLoad );

	} catch( const fs::filesystem_error& e ){
		// �G���[���b�Z�[�W��\��.
		ERROR_MESSAGE( e.path1().string().c_str() );
	}

	CDebugText::PushLog( "Effect Load End " );
	CLog::Print( "------ �G�t�F�N�g�ǂݍ��ݏI�� -------" );

	// �Ǎ����I������̂� true �ɂ���.
	m_HasFinishedLoading = true;
}