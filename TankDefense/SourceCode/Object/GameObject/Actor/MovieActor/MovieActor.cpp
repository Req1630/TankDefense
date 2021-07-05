#include "MovieActor.h"
#include "..\..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"

template class CMovieActor<CEnemy>;

template<typename T>
CMovieActor<T>::CMovieActor()
	: m_NowTime	( 0.0f )
	, m_IsAcive	( false )
{}

template<typename T>
CMovieActor<T>::~CMovieActor()
{
}

//-----------------------------.
// 初期化関数.
//-----------------------------.
template<typename T>
bool CMovieActor<T>::Init()
{
	if( T::Init() == false ) return false;
	return true;
}

//-----------------------------.
// 更新関数.
//-----------------------------.
template<typename T>
void CMovieActor<T>::Update( const float& deltaTime )
{
	m_NowTime += deltaTime;
	ConfirmAcive();

	if( m_IsAcive == false ) return;
	T::Update( deltaTime );
}

//-----------------------------.
// 描画関数.
//-----------------------------.
template<typename T>
void CMovieActor<T>::Render()
{
	if( m_IsAcive == false ) return;
	T::Render();
}

//-----------------------------.
// モデル表示.
//-----------------------------.
template<typename T>
void CMovieActor<T>::ModelRender( const bool& isWire )
{
	T::m_pSkinMesh->SetRasterizerState( isWire ? ERS_STATE::Wire : ERS_STATE::Back );
	T::m_pSkinMesh->Render();
	T::m_pSkinMesh->SetRasterizerState( ERS_STATE::None );
}

//-----------------------------.
// 動作できるか確認する.
//-----------------------------.
template<typename T>
void CMovieActor<T>::ConfirmAcive()
{
	if( m_IsAcive == true ) return;
	if( m_NowTime < m_MovieState.AciveStartTime ) return;

	m_IsAcive = true;
}