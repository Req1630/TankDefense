#ifndef EDIT_H
#define EDIT_H

#include "..\..\SceneBase\SceneBase.h"
#include "..\..\..\Utility\ImGuiManager\ImGuiManager.h"

class CEditorBase;
class CStageEditor;
class CEnemySpawnRangeEditor;

/**********************************
*	エディタシーンクラス.
*/
class CEdit : public CSceneBase
{
private:

public:
	CEdit( CSceneManager* pSceneManager );
	virtual ~CEdit();

	// 読込関数.
	virtual bool Load() override;
	// 更新関数.
	virtual void Update() override;
	// モデル描画関数.
	virtual void ModelRender() override;
	// 画像描画関数.
	virtual void SpriteRender() override;

private:

private:
	std::vector<std::unique_ptr<CEditorBase>>	m_Editors;
	CEditorBase*								m_pActiveEditor;
	ImGuiWindowFlags							m_WindowFlags;
};

#endif	// #ifndef EDIT_H.