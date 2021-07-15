#ifndef ACTOR_EDITOR_H
#define ACTOR_EDITOR_H

#include "..\EditorBase.h"

class CPlayerEditor;		// �v���C���[�G�f�B�^.

/****************************************
*	�A�N�^�[�G�f�B�^�N���X.
**/
class CActorEditor : public CEditorBase
{
public:
	CActorEditor();
	virtual ~CActorEditor();

	// ������.
	virtual bool Init() override;
	// �X�V.
	virtual void Update() override;
	// ImGui�`��.
	virtual bool ImGuiRender() override;
	// ���f���`��.
	virtual void ModelRender() override;
	// �G�t�F�N�g�`��.
	virtual void EffectRneder() override;
	// �E�B�W�F�b�g�`��.
	virtual void WidgetRender() override;

private:
	// �p�����[�^�̏�������.
	virtual void ParameterWriting( const char* filePath ) override;
	// �p�����[�^�̓ǂݍ���.
	virtual void ParameterLoading( const char* filePath ) override;

private:
	std::unique_ptr<CPlayerEditor>	m_pPlayerEdit;
};

#endif	// #ifndef ACTOR_EDITOR_H.