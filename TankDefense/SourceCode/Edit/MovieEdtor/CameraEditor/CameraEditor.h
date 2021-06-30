#ifndef CAMERA_EDITOR_H
#define CAMERA_EDITOR_H

#include "..\..\..\Object\CameraBase\MovieCamera\MovieCamera.h"

class CMovieCamera;

/********************************
*	���o�p�J�����G�f�B�^.
**/
class CCameraEditor
{
	enum enShakePosFlags : unsigned char
	{
		EShakePosFlag_None		= 0,

		EShakePosFlag_PosVer		= 1 << 0,	// ���W�̏c����.
		EShakePosFlag_PosHor		= 1 << 1,	// ���W�̉�����.
		EShakePosFlag_LookPosVer	= 1 << 2,	// ���_���W�̏c����.
		EShakePosFlag_LookPosHor	= 1 << 3,	// ���_�A�[�̉�����.

		EShakePosFlag_max,
	} typedef EShakePosFlag;
	using shake_pos_flag = unsigned int;
public:
	CCameraEditor();
	~CCameraEditor();

	// �X�V����.
	void Update();
	// ImGui�`��.
	bool ImGuiRender();
	// ���f���`��.
	void ModelRender();

	// ���o�J�������̎擾.
	inline SMovieMoveCamera GetMovieCameraState() { return m_MovieMoveCamera; }

private:
	// �J��������.
	void CameraController();
	// �J�����Đ�.
	void CameraPlaying();
	// ���W�̍X�V.
	void PositionUpdate();
	// ���_���W�̍X�V.
	void LookPositionUpdate();

	// �J�����ړ������̐؂�ւ�.
	void ChangeMoveCamera();

	// �ړ��J�����̕\��.
	void MoveCameraDraw();
	// �h��J�����̕\��.
	void ShakeCameraDraw();

private:
	std::unique_ptr<CMovieCamera>	m_pCamera;
	CCameraBase::SCameraState		m_CameraState;
	SMovieMoveCamera				m_MovieMoveCamera;
	SMovieShakeCamera				m_MovieShakeCamera;
	D3DXVECTOR2						m_Radian;			// ���W�A��.
	shake_pos_flag					m_ShakePosFlag;		// �h����W�̃t���O.
	float							m_DeltaTime;		// �f���^�^�C��.
	bool							m_IsCameraControll;	// �J��������ł��邩.
	bool							m_IsCameraPlaying;	// �J�����Đ�����.
};

#endif	// #ifndef CAMERA_EDITOR_H.