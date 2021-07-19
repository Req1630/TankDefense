#ifndef ACTOR_EDITOR_H
#define ACTOR_EDITOR_H

#include "..\EditorBase.h"
#include <vector>
#include <functional>

class CPlayerEditor;		// �v���C���[�G�f�B�^.
class CEnemyEditor;

/****************************************
*	�A�N�^�[�G�f�B�^�N���X.
**/
class CActorEditor : public CEditorBase
{
	// �G�f�B�^�A�N�^�[���X�g.
	enum enEditActorList : char
	{
		EEditActorNo_None,

		EEditActorNo_Player = 0,	// �v���C���[.

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
	// �A�N�^�[�̐؂�ւ�.
	void ChangeActor( const int& selectNo );

	// �e�֐��̐ݒ�.
	template<class T>
	void SetEditFunction( T* );

	// �p�����[�^�̏�������.
	virtual void ParameterWriting( const char* filePath ) override;
	// �p�����[�^�̓ǂݍ���.
	virtual void ParameterLoading( const char* filePath ) override;

private:
	std::vector<std::function<bool(void)>>	m_EditFuncList;
	std::unique_ptr<CPlayerEditor>		m_pPlayerEdit;
	int									m_NowSelectActor;
};

#endif	// #ifndef ACTOR_EDITOR_H.