#ifndef ACTOR_EDITOR_H
#define ACTOR_EDITOR_H

#include "..\EditorBase.h"
#include <vector>
#include <functional>

class CPlayerEditor;		// プレイヤーエディタ.
class CEnemyEditor;

/****************************************
*	アクターエディタクラス.
**/
class CActorEditor : public CEditorBase
{
	// エディタアクターリスト.
	enum enEditActorList : char
	{
		EEditActorNo_None,

		EEditActorNo_Player = 0,	// プレイヤー.

		EEditActorNo_Max,
	} typedef EEditActorNo;

	enum enEditFuncList : char
	{
		EEditFuncNo_None,

		EEditFuncNo_Update = 0,
		EEditFuncNo_ImGuiRender,
		EEditFuncNo_ModelRender,
		EEditFuncNo_Writing,
		EEditFuncNo_Reading,

		EEditFuncNo_Max,
	} typedef EEditFuncNo;

public:
	CActorEditor();
	virtual ~CActorEditor();

	// 初期化.
	virtual bool Init() override;
	// 更新.
	virtual void Update() override;
	// ImGui描画.
	virtual bool ImGuiRender() override;
	// モデル描画.
	virtual void ModelRender() override;
	// エフェクト描画.
	virtual void EffectRneder() override;
	// ウィジェット描画.
	virtual void WidgetRender() override;

private:
	// アクターの切り替え.
	void ChangeActor( const int& selectNo );

	// 各関数の設定.
	template<class T>
	void SetEditFunction( T* );

	// パラメータの書き込み.
	virtual void ParameterWriting( const char* filePath ) override;
	// パラメータの読み込み.
	virtual void ParameterLoading( const char* filePath ) override;

private:
	std::vector<std::function<bool(void)>>	m_EditFuncList;
	std::unique_ptr<CPlayerEditor>		m_pPlayerEdit;
	int									m_NowSelectActor;
};

#endif	// #ifndef ACTOR_EDITOR_H.