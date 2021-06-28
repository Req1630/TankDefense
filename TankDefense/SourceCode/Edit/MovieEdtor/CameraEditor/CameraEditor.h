#ifndef CAMERA_EDITOR_H
#define CAMERA_EDITOR_H

#include "..\..\..\Object\CameraBase\MovieCamera\MovieCamera.h"

class CNormalCamera;

/********************************
*	���o�p�J�����G�f�B�^.
**/
class CCameraEditor
{
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
	inline SMovieCamera GetMovieCameraState() { return m_MovieCamera; }

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

private:
	std::unique_ptr<CNormalCamera>	m_pCamera;
	CCameraBase::SCameraState		m_CameraState;
	SMovieCamera					m_MovieCamera;
	D3DXVECTOR2						m_Radian;			// ���W�A��.
	float							m_DeltaTime;		// �f���^�^�C��.
	float							m_PosMoveTime;		// �ړ�����.
	float							m_LookPosMoveTime;	// �����ʒu�ړ�����.
	bool							m_IsCameraControll;	// �J��������ł��邩.
	bool							m_IsCameraPlaying;	// �J�����Đ�����.
};

#endif	// #ifndef CAMERA_EDITOR_H.