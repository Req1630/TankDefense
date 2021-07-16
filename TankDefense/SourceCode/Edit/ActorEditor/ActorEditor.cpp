#include "ActorEditor.h"
#include "EachActorEditor/PlayerEditor/PlayerEditor.h"

CActorEditor::CActorEditor()
	: m_pPlayerEdit	( std::make_unique<CPlayerEditor>() )
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
	if( m_pPlayerEdit->Init() == false ) return false;

	return true;
}

//---------------------------------.
// 更新.
//---------------------------------.
void CActorEditor::Update()
{
	m_pPlayerEdit->Update();
}

//---------------------------------.
// ImGui描画.
//---------------------------------.
bool CActorEditor::ImGuiRender()
{
	if( BeginTab("ActorEdit") == false ) return false;

	m_pPlayerEdit->ImGuiRender();

	EndTab();
	return true;
}

//---------------------------------.
// モデル描画.
//---------------------------------.
void CActorEditor::ModelRender()
{
	m_pPlayerEdit->ModelRender();
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
