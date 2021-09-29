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
// �������֐�.
//-----------------------------.
template<typename T>
bool CMovieActor<T>::Init()
{
	if( T::Init() == false ) return false;
	T::m_Tranceform.Position = m_MovieState.StartPosition;
	T::m_Tranceform.Rotation = m_MovieState.StartRotation;

	m_IsAcive = false;
	m_NowTime = 0.0f;

	return true;
}

//-----------------------------.
// �X�V�֐�.
//-----------------------------.
template<typename T>
void CMovieActor<T>::Update( const float& deltaTime )
{
	m_NowTime += deltaTime;
	ConfirmAcive();

	if( m_IsAcive == false ) return;
	T::MovieUpdate( deltaTime );
}

//-----------------------------.
// �`��֐�.
//-----------------------------.
template<typename T>
void CMovieActor<T>::Render()
{
	if( m_IsAcive == false ) return;
	T::Render();
}

//-----------------------------.
// ���f���\��.
//-----------------------------.
template<typename T>
void CMovieActor<T>::ModelRender( const bool& isWire )
{
	T::m_pSkinMesh->SetRasterizerState( isWire ? ERS_STATE::Wire : ERS_STATE::Back );
	T::m_pSkinMesh->Render();
	T::m_pSkinMesh->SetRasterizerState( ERS_STATE::None );
}

//-----------------------------.
// ����ł��邩�m�F����.
//-----------------------------.
template<typename T>
void CMovieActor<T>::ConfirmAcive()
{
	if( m_IsAcive == true ) return;
	if( m_NowTime < m_MovieState.AciveStartTime ) return;

	m_IsAcive = true;
}