#include "EnemyEditor.h"

CEnemyEditor::CEnemyEditor()
{
	m_PrameterNo = EParamNo::Player;
}

CEnemyEditor::~CEnemyEditor()
{
}

//------------------------------------.
// ������.
//------------------------------------.
bool CEnemyEditor::Init()
{
	if( ReadingParam() == false ) return false;

	return true;
}

//------------------------------------.
// �X�V.
//------------------------------------.
void CEnemyEditor::Update()
{
}

//------------------------------------.
// ImGui�`��.
//------------------------------------.
bool CEnemyEditor::ImGuiRender()
{
	ImGui::Text("test");

	return true;
}

//------------------------------------.
// ���f���`��.
//------------------------------------.
void CEnemyEditor::ModelRender()
{
}