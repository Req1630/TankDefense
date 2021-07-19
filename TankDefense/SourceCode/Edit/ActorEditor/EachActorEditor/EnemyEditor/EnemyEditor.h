#ifndef ENEMY_EDITOR_H
#define ENEMY_EDITOR_H

#include "..\EachActorEditorBase.h"
#include "..\..\..\..\Object\GameObject\Actor\Player\PlayerParameter.h"

/************************************
*	�G�l�~�[�G�f�B�^.
**/
class CEnemyEditor : public CEachActorEditorBase<SPlayerParam>
{
public:
	CEnemyEditor();
	virtual ~CEnemyEditor();

	// ������.
	virtual bool Init() override;
	// �X�V.
	virtual void Update() override;
	// ImGui�`��.
	virtual bool ImGuiRender() override;
	// ���f���`��.
	virtual void ModelRender() override;
};

#endif	// #ifndef ENEMY_EDITOR_H.