#ifndef ENEMY_EDITOR_H
#define ENEMY_EDITOR_H

#include "..\EachActorEditorBase.h"
#include "..\..\..\..\Object\GameObject\Actor\Player\PlayerParameter.h"

/************************************
*	エネミーエディタ.
**/
class CEnemyEditor : public CEachActorEditorBase<SPlayerParam>
{
public:
	CEnemyEditor();
	virtual ~CEnemyEditor();

	// 初期化.
	virtual bool Init() override;
	// 更新.
	virtual void Update() override;
	// ImGui描画.
	virtual bool ImGuiRender() override;
	// モデル描画.
	virtual void ModelRender() override;
};

#endif	// #ifndef ENEMY_EDITOR_H.