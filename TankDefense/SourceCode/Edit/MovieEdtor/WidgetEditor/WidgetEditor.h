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

	// ウィジェット情報の取得.
	std::vector<SMovieWidget> GetWidgetStateList();
	// ウィジェット情報の取得.
	void SetWidgetStateList( const std::vector<SMovieWidget>& stateList );

private:
	// 操作関数.
	void WidgetControll();
	// ウィジェットの追加の表示.
	void PushWidgetDraw();
	// 編集するウィジェットの表示.
	void SelectEidtParameterDraw();
	// ウィジェットパラメータの表示.
	void WidgetParameterDraw();
	// アニメーションパラメータの表示.
	void AnimationParameterDraw();
	// フェードパラメータの表示.
	void FadeParameterDraw();

	// ImGuiでゲームパッド操作を有効にする.
	void OnImGuiGamepad();
	// ImGuiでゲームパッド操作を無効にする.
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