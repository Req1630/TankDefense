#ifndef STAGE_EDITOR_H
#define STAGE_EDITOR_H

#include "..\EditorBase.h"

enum enActorList
{
	EActorNo_None,

	EActorNo_Player = 0,
	EActorNo_Ground,
	EActorNo_Ground_I,
	EActorNo_Ground_L,
	EActorNo_Tree,
	EActorNo_ItemSpeed,

	EActorNo_Max,

	EActorNo_Begin	= EActorNo_Player,
	EActorNo_End	= EActorNo_Max,
} typedef EActorNo;

struct stActorParam
{
	EActorNo	ActorNo;
	STranceform	Tranceform;

	stActorParam() : ActorNo(), Tranceform() 
	{}
	stActorParam( const EActorNo& no, const STranceform& t ) 
		: ActorNo( no ), Tranceform( t )
	{}
} typedef SActorParam;

class CStageEditPlayer;

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
	int									m_DeleteActorNo;		// �폜����A�N�^�[�̔ԍ�.
	bool								m_IsArrangementActive;	// �z�u���삩.
};

#endif	// #ifndef STAGE_EDITOR_H.