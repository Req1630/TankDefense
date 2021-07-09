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
// 初期化.
//------------------------------.
bool CMovieManager::Init()
{
	return true;
}

//------------------------------.
// 更新.
//------------------------------.
void CMovieManager::Update()
{
	if( m_pActiveMovie == nullptr ) return;
	m_pActiveMovie->Update();
}

//------------------------------.
// モデルの描画.
//------------------------------.
void CMovieManager::ModelRender()
{
	if( m_pActiveMovie == nullptr ) return;
}

//------------------------------.
// 画像の描画.
//------------------------------.
void CMovieManager::WidgetRender()
{
	if( m_pActiveMovie == nullptr ) return;
	
}

//------------------------------.
// ムービー(演出)の切り替え.
//------------------------------.
void CMovieManager::ChangeMovie( const EMovieNo& key )
{
	m_pActiveMovie = m_pMovieList.at(key).get();
}

//------------------------------.
// ムービー(演出)の再生.
//------------------------------.
void CMovieManager::Play()
{
	if( m_pActiveMovie == nullptr ) return;
	m_pActiveMovie->Play();
}