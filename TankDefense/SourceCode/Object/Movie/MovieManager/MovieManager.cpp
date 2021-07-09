#include "MovieManager.h"
#include "..\MovieDataLoader\MovieDataLoader.h"
#include "..\Movie.h"

CMovieManager::CMovieManager()
	: m_pMovieDataLoader	( std::make_unique<CMovieDataLoader>() )
	, m_pMovieList			()
	, m_pActiveMovie		( nullptr )
{
}

CMovieManager::~CMovieManager()
{
}

//------------------------------.
// ������.
//------------------------------.
bool CMovieManager::Init()
{
	return true;
}

//------------------------------.
// �X�V.
//------------------------------.
void CMovieManager::Update()
{
	if( m_pActiveMovie == nullptr ) return;
	m_pActiveMovie->Update();
}

//------------------------------.
// ���f���̕`��.
//------------------------------.
void CMovieManager::ModelRender()
{
	if( m_pActiveMovie == nullptr ) return;
}

//------------------------------.
// �摜�̕`��.
//------------------------------.
void CMovieManager::WidgetRender()
{
	if( m_pActiveMovie == nullptr ) return;
	
}

//------------------------------.
// ���[�r�[(���o)�̐؂�ւ�.
//------------------------------.
void CMovieManager::ChangeMovie( const EMovieNo& key )
{
	m_pActiveMovie = m_pMovieList.at(key).get();
}

//------------------------------.
// ���[�r�[(���o)�̍Đ�.
//------------------------------.
void CMovieManager::Play()
{
	if( m_pActiveMovie == nullptr ) return;
	m_pActiveMovie->Play();
}