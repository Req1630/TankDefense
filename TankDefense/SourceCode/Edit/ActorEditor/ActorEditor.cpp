#include "ActorEditor.h"

CActorEditor::CActorEditor()
{
}

CActorEditor::~CActorEditor()
{
}

//---------------------------------.
// 初期化.
//---------------------------------.
bool CActorEditor::Init()
{
	return true;
}

//---------------------------------.
// 更新.
//---------------------------------.
void CActorEditor::Update()
{
}

//---------------------------------.
// ImGui描画.
//---------------------------------.
bool CActorEditor::ImGuiRender()
{
	return true;
}

//---------------------------------.
// モデル描画.
//---------------------------------.
void CActorEditor::ModelRender()
{
}

//---------------------------------.
// エフェクト描画.
//---------------------------------.
void CActorEditor::EffectRneder()
{
}

//---------------------------------.
// ウィジェット描画.
//---------------------------------.
void CActorEditor::WidgetRender()
{
}

//---------------------------------.
// パラメータの書き込み.
//---------------------------------.
void CActorEditor::ParameterWriting( const char* filePath )
{
}

//---------------------------------.
// パラメータの読み込み.
//---------------------------------.
void CActorEditor::ParameterLoading( const char* filePath )
{
}
