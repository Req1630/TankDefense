#ifndef STAGE_RENDER_H
#define STAGE_RENDER_H

#include "..\StageEditor.h"
#include "..\..\..\Object\GameObject\Actor\Stage\StageLoader\StageLoader.h"

/************************************
*	�X�e�[�W�̕`��N���X.
**/
class CStageRender
{
public:
	CStageRender();
	~CStageRender();

	// ������.
	bool Init();
	// �`��.
	void Render();

	// �X�e�[�W�̐ݒ�.
	void SetStage( const EStageNo& no );

private:
	std::unique_ptr<CStageLoader>	m_pStageLoader;		// �X�e�[�W�̓ǂݍ��݃N���X.
	actor_mesh_list					m_ActorMeshList;	// �A�N�^�[���b�V�����X�g.
	std::vector<SActorParam>		m_ActorList;		// �ۑ��p�̃A�N�^�[���X�g.
};

#endif	// #ifndef STAGE_RENDER_H.