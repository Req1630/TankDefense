#ifndef EDIT_H
#define EDIT_H

#include "..\..\SceneBase\SceneBase.h"
#include "..\..\..\Utility\ImGuiManager\ImGuiManager.h"

class CStageEditor;

/**********************************
*	�G�f�B�^�V�[���N���X.
*/
class CEdit : public CSceneBase
{
private:

public:
	CEdit( CSceneManager* pSceneManager );
	virtual ~CEdit();

	// �Ǎ��֐�.
	virtual bool Load() override;
	// �X�V�֐�.
	virtual void Update() override;
	// ���f���`��֐�.
	virtual void ModelRender() override;
	// �摜�`��֐�.
	virtual void SpriteRender() override;

private:

private:
	std::unique_ptr<CStageEditor>	m_StageEditor;
	ImGuiWindowFlags				m_WindowFlags;
};

#endif	// #ifndef EDIT_H.