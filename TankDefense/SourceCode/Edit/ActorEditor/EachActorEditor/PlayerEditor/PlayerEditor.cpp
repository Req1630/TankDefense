#include "PlayerEditor.h"

CPlayerEditor::CPlayerEditor()
{
	m_PrameterNo = EParamNo::Player;
}

CPlayerEditor::~CPlayerEditor()
{
}

//------------------------------------.
// 初期化.
//------------------------------------.
bool CPlayerEditor::Init()
{
	if( ReadingParam() == false ) return false;

	return true;
}

//------------------------------------.
// 更新.
//------------------------------------.
void CPlayerEditor::Update()
{
}

//------------------------------------.
// ImGui描画.
//------------------------------------.
bool CPlayerEditor::ImGuiRender()
{
	return true;
}

//------------------------------------.
// モデル描画.
//------------------------------------.
void CPlayerEditor::ModelRender()
{
}