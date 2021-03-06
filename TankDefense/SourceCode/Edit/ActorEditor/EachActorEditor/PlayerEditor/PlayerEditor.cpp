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
// 初期化.
//------------------------------------.
bool CPlayerEditor::Init()
{
	if( ReadingParam()									== false ) return false;
	if( ReadingDragParameter(PLAYER_PARAM_FILE_PATH)	== false ) return false;

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

	EditParameterDraw( EPlayerParamNo_Max );

	return true;
}

//------------------------------------.
// モデル描画.
//------------------------------------.
void CPlayerEditor::ModelRender()
{
}