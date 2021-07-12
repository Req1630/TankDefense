#ifndef STAGE_EDITOR_H
#define STAGE_EDITOR_H

#include "..\EditorBase.h"
#include "..\..\Object\GameObject\Actor\Stage\StageStruct.h"
#include "..\UndoRedo\UndoRedo.h"

class CStageLoader;

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
	virtual bool ImGuiRender() override;
	// ���f���`��.
	virtual void ModelRender() override;
	// �G�t�F�N�g�`��.
	virtual void EffectRneder() override;
	// �E�B�W�F�b�g�`��.
	virtual void WidgetRender() override;

	// �G�f�B�^�p�v���C���[�̐ݒ�.
	virtual void SetEditPlayer( CEditPlayer* pPlayer ) override { m_pEditPlayer = pPlayer; }

private:
	// �X�e�[�W�I��.
	void StageSelect();
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
	CEditPlayer*							m_pEditPlayer;			// �G�f�B�^�p�v���C���[.
	std::unique_ptr<CStageLoader>			m_pStageLoader;			// �X�e�[�W�ǂݍ��݃N���X.
	std::unique_ptr<CUndoRedo<SActorParam>>	m_pUndoRedo;			// ���ɖ߂�����N���X.
	std::vector<SActorParam>				m_ActorList;			// �ۑ��p�̃A�N�^�[���X�g.
	actor_mesh_list							m_ActorMeshList;		// �A�N�^�[���b�V�����X�g.
	SActorMesh								m_NowSelectActor;		// ���ݑI�����Ă���A�N�^�[.
	EStageNo								m_NowStageNo;			// ���݂̃X�e�[�W�ԍ�.
	int										m_DeleteActorNo;		// �폜����A�N�^�[�̔ԍ�.
};

#endif	// #ifndef STAGE_EDITOR_H.