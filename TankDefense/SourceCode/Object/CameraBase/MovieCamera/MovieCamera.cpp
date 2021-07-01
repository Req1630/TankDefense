#include "MovieCamera.h"
#include "..\..\..\Utility\Input\Input.h"

CMovieCamera::CMovieCamera()
	: m_MovieCameraState		()
	, m_PosMoveTime				( 1.0f )
	, m_LookPosMoveTime			( 1.0f )
	, m_IsMoveCameraPlaying		( false )
	, m_IsShakeCameraPlaying	( false )
	, m_PlayShakeState			( SMovieShakeCamera::EShakePosNo_Max )
{
	m_IsActive = false;
}

CMovieCamera::~CMovieCamera()
{
}

//------------------------------.
// 更新関数.
//------------------------------.
void CMovieCamera::Update( const float& deltaTime )
{
	m_DeltaTime = deltaTime;

	if( m_IsActive == false ) return;

	MoveCamera();
	ShakeCamera();

	if( m_IsMoveCameraPlaying	== false && 
		m_IsShakeCameraPlaying	== false ){
		m_IsActive = false;
	}
}

//------------------------------.
// 再生.
//------------------------------.
void CMovieCamera::Play()
{
	m_IsMoveCameraPlaying	= true;
	m_IsShakeCameraPlaying	= true;
	m_IsActive				= true;
	m_PosMoveTime			= 1.0f;
	m_LookPosMoveTime		= 1.0f;
	m_Tranceform.Position	= m_MovieCameraState.MoveState.StartState.Position;
	m_LookPosition			= m_MovieCameraState.MoveState.StartState.LookPosition;

	if( m_MovieCameraState.MoveState.PosMoveTime		<= 0.0f || 
		m_MovieCameraState.MoveState.LookPosMoveTime	<= 0.0f ){
		m_IsMoveCameraPlaying = false;
	}

	for( int i = 0; i < SMovieShakeCamera::EShakePosNo_Max; i++ ){
		const bool isPlay = m_MovieCameraState.ShakeState.ShakeState[i].IsPlaying;
		m_PlayShakeState[i].Time			= isPlay ? m_MovieCameraState.ShakeState.ShakeState[i].Time : 0.0f;
		m_PlayShakeState[i].IsAttenuation	= m_MovieCameraState.ShakeState.ShakeState[i].IsAttenuation;
		m_PlayShakeState[i].Attenuation		= 1.0f;
	}

}

//------------------------------.
// 値のリセット.
//------------------------------.
void CMovieCamera::Reset()
{
}

//------------------------------.
// カメラの移動.
//------------------------------.
void CMovieCamera::MoveCamera()
{
	if( m_IsMoveCameraPlaying == false ) return;

	const SMovieMoveCamera& moveState = m_MovieCameraState.MoveState;

	// 指定秒で、指定座標に移動するカメラ動作.
	AnySecondsMovePosition( 
		moveState.EndState.Position, 
		moveState.StartState.Position, 
		m_PosMoveTime );
	// 指定秒で、指定座標に移動するカメラ動作.
	AnySecondsMoveLookPosition( 
		moveState.EndState.LookPosition, 
		moveState.StartState.LookPosition, 
		m_LookPosMoveTime );

	m_PosMoveTime		-= m_DeltaTime / moveState.PosMoveTime;
	m_LookPosMoveTime	-= m_DeltaTime / moveState.LookPosMoveTime;

	if( m_PosMoveTime < 0.0f && m_LookPosMoveTime < 0.0f ){
		m_IsMoveCameraPlaying = false;
	}
}

//------------------------------.
// カメラを揺らす.
//------------------------------.
void CMovieCamera::ShakeCamera()
{
	if( m_IsShakeCameraPlaying == false ) return;

	const SMovieShakeCamera& shakeState = m_MovieCameraState.ShakeState;

	int playEndCount = 0;
	for( int i = 0; i < SMovieShakeCamera::EShakePosNo_Max; i++ ){
		if( m_PlayShakeState[i].Time <= 0.0f ){
			playEndCount++;
			continue;
		}
		const float at = m_PlayShakeState[i].IsAttenuation == true ? 
			m_PlayShakeState[i].Time / shakeState.ShakeState[i].Time : 1.0f;
		CCameraBase::SShakeState ss;
		ss.Frequency	= shakeState.ShakeState[i].Frequency;
		ss.Amplitube	= shakeState.ShakeState[i].Amplitube;
		ss.BasePos		= shakeState.ShakeState[i].BasePosition;
		ss.Time			= m_PlayShakeState[i].Time;
		ss.Attenuation	= at;
		switch( i )
		{
		case SMovieShakeCamera::EShakePosNo_PosVer:
			ShakePositoinVertical( ss );
			break;
		case SMovieShakeCamera::EShakePosNo_PosHor:
			ShakePositoinHorizontal( ss );
			break;
		case SMovieShakeCamera::EShakePosNo_LookPosVer:
			ShakeLookPositoinVertical( ss );
			break;
		case SMovieShakeCamera::EShakePosNo_LookPosHor:
			ShakeLookPositoinHorizontal( ss );
			break;
		default:
			break;
		}
		m_PlayShakeState[i].Time -= m_DeltaTime;
		if( m_PlayShakeState[i].Time < 0.0f ){
			playEndCount++;
		}
	}
	if( playEndCount == SMovieShakeCamera::EShakePosNo_Max ){
		m_IsShakeCameraPlaying = false;
	}
}

