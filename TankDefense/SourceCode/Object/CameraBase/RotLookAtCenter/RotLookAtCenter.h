/**
* @file RotLookAtCenter.h.
* @brief �^�[�Q�b�g�𒆐S�ɉ�]����J�����N���X.
* @author ���c���.
*/
#ifndef ROTATION_LOOK_AT_CENTER_CAMERA_H
#define ROTATION_LOOK_AT_CENTER_CAMERA_H

#include "..\CameraBase.h"

/********************************************
*	�^�[�Q�b�g�𒆐S�ɉ�]����J�����N���X.
**/
class CRotLookAtCenter : public CCameraBase
{
private: //------ �萔 -------.
	static constexpr float DEFAULT_LENGTH			= 20.0f;	// �J�����ʒu�ƒ����ʒu�̋���.
	static constexpr float DEFAULT_HORIZONTAL_MAX	= 360.0f;	// ����]�̍ő�T�C�Y.
	static constexpr float DEFAULT_HORIZONTAL_MIN	= 0.0f;		// ����]�̍ŏ��T�C�Y.
	static constexpr float DEFAULT_VERTICAL_MAX		= 90.0f;	// �c��]�̍ő�T�C�Y.
	static constexpr float DEFAULT_VERTICAL_MIN		= 0.0f;		// �c��]�̍ŏ��T�C�Y.
	static constexpr float DEFAULT_MOVE_SPEED		= 0.01f;	// �ړ����x.

public:
	// �J�����ݒ�t�@�C���̃p�X.
	inline static const char*	CONFIG_FILE_PATH		= "Data\\Config\\Camera.bin";
	// �J�����ݒ�̏��.
	struct stConfigState
	{
		bool	IsReverse;	// ���]���邩.
		float	MoveSpeed;	// �ړ����x.

		stConfigState()
			: IsReverse	( false )
			, MoveSpeed	( 0.01f )
		{}
	} typedef SConfigState;

public:  //------ �֐� -------.
	CRotLookAtCenter();
	CRotLookAtCenter( const bool& isLoad );
	virtual ~CRotLookAtCenter();

	// �p�����[�^�[�̓ǂݍ���.
	bool LoadParameter( const bool& isLoad );

	// �X�V.
	virtual void Update( const float& deltaTime ) override;

	// �I�u�W�F�N�g���𒍎��ɉ�].
	void RotationLookAtObject( const D3DXVECTOR3& vLookPos, const float& attenRate, const bool& isVert = false );

	// �������̉�].
	void DegreeHorizontalMove( const float& movePower );
	// �������̉E��].
	void DegreeHorizontalRightMove();
	// �������̍���].
	void DegreeHorizontalLeftMove();
	// �c�����̉�].
	void DegreeVerticalMove( const float& movePower );
	// ���W�A���̎擾�֐�.
	float GetRadianX() const { return m_vDegree.x; }
	// �J�����ƒ����_�̒����ݒ�.
	void SetLength( const float& length );
	// �����̐ݒ�.
	void SetHeight( const float& height );
	// �������̍ő�A�ŏ��̐ݒ�.
	void SetHorizontalDegree( const float& max, const float& min );
	// �c�����̍ő�A�ŏ��̐ݒ�.
	void SetVerticalDegree( const float& max, const float& min );
	// ���W�ɑ������킹��l�̐ݒ�.
	void SetAddPositionValue( const D3DXVECTOR3& addValue );

	// �E�����̉�]���x.
	void SetRightMoveSpeed( const float& speed ){ m_RightMoveSpeed = speed; }
	// �������̉�]���x.
	void SetLeftMoveSpeed( const float& speed ){ m_LeftMoveSpeed = speed; }

	// ��]���x�̎擾.
	inline float GetMoveSpeed() { return DEFAULT_MOVE_SPEED; }

private: //------ �ϐ� -------.
	D3DXVECTOR2		m_vDegree;			// �J�����̊p�x.
	float			m_Length;			// �J�����ƒ����_�̒���.
	float			m_HorizontalMax;	// �������̍ő�T�C�Y.
	float			m_HorizontalMin;	// �������̍ŏ��T�C�Y.
	float			m_VerticalMax;		// �c�����̍ő�T�C�Y.
	float			m_VerticalMin;		// �c�����̍ŏ��T�C�Y.
	float			m_RightMoveSpeed;	// �E�̈ړ����x.
	float			m_LeftMoveSpeed;	// ���̈ړ����x.
	D3DXVECTOR3		m_AddValue;
};

#endif	// #ifndef ROTATION_LOOK_AT_CENTER_CAMERA_H.