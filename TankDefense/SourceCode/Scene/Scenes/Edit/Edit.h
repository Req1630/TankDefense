#ifndef EDIT_H
#define EDIT_H

#include "..\..\SceneBase\SceneBase.h"
#include "..\..\..\Utility\ImGuiManager\ImGuiManager.h"

class CStageEditor;

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
	std::unique_ptr<CStageEditor>	m_StageEditor;
	ImGuiWindowFlags				m_WindowFlags;
};

#endif	// #ifndef EDIT_H.