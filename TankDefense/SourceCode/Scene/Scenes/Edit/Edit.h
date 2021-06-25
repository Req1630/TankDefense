#ifndef EDIT_H
#define EDIT_H

#include "..\..\SceneBase\SceneBase.h"
#include "..\..\..\Utility\ImGuiManager\ImGuiManager.h"

class CEditorBase;
class CStageEditor;
class CEnemySpawnRangeEditor;

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
	std::vector<std::unique_ptr<CEditorBase>>	m_Editors;
	CEditorBase*								m_pActiveEditor;
	ImGuiWindowFlags							m_WindowFlags;
};

#endif	// #ifndef EDIT_H.