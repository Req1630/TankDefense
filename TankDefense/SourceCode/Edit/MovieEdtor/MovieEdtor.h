#ifndef MOVIE_EDITOR_H
#define MOVIE_EDITOR_H

#include "..\EditorBase.h"
#include "..\..\Object\CameraBase\MovieCamera\MovieCamera.h"

class CCameraEditor;
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
	// �p�����[�^�̏�������.
	virtual void ParameterWriting( const char* filePath ) override;
	// �p�����[�^�̓ǂݍ���.
	virtual void ParameterLoading( const char* filePath ) override;

private:
	std::unique_ptr<CCameraEditor>	m_pCameraEdit;
	std::unique_ptr<CStageRender>	m_pStageRender;
	float	m_MovieEndSecTime;		// ����I������.

};

#endif	// #ifndef MOVIE_EDITOR_H.