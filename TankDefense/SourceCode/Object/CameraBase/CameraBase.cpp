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
// �w��b�ŁA�w����W�Ɉړ�����J��������.
//-----------------------------------------------.
void CCameraBase::AnySecondsMove( const SCameraState& newState,const SCameraState& oldState, const float& sec )
{
	const float seconds = sec <= 0.0f ? 0.0f : sec;
	// ���݂̍��W, �̂̍��W, �w����W,
	D3DXVec3Lerp( &m_Tranceform.Position, &newState.Position, &oldState.Position, seconds );
	D3DXVec3Lerp( &m_LookPosition, &newState.LookPosition, &oldState.LookPosition, seconds );
}

//-----------------------------------------------.
// �w��b�ŁA�w����W�Ɉړ�����J��������.
//-----------------------------------------------.
void CCameraBase::AnySecondsMovePosition( const D3DXVECTOR3& newPos, const D3DXVECTOR3& oldPos, const float& sec )
{
	const float seconds = sec <= 0.0f ? 0.0f : sec;
	D3DXVec3Lerp( &m_Tranceform.Position, &newPos, &oldPos, seconds );
}

//-----------------------------------------------.
// �w��b�ŁA�w����W�Ɉړ�����J��������.
//-----------------------------------------------.
void CCameraBase::AnySecondsMoveLookPosition( const D3DXVECTOR3& newPos, const D3DXVECTOR3& oldPos, const float& sec )
{
	const float seconds = sec <= 0.0f ? 0.0f : sec;
	D3DXVec3Lerp( &m_LookPosition, &newPos, &oldPos, seconds );
}

//-----------------------------------------------.
// ���W���c�����ɗh�炷.
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
// ���W���c�����ɗh�炷.
//-----------------------------------------------.
void CCameraBase::ShakePositoinVertical( const SShakeState& state )
{
	ShakePositoinVertical( state.Amplitube, state.Frequency, state.Time, state.BasePos, state.Attenuation );
}

//-----------------------------------------------.
// ���W���������ɗh�炷.
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
	// ���x�N�g����p��.
	D3DXVECTOR3 vecAxisX = Math::X_AXIS;
	// X���x�N�g�����̂��̂���]��Ԃɂ��ϊ�����.
	D3DXVec3TransformCoord( &vecAxisX, &vecAxisX, &mRot );

	const float sample = amplitube * sinf(Math::M_2PI*frequency*time)*attenuation;
	m_Tranceform.Position.x = basePos.x + vecAxisX.x * sample;
	m_Tranceform.Position.z = basePos.z + vecAxisX.z * sample;
}

//-----------------------------------------------.
// ���W���������ɗh�炷.
//-----------------------------------------------.
void CCameraBase::ShakePositoinHorizontal( const SShakeState& state )
{
	ShakePositoinHorizontal( state.Amplitube, state.Frequency, state.Time, state.BasePos, state.Attenuation );
}

//-----------------------------------------------.
// ���_���W���c�����ɗh�炷.
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
// ���_���W���c�����ɗh�炷.
//-----------------------------------------------.
void CCameraBase::ShakeLookPositoinVertical( const SShakeState& state )
{
	ShakeLookPositoinVertical( state.Amplitube, state.Frequency, state.Time, state.BasePos, state.Attenuation );
}

//-----------------------------------------------.
// ���_���W���������ɗh�炷.
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
	// ���x�N�g����p��.
	D3DXVECTOR3 vecAxisX = Math::X_AXIS;
	// X���x�N�g�����̂��̂���]��Ԃɂ��ϊ�����.
	D3DXVec3TransformCoord( &vecAxisX, &vecAxisX, &mRot );

	const float sample = amplitube * sinf(Math::M_2PI*frequency*time)*attenuation;
	m_LookPosition.x = basePos.x + vecAxisX.x * sample;
	m_LookPosition.z = basePos.z + vecAxisX.z * sample;
}

//-----------------------------------------------.
// ���_���W���������ɗh�炷.
//-----------------------------------------------.
void CCameraBase::ShakeLookPositoinHorizontal( const SShakeState& state )
{
	ShakeLookPositoinHorizontal( state.Amplitube, state.Frequency, state.Time, state.BasePos, state.Attenuation );
}

//-----------------------------------------------.
// �r���[�E�v���W�F�N�V�����̍X�V.
//-----------------------------------------------.
void CCameraBase::UpdateViewProj()
{
	// �r���[(�J����)�ϊ�.
	D3DXMatrixLookAtLH( 
		&m_ViewMatrix,				// (out)viewMarix.
		&m_Tranceform.Position,		// �J�������W.
		&m_LookPosition,			// �J�����������W.
		&m_ViewVector );			// �J�����x�N�g��.

	// �v���W�F�N�V����(�ˉe)�ϊ�.
	D3DXMatrixPerspectiveFovLH(
		&m_ProjMatrix,				// (out)ProjMatrix.
		m_ViewAngle,				// ����p.
		m_Aspect,					// ��ʃA�X�y�N�g��.
		m_MinClippingDistance,		// �ŏ��`�拗��.
		m_MaxClippingDistance );	// �ő�`�拗��.
}