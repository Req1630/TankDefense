#include "EnemyEditor.h"

CEnemyEditor::CEnemyEditor()
{
	m_PrameterNo = EParamNo::Player;
}

CEnemyEditor::~CEnemyEditor()
{
}

//------------------------------------.
// 初期化.
//------------------------------------.
bool CEnemyEditor::Init()
{
	if( ReadingParam() == false ) return false;

	return true;
}

//------------------------------------.
// 更新.
//------------------------------------.
void CEnemyEditor::Update()
{
}

//------------------------------------.
// ImGui描画.
//------------------------------------.
bool CEnemyEditor::ImGuiRender()
{
	ImGui::Text("test");

	return true;
}

//------------------------------------.
// モデル描画.
//------------------------------------.
void CEnemyEditor::ModelRender()
{
}