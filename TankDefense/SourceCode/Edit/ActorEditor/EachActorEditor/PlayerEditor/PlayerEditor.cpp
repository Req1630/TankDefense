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
// èâä˙âª.
//------------------------------------.
bool CPlayerEditor::Init()
{
	if( ReadingParam()									== false ) return false;
	if( ReadingDragParameter(PLAYER_PARAM_FILE_PATH)	== false ) return false;

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
	EditParameterDraw( EPlayerParamNo_Max );
	return true;
}

//------------------------------------.
// ÉÇÉfÉãï`âÊ.
//------------------------------------.
void CPlayerEditor::ModelRender()
{
}