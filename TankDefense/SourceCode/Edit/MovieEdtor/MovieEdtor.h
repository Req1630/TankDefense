#ifndef MOVIE_EDITOR_H
#define MOVIE_EDITOR_H

#include "..\EditorBase.h"
#include "..\..\Object\CameraBase\MovieCamera\MovieCamera.h"

class CMovie;			// ���[�r�[(���o)�N���X.
class CCameraEditor;	// �J�����G�f�B�^.
class CStageRender;		// �X�e�[�W�`��N���X.

/******************************************
*	���o�G�f�B�^�N���X.
**/
class CMovieEditor : public CEditorBase
{
public:
	CMovieEditor();
	virtual ~CMovieEditor();

	// ������.
	virtual bool Init() override;
	// �X�V.
	virtual void Update() override;
	// ImGui�`��.
	virtual bool ImGuiRender() override;
	// ���f���`��.
	virtual void ModelRender() override;
	// �G�t�F�N�g�`��.
	virtual void EffectRneder() override;

private:
	// �Đ��̕\��.
	void PlayDraw();
	// �p�����[�^�̏�������.
	virtual void ParameterWriting( const char* filePath ) override;
	// �p�����[�^�̓ǂݍ���.
	virtual void ParameterLoading( const char* filePath ) override;

private:
	std::unique_ptr<CMovie>			m_pMovie;			// ���[�r�[.
	std::unique_ptr<CCameraEditor>	m_pCameraEdit;		// �J�����G�f�B�^.
	std::unique_ptr<CStageRender>	m_pStageRender;		// �X�e�[�W�̕`��.
	float							m_MovieEndSecTime;	// ����I������.

	bool							m_IsMoviePlaying;	// �Đ������ǂ���.

};

#endif	// #ifndef MOVIE_EDITOR_H.