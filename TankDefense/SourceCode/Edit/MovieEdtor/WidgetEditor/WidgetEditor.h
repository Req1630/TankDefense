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

private:
	// �E�B�W�F�b�g�̒ǉ��̕\��.
	void PushWidgetDraw();
	// �ҏW����E�B�W�F�b�g�̕\��.
	void SelectEidtParameterDraw();
	// �E�B�W�F�b�g�p�����[�^�̕\��.
	void WidgetParameterDraw();

private:
	std::vector<std::unique_ptr<CMovieWidget>>	m_pWidgetList;
	std::vector<std::string>					m_SpriteNameList;
	std::string									m_NowPushSelectName;
	SMovieWidget								m_NowSelectState;
	int											m_NowSelectIndex;
};

#endif	// #ifndef WIDGET_EDITOR_H.