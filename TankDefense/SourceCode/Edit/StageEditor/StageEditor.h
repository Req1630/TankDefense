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
*	�X�e�[�W�G�f�B�^.
**/
class CStageEditor : public CEditorBase
{
public:
	CStageEditor();
	virtual ~CStageEditor();

	// ������.
	virtual bool Init() override;
	// �X�V.
	virtual void Update() override;
	// ImGui�`��.
	virtual void ImGuiRender() override;
	// ���f���`��.
	virtual void ModelRender() override;
	// �G�t�F�N�g�`��.
	virtual void EffectRneder() override;

private:
	// �p�����[�^�̏�������.
	virtual void ParameterWriting( const char* filePath ) override;
	// �p�����[�^�̓ǂݍ���.
	virtual void ParameterLoading( const char* filePath ) override;

private:
	std::vector<SActorParam>	m_ActorList;
};

#endif	// #ifndef STAGE_EDITOR_H.