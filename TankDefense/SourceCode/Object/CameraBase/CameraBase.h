/**
* @file CameraBase.h.
* @brief �J�������N���X.
* @author ���c���.
*/
#ifndef CAMERA_BASE_H
#define CAMERA_BASE_H

#include "..\Object.h"

/****************************************
*	�J�����x�[�X�N���X.
**/
class CCameraBase : public CObject
{
public:
	// �J�������.
	struct stCameraState
	{
		D3DXVECTOR3 Position;		// ���W.
		D3DXVECTOR3 LookPosition;	// ���_���W.

//		float	operator[] ( size_t i ) const	{ return (&*Position)[i]; }
//		float&	operator[] ( size_t i )			{ return (&*Position)[i]; }
	} typedef SCameraState;

	// �J������h�炷���߂̏��.
	struct stShakeState
	{
		float			Amplitube;	// �U�ꕝ(�l���傫����Α傫���h���).
		float			Frequency;	// ���g��(��b�Ԃɉ���h�炷��).
		float			Time;		// �h�炷����.
		D3DXVECTOR3		BasePos;	// ��ƂȂ���W.
		float			Attenuation;// �����l 0.0f ~ 1.0f�̒l������ƌ�������.
	} typedef SShakeState;

public:
	CCameraBase();
	virtual ~CCameraBase();

	// �X�V�֐�.
	virtual void Update( const float& deltaTime ) = 0;

	// �w��b�ŁA�w����W�Ɉړ�����J��������.
	void AnySecondsMove( const SCameraState& newState, const SCameraState& oldState, const float& sec );
	// �w��b�ŁA�w����W�Ɉړ�����J��������.
	void AnySecondsMovePosition( const D3DXVECTOR3& newPos, const D3DXVECTOR3& oldPos, const float& sec );
	// �w��b�ŁA�w����W�Ɉړ�����J��������.
	void AnySecondsMoveLookPosition( const D3DXVECTOR3& newPos, const D3DXVECTOR3& oldPos, const float& sec );

	// ���W���c�����ɗh�炷.
	//	amplitube	�U�ꕝ(�l���傫����Α傫���h���).
	//	frequency	���g��(��b�Ԃɉ���h�炷��).
	//	time		�h�炷����.
	//	basePos		��ƂȂ���W.
	//	attenuation	�����l 0.0f ~ 1.0f�̒l������ƌ�������.
	void ShakePositoinVertical(
		const float& amplitube,
		const float& frequency,
		const float& time,
		const D3DXVECTOR3& basePos,
		const float& attenuation = 1.0f );
	// ���W���c�����ɗh�炷.
	void ShakePositoinVertical( const SShakeState& state );

	// ���W���������ɗh�炷.
	//	amplitube	�U�ꕝ(�l���傫����Α傫���h���).
	//	frequency	���g��(��b�Ԃɉ���h�炷��).
	//	time		�h�炷����.
	//	basePos		��ƂȂ���W.
	//	attenuation	�����l 0.0f ~ 1.0f�̒l������ƌ�������.
	void ShakePositoinHorizontal(
		const float& amplitube,
		const float& frequency,
		const float& time,
		const D3DXVECTOR3& basePos,
		const float& attenuation = 1.0f );
	// ���W���������ɗh�炷.
	void ShakePositoinHorizontal( const SShakeState& state );

	// ���_���W���c�����ɗh�炷.
	//	amplitube	�U�ꕝ(�l���傫����Α傫���h���).
	//	frequency	���g��(��b�Ԃɉ���h�炷��).
	//	time		�h�炷����.
	//	basePos		��ƂȂ���W.
	//	attenuation	�����l 0.0f ~ 1.0f�̒l������ƌ�������.
	void ShakeLookPositoinVertical(
		const float& amplitube,
		const float& frequency,
		const float& time,
		const D3DXVECTOR3& basePos,
		const float& attenuation = 1.0f );
	// ���_���W���c�����ɗh�炷.
	void ShakeLookPositoinVertical( const SShakeState& state );

	// ���_���W���������ɗh�炷.
	//	amplitube	�U�ꕝ(�l���傫����Α傫���h���).
	//	frequency	���g��(��b�Ԃɉ���h�炷��).
	//	time		�h�炷����.
	//	basePos		��ƂȂ���W.
	//	attenuation	�����l 0.0f ~ 1.0f�̒l������ƌ�������.
	void ShakeLookPositoinHorizontal(
		const float& amplitube,
		const float& frequency,
		const float& time,
		const D3DXVECTOR3& basePos,
		const float& attenuation = 1.0f );
	// ���_���W���������ɗh�炷.
	void ShakeLookPositoinHorizontal( const SShakeState& state );

	// �r���[�E�v���W�F�N�V�����̍X�V.
	void UpdateViewProj();

	// �������W�̐ݒ�.
	inline void SetLookPosition( const D3DXVECTOR3& p ) { m_LookPosition = p; }
	// �������W�̎擾.
	inline D3DXVECTOR3 GetLookPosition() const { return m_LookPosition; }

	// �r���[�s��擾�֐�.
	inline D3DXMATRIX GetViewMatrix() const { return m_ViewMatrix; }
	// �v���W�F�N�V�����s��擾�֐�.
	inline D3DXMATRIX GetProjMatrix() const { return m_ProjMatrix; }
	// �r���[�E�v���W�F�N�V�����̍����s��擾�֐�.
	inline D3DXMATRIX GetViewProjMatrix() const { return m_ViewMatrix * m_ProjMatrix; }

	// �����Ԃ�.
	inline bool IsActive() const { return m_IsActive; }

protected:
	D3DXMATRIX	m_ViewMatrix;			// �r���[�s��.
	D3DXMATRIX	m_ProjMatrix;			// �v���W�F�N�V�����s��.
	D3DXVECTOR3	m_LookPosition;			// �J�����������W.
	D3DXVECTOR3	m_ViewVector;			// �r���[�x�N�g��.
	float		m_ViewAngle;			// ����p.
	float		m_Aspect;				// �A�X�y�N�g��.
	float		m_MaxClippingDistance;	// �ő�N���b�s���O����.
	float		m_MinClippingDistance;	// �ŏ��N���b�s���O����.
	bool		m_IsActive;				// ���삵�Ă��邩.
};

#endif	// #ifndef CAMERA_BASE_H.