#include "MovieCamera.h"
#include "..\..\..\Utility\Input\Input.h"

CMoiveCamera::CMoiveCamera()
	: m_MovieCameraState	()
	, m_PosMoveTime			( 1.0f )
	, m_LookPosMoveTime		( 1.0f )
{
	m_IsActive = true;
}

CMoiveCamera::~CMoiveCamera()
{
}

//------------------------------.
// 更新関数.
//------------------------------.
void CMoiveCamera::Update( const float& deltaTime )
{
	// 指定秒で、指定座標に移動するカメラ動作.
	AnySecondsMovePosition( 
		m_MovieCameraState.EndState.Position, 
		m_MovieCameraState.StartState.Position, 
		m_PosMoveTime );
	// 指定秒で、指定座標に移動するカメラ動作.
	AnySecondsMoveLookPosition( 
		m_MovieCameraState.EndState.LookPosition, 
		m_MovieCameraState.StartState.LookPosition, 
		m_LookPosMoveTime );

	m_PosMoveTime		-= deltaTime / m_MovieCameraState.PosMoveTime;
	m_LookPosMoveTime	-= deltaTime / m_MovieCameraState.LookPosMoveTime;
	if( m_PosMoveTime		<= 0.0f ) m_PosMoveTime		= 0.0f;
	if( m_LookPosMoveTime	<= 0.0f ) m_LookPosMoveTime	= 0.0f;

	if( m_PosMoveTime <= 0.0f && m_LookPosMoveTime <= 0.0f ){
		// カメラ動作終了.
		m_IsActive = false;
	}
}
