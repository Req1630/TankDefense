#include "PlayerEditor.h"

CPlayerEditor::CPlayerEditor()
{
	m_PrameterNo = EParamNo::Player;
}

CPlayerEditor::~CPlayerEditor()
{
}

//------------------------------------.
// èâä˙âª.
//------------------------------------.
bool CPlayerEditor::Init()
{
	if( ReadingParam() == false ) return false;

	return true;
}

//------------------------------------.
// çXêV.
//------------------------------------.
void CPlayerEditor::Update()
{
}

//------------------------------------.
// ImGuiï`âÊ.
//------------------------------------.
bool CPlayerEditor::ImGuiRender()
{
	return true;
}

//------------------------------------.
// ÉÇÉfÉãï`âÊ.
//------------------------------------.
void CPlayerEditor::ModelRender()
{
}