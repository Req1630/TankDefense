#ifndef STAGE_RENDER_H
#define STAGE_RENDER_H

#include "..\StageEditor.h"

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

private:
	actor_mesh_list				m_ActorMeshList;	// �A�N�^�[���b�V�����X�g.
	std::vector<SActorParam>	m_ActorList;		// �ۑ��p�̃A�N�^�[���X�g.
};

#endif	// #ifndef STAGE_RENDER_H.