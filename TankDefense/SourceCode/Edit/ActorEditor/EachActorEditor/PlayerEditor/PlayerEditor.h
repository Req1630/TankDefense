#ifndef PLAYRE_EDITOR_H
#define PLAYRE_EDITOR_H

#include "..\EachActorEditorBase.h"
#include "..\..\..\..\Object\GameObject\Actor\Player\PlayerParameter.h"

/************************************
*	�v���C���[�G�f�B�^.
**/
class CPlayerEditor : public CEachActorEditorBase<SPlayerParam>
{
public:
	CPlayerEditor();
	virtual ~CPlayerEditor();

	// ������.
	virtual bool Init() override;
	// �X�V.
	virtual void Update() override;
	// ImGui�`��.
	virtual bool ImGuiRender() override;
	// ���f���`��.
	virtual void ModelRender() override;
};

#endif	// #ifndef PLAYRE_EDITOR_H.