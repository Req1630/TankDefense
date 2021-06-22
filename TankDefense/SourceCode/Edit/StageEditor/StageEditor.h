#ifndef STAGE_EDITOR_H
#define STAGE_EDITOR_H

#include "..\EditorBase.h"

enum enActorList
{
	EActorNo_None,

	EActorNo_Player,

	EActorNo_Max,
} typedef EActorNo;

struct stActorParam
{
	EActorNo	m_ActorNo;
	STranceform	m_Tranceform;
} typedef SActorParam;

/*****************************************
*	ステージエディタ.
**/
class CStageEditor : public CEditorBase
{
public:
	CStageEditor();
	virtual ~CStageEditor();

	// 初期化.
	virtual bool Init() override;
	// 更新.
	virtual void Update() override;
	// ImGui描画.
	virtual void ImGuiRender() override;
	// モデル描画.
	virtual void ModelRender() override;
	// エフェクト描画.
	virtual void EffectRneder() override;

private:
	// パラメータの書き込み.
	virtual void ParameterWriting( const char* filePath ) override;
	// パラメータの読み込み.
	virtual void ParameterLoading( const char* filePath ) override;

private:
	std::vector<SActorParam>	m_ActorList;
};

#endif	// #ifndef STAGE_EDITOR_H.