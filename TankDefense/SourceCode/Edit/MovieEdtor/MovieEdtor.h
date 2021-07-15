#ifndef MOVIE_EDITOR_H
#define MOVIE_EDITOR_H

#include "..\EditorBase.h"
#include "..\..\Object\CameraBase\MovieCamera\MovieCamera.h"
#include "..\..\Object\GameObject\Actor\Stage\StageStruct.h"

class CMovie;			// ���[�r�[(���o)�N���X.
class CCameraEditor;	// �J�����G�f�B�^.
class CMovieActorEditor;// �A�N�^�[�G�f�B�^.
class CWidgetEditor;	// �E�B�W�F�b�g�G�f�B�^.
class CStageRender;		// �X�e�[�W�`��N���X.
class CMovieDataLoader;	// ���[�r�f�[�^�ǂݍ��݃N���X.
enum class enMovieList typedef EMovieNo;

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
	// �E�B�W�F�b�g�`��.
	virtual void WidgetRender() override;

	// �G�f�B�^�p�v���C���[�̐ݒ�.
	virtual void SetEditPlayer( CEditPlayer* pPlayer ) override;

private:
	// �Đ��̕\��.
	void PlayDraw();
	// ���[�r�[�̑I��\��.
	void SelectMovieDraw();
	// �ۑ��A�Ǎ��{�^���̑I��.
	void SaveLoadDraw();
	// �p�����[�^�̏�������.
	virtual void ParameterWriting( const char* filePath ) override;
	// �p�����[�^�̓ǂݍ���.
	virtual void ParameterLoading( const char* filePath ) override;

private:
	std::unordered_map<EMovieNo, std::string>	m_MovieNameList;	// ���[�r�[�����X�g.
	std::unique_ptr<CMovieDataLoader>			m_pMovieDataLoader;	// ���[�r�[�ǂݍ��݃N���X.
	std::unique_ptr<CMovie>						m_pMovie;			// ���[�r�[.
	std::unique_ptr<CCameraEditor>				m_pCameraEdit;		// �J�����G�f�B�^.
	std::unique_ptr<CMovieActorEditor>			m_pActorEdit;		// �A�N�^�[�G�f�B�^.
	std::unique_ptr<CWidgetEditor>				m_pWidgetEditor;	// �E�B�W�F�b�g�G�f�B�^.
	std::unique_ptr<CStageRender>				m_pStageRender;		// �X�e�[�W�̕`��.
	EStageNo	m_NowStage;			// ���݂̃X�e�[�W.
	EMovieNo	m_NowSelectMovie;	// ���ݑI�����Ă��郀�[�r�[.
	float		m_MovieEndSecTime;	// ����I������.
	bool		m_IsMoviePlaying;	// �Đ������ǂ���.

};

#endif	// #ifndef MOVIE_EDITOR_H.