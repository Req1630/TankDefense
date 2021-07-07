#ifndef WIDGET_EDITOR_H
#define WIDGET_EDITOR_H

#include "..\..\..\Object\GameObject\Widget\MovieWidget\MovieWidget.h"

/**************************************
*	画像エディタクラス.
**/
class CWidgetEditor
{
public:
	CWidgetEditor();
	~CWidgetEditor();

	// 初期化.
	bool Init();
	// 更新.
	void Update();
	// ImGui描画.
	bool ImGuiRender();
	// 画像の描画.
	void SpriteRender();

private:
	// ウィジェットの追加の表示.
	void PushWidgetDraw();
	// 編集するウィジェットの表示.
	void SelectEidtParameterDraw();
	// ウィジェットパラメータの表示.
	void WidgetParameterDraw();

private:
	std::vector<std::unique_ptr<CMovieWidget>>	m_pWidgetList;
	std::vector<std::string>					m_SpriteNameList;
	std::string									m_NowPushSelectName;
	SMovieWidget								m_NowSelectState;
	int											m_NowSelectIndex;
};

#endif	// #ifndef WIDGET_EDITOR_H.