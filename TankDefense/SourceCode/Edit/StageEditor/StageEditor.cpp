#include "StageEditor.h"

CStageEditor::CStageEditor()
{
}

CStageEditor::~CStageEditor()
{
}

//------------------------------------.
// 初期化.
//------------------------------------.
bool CStageEditor::Init()
{
	return true;
}

//------------------------------------.
// 更新.
//------------------------------------.
void CStageEditor::Update()
{
}

//------------------------------------.
// ImGui描画.
//------------------------------------.
void CStageEditor::ImGuiRender()
{
	if( BeginTab("StageEdit") == false ) return;


	EndTab();
}

//------------------------------------.
// モデル描画.
//------------------------------------.
void CStageEditor::ModelRender()
{
}

//------------------------------------.
// エフェクト描画.
//------------------------------------.
void CStageEditor::EffectRneder()
{
}

//------------------------------------.
// パラメータの書き込み.
//------------------------------------.
void CStageEditor::ParameterWriting( const char* filePath )
{
}

//------------------------------------.
// パラメータの読み込み.
//------------------------------------.
void CStageEditor::ParameterLoading( const char* filePath )
{
}