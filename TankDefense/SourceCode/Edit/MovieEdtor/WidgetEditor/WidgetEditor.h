#ifndef WIDGET_EDITOR_H
#define WIDGET_EDITOR_H

#include "..\..\..\Object\GameObject\Widget\MovieWidget\MovieWidget.h"

/**************************************
*	�摜�G�f�B�^�N���X.
**/
class CWidgetEditor
{
public:
	CWidgetEditor();
	~CWidgetEditor();

	// ������.
	bool Init();
	// �X�V.
	void Update();
	// ImGui�`��.
	bool ImGuiRender();
	// �摜�̕`��.
	void SpriteRender();

	// �E�B�W�F�b�g���̎擾.
	std::vector<SMovieWidget> GetWidgetStateList();
	// �E�B�W�F�b�g���̎擾.
	void SetWidgetStateList( const std::vector<SMovieWidget>& stateList );

private:
	// ����֐�.
	void WidgetControll();
	// �E�B�W�F�b�g�̒ǉ��̕\��.
	void PushWidgetDraw();
	// �ҏW����E�B�W�F�b�g�̕\��.
	void SelectEidtParameterDraw();
	// �E�B�W�F�b�g�p�����[�^�̕\��.
	void WidgetParameterDraw();
	// �A�j���[�V�����p�����[�^�̕\��.
	void AnimationParameterDraw();
	// �t�F�[�h�p�����[�^�̕\��.
	void FadeParameterDraw();

	// ImGui�ŃQ�[���p�b�h�����L���ɂ���.
	void OnImGuiGamepad();
	// ImGui�ŃQ�[���p�b�h����𖳌��ɂ���.
	void OffImGuiGamepad();

private:
	std::vector<std::unique_ptr<CMovieWidget>>	m_pWidgetList;
	std::vector<std::string>					m_SpriteNameList;
	std::string									m_NowPushSelectName;
	SMovieWidget								m_NowSelectState;
	int											m_NowSelectIndex;
	bool										m_IsAllWidgetRender;
	bool										m_IsPlay;
	bool										m_IsImGuiGamepad;
};

#endif	// #ifndef WIDGET_EDITOR_H.