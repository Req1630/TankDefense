#ifndef PLAYRE_EDITOR_H
#define PLAYRE_EDITOR_H

#include "..\EachActorEditorBase.h"
#include "..\..\..\..\Object\GameObject\Actor\Player\PlayerParameter.h"

/************************************
*	プレイヤーエディタ.
**/
class CPlayerEditor : public CEachActorEditorBase<SPlayerParam>
{
public:
	CPlayerEditor();
	virtual ~CPlayerEditor();

	// 初期化.
	virtual bool Init() override;
	// 更新.
	virtual void Update() override;
	// ImGui描画.
	virtual bool ImGuiRender() override;
	// モデル描画.
	virtual void ModelRender() override;
};

#endif	// #ifndef PLAYRE_EDITOR_H.