#include "PlayerEditor.h"

CPlayerEditor::CPlayerEditor()
{
	m_PrameterNo = EParamNo::Player;
}

CPlayerEditor::~CPlayerEditor()
{
}

//------------------------------------.
// ������.
//------------------------------------.
bool CPlayerEditor::Init()
{
	if( ReadingParam() == false ) return false;

	return true;
}

//------------------------------------.
// �X�V.
//------------------------------------.
void CPlayerEditor::Update()
{
}

//------------------------------------.
// ImGui�`��.
//------------------------------------.
bool CPlayerEditor::ImGuiRender()
{
	return true;
}

//------------------------------------.
// ���f���`��.
//------------------------------------.
void CPlayerEditor::ModelRender()
{
}