#include "CameraBase.h"
#include "..\..\Utility\Math\Math.h"

namespace
{
	const float			DEFFULT_ASPECT			= static_cast<float>(WND_W)/static_cast<float>(WND_H);
	const float			MAX_CLIPPING_DSTANCE	= 1000.0f;
	const float			MIN_CLIPPING_DSTANCE	= 0.01f;
	const D3DXVECTOR3	INIT_CAMERA_POSITION	= { 0.0f, 5.0f, -15.0f };
};

CCameraBase::CCameraBase()
	: m_ViewMatrix			()
	, m_ProjMatrix			()
	, m_LookPosition		( 0.0f, 0.0f, 0.0f )
	, m_ViewVector			( 0.0f, 1.0f, 0.0f )
	, m_ViewAngle			( static_cast<float>(D3DX_PI/4.0) )
	, m_Aspect				( DEFFULT_ASPECT )
	, m_MaxClippingDistance	( MAX_CLIPPING_DSTANCE )
	, m_MinClippingDistance	( MIN_CLIPPING_DSTANCE )
	, m_IsActive			( false )
{
	m_Tranceform.Position = INIT_CAMERA_POSITION;
}

CCameraBase::~CCameraBase()
{
}

//-----------------------------------------------.
// 指定秒で、指定座標に移動するカメラ動作.
//-----------------------------------------------.
void CCameraBase::AnySecondsMove( const SCameraState& newState,const SCameraState& oldState, const float& sec )
{
	const float seconds = sec <= 0.0f ? 0.0f : sec;
	// 現在の座標, 昔の座標, 指定座標,
	D3DXVec3Lerp( &m_Tranceform.Position, &newState.Position, &oldState.Position, seconds );
	D3DXVec3Lerp( &m_LookPosition, &newState.LookPosition, &oldState.LookPosition, seconds );
}

//-----------------------------------------------.
// 指定秒で、指定座標に移動するカメラ動作.
//-----------------------------------------------.
void CCameraBase::AnySecondsMovePosition( const D3DXVECTOR3& newPos, const D3DXVECTOR3& oldPos, const float& sec )
{
	const float seconds = sec <= 0.0f ? 0.0f : sec;
	D3DXVec3Lerp( &m_Tranceform.Position, &newPos, &oldPos, seconds );
}

//-----------------------------------------------.
// 指定秒で、指定座標に移動するカメラ動作.
//-----------------------------------------------.
void CCameraBase::AnySecondsMoveLookPosition( const D3DXVECTOR3& newPos, const D3DXVECTOR3& oldPos, const float& sec )
{
	const float seconds = sec <= 0.0f ? 0.0f : sec;
	D3DXVec3Lerp( &m_LookPosition, &newPos, &oldPos, seconds );
}

//-----------------------------------------------.
// 座標を縦方向に揺らす.
//-----------------------------------------------.
void CCameraBase::ShakePositoinVertical(
	const float& amplitube,
	const float& frequency,
	const float& time,
	const D3DXVECTOR3& basePos,
	const float& attenuation )
{
	m_Tranceform.Position.y = basePos.y + amplitube * sinf(Math::M_2PI*frequency*time)*attenuation;
}

//-----------------------------------------------.
// 座標を縦方向に揺らす.
//-----------------------------------------------.
void CCameraBase::ShakePositoinVertical( const SShakeState& state )
{
	ShakePositoinVertical( state.Amplitube, state.Frequency, state.Time, state.BasePos, state.Attenuation );
}

//-----------------------------------------------.
// 座標を横方向に揺らす.
//-----------------------------------------------.
void CCameraBase::ShakePositoinHorizontal(
	const float& amplitube,
	const float& frequency,
	const float& time,
	const D3DXVECTOR3& basePos,
	const float& attenuation )
{
	const D3DXVECTOR3 vec = m_Tranceform.Position - m_LookPosition;
	D3DXMATRIX mRot;
	D3DXMatrixRotationYawPitchRoll( &mRot, atan2f( vec.x, vec.z ), 0.0f, 0.0f );
	// 軸ベクトルを用意.
	D3DXVECTOR3 vecAxisX = Math::X_AXIS;
	// X軸ベクトルそのものを回転状態により変換する.
	D3DXVec3TransformCoord( &vecAxisX, &vecAxisX, &mRot );

	const float sample = amplitube * sinf(Math::M_2PI*frequency*time)*attenuation;
	m_Tranceform.Position.x = basePos.x + vecAxisX.x * sample;
	m_Tranceform.Position.z = basePos.z + vecAxisX.z * sample;
}

//-----------------------------------------------.
// 座標を横方向に揺らす.
//-----------------------------------------------.
void CCameraBase::ShakePositoinHorizontal( const SShakeState& state )
{
	ShakePositoinHorizontal( state.Amplitube, state.Frequency, state.Time, state.BasePos, state.Attenuation );
}

//-----------------------------------------------.
// 視点座標を縦方向に揺らす.
//-----------------------------------------------.
void CCameraBase::ShakeLookPositoinVertical(
	const float& amplitube,
	const float& frequency,
	const float& time,
	const D3DXVECTOR3& basePos,
	const float& attenuation )
{
	m_LookPosition.y = basePos.y + amplitube * sinf(Math::M_2PI*frequency*time)*attenuation;
}

//-----------------------------------------------.
// 視点座標を縦方向に揺らす.
//-----------------------------------------------.
void CCameraBase::ShakeLookPositoinVertical( const SShakeState& state )
{
	ShakeLookPositoinVertical( state.Amplitube, state.Frequency, state.Time, state.BasePos, state.Attenuation );
}

//-----------------------------------------------.
// 視点座標を横方向に揺らす.
//-----------------------------------------------.
void CCameraBase::ShakeLookPositoinHorizontal(
	const float& amplitube,
	const float& frequency,
	const float& time,
	const D3DXVECTOR3& basePos,
	const float& attenuation )
{
	const D3DXVECTOR3 vec = m_Tranceform.Position - m_LookPosition;
	D3DXMATRIX mRot;
	D3DXMatrixRotationYawPitchRoll( &mRot, atan2f( vec.x, vec.z ), 0.0f, 0.0f );
	// 軸ベクトルを用意.
	D3DXVECTOR3 vecAxisX = Math::X_AXIS;
	// X軸ベクトルそのものを回転状態により変換する.
	D3DXVec3TransformCoord( &vecAxisX, &vecAxisX, &mRot );

	const float sample = amplitube * sinf(Math::M_2PI*frequency*time)*attenuation;
	m_LookPosition.x = basePos.x + vecAxisX.x * sample;
	m_LookPosition.z = basePos.z + vecAxisX.z * sample;
}

//-----------------------------------------------.
// 視点座標を横方向に揺らす.
//-----------------------------------------------.
void CCameraBase::ShakeLookPositoinHorizontal( const SShakeState& state )
{
	ShakeLookPositoinHorizontal( state.Amplitube, state.Frequency, state.Time, state.BasePos, state.Attenuation );
}

//-----------------------------------------------.
// ビュー・プロジェクションの更新.
//-----------------------------------------------.
void CCameraBase::UpdateViewProj()
{
	// ビュー(カメラ)変換.
	D3DXMatrixLookAtLH( 
		&m_ViewMatrix,				// (out)viewMarix.
		&m_Tranceform.Position,		// カメラ座標.
		&m_LookPosition,			// カメラ注視座標.
		&m_ViewVector );			// カメラベクトル.

	// プロジェクション(射影)変換.
	D3DXMatrixPerspectiveFovLH(
		&m_ProjMatrix,				// (out)ProjMatrix.
		m_ViewAngle,				// 視野角.
		m_Aspect,					// 画面アスペクト比.
		m_MinClippingDistance,		// 最小描画距離.
		m_MaxClippingDistance );	// 最大描画距離.
}