#include "PlayerEditor.h"
#include "PlayerParam.h"

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
	if( ReadingParam()									== false ) return false;
	if( ReadingDragParameter(PLAYER_PARAM_FILE_PATH)	== false ) return false;

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
	EditParameterDraw( EPlayerParamNo_Max );
	return true;
}

//------------------------------------.
// ���f���`��.
//------------------------------------.
void CPlayerEditor::ModelRender()
{
}