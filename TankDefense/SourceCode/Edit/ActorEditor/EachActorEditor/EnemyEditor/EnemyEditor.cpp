#include "EnemyEditor.h"

CEnemyEditor::CEnemyEditor()
{
	m_PrameterNo = EParamNo::Player;
}

CEnemyEditor::~CEnemyEditor()
{
}

//------------------------------------.
// ‰Šú‰».
//------------------------------------.
bool CEnemyEditor::Init()
{
	if( ReadingParam() == false ) return false;

	return true;
}

//------------------------------------.
// XV.
//------------------------------------.
void CEnemyEditor::Update()
{
}

//------------------------------------.
// ImGui•`‰æ.
//------------------------------------.
bool CEnemyEditor::ImGuiRender()
{
	ImGui::Text("test");

	return true;
}

//------------------------------------.
// ƒ‚ƒfƒ‹•`‰æ.
//------------------------------------.
void CEnemyEditor::ModelRender()
{
}