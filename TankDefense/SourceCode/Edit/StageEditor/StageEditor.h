#ifndef STAGE_EDITOR_H
#define STAGE_EDITOR_H

#include "..\EditorBase.h"
#include "StageStruct.h"
#include <queue>
#include <functional>

class CStageEditPlayer;

class CUndoRedo
{
	struct stPram
	{
		int						VecotPos;
		SActorParam				ActorParam;
		std::function<void()>	Func;
	} typedef SPram;

	void Undo()
	{
		m_UndoQueue.back().Func();
	}
	void Redo()
	{
	}

	void PushUndo( const SPram& param )
	{
		m_UndoQueue.push(param);
	}
	void PushRedo( const SPram& param )
	{
		m_RedoQueue.push(param);
	}

private:
	std::queue<SPram> m_UndoQueue;
	std::queue<SPram> m_RedoQueue;
};

/*****************************************
*	�X�e�[�W�G�f�B�^.
**/
class CStageEditor : public CEditorBase
{
	struct stActorMesh
	{
		EActorNo		ActorNo;
		std::string		MeshName;
		CDX9StaticMesh*	pStaticMesh;
		stActorMesh() : ActorNo ( EActorNo_None ), MeshName ("") , pStaticMesh ( nullptr )
		{}
		stActorMesh( const EActorNo& no, const std::string& name, CDX9StaticMesh* pMesh) 
			: ActorNo ( no ), MeshName ( name ) , pStaticMesh ( pMesh )
		{}

	} typedef SActorMesh;
	using actor_mesh_list = std::vector<SActorMesh>;

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
	// �z�u�����̐؂�ւ�.
	void ChangeArrangement();
	// �A�N�^�[�̍폜����.
	void DeleteActor();
	// �A�N�^�[���b�V���I���̕\��.
	void ActorMeshSelectDraw();
	// �폜����A�N�^�[���b�V���I���̕\��.
	void DelteActorMeshSelectDraw();
	// �A���h�D/���h�D�̕\��.
	void UndoRedoDraw();
	// ��������̕\��.
	void ControllerDraw();
	// �A�N�^�[���b�V�����X�g�̏�����.
	bool InitActorMeshList();

	// �p�����[�^�̏�������.
	virtual void ParameterWriting( const char* filePath ) override;
	// �p�����[�^�̓ǂݍ���.
	virtual void ParameterLoading( const char* filePath ) override;

private:
	std::unique_ptr<CStageEditPlayer>	m_EditPlayer;			// �G�f�B�^�p�v���C���[.
	std::vector<SActorParam>			m_ActorList;			// �ۑ��p�̃A�N�^�[���X�g.
	actor_mesh_list						m_ActorMeshList;		// �A�N�^�[���b�V�����X�g.
	SActorMesh							m_NowSelectActor;		// ���ݑI�����Ă���A�N�^�[.
	int									m_ArrangementCount;		// �A�N�^�[��ǉ�������.
	int									m_DeleteActorNo;		// �폜����A�N�^�[�̔ԍ�.
	bool								m_IsArrangementActive;	// �z�u���삩.
};

#endif	// #ifndef STAGE_EDITOR_H.