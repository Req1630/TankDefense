#ifndef ACTOR_EDITOR_H
#define ACTOR_EDITOR_H

#include "..\EditorBase.h"

class CPlayerEditor;		// プレイヤーエディタ.

/****************************************
*	アクターエディタクラス.
**/
class CActorEditor : public CEditorBase
{
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
	// パラメータの書き込み.
	virtual void ParameterWriting( const char* filePath ) override;
	// パラメータの読み込み.
	virtual void ParameterLoading( const char* filePath ) override;

private:
	std::unique_ptr<CPlayerEditor>	m_pPlayerEdit;
};

#endif	// #ifndef ACTOR_EDITOR_H.