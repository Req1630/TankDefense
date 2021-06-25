#include "EditorBase.h"
#include "..\Utility\FileManager\FileManager.h"

namespace
{
	constexpr	char	SAVE_BUTTON_TEXT[]	= u8"保存";	// 保存ボタンのテキスト.
	constexpr	char	LOAD_BUTTON_TEXT[]	= u8"読込";	// 読込ボタンのテキスト.
	constexpr	float	MESSAGE_RENDER_TIME	= 3.0f;		// メッセージ描画時間.
}

//------------------------------------.
// ImGuiタブの開始.
//------------------------------------.
bool CEditorBase::BeginTab( const char* tabName )
{
	return ImGui::BeginTabItem( tabName );
}

//------------------------------------.
// ImGuiタブの終了.
//------------------------------------.
void CEditorBase::EndTab()
{
	ImGui::EndTabItem();
}

//------------------------------------.
// 保存ボタン.
//------------------------------------.
void CEditorBase::SaveButton( const char* filePath )
{
	if( ImGui::Button( SAVE_BUTTON_TEXT )) ParameterWriting( filePath );
}

//------------------------------------.
// 読込ボタン.
//------------------------------------.
void CEditorBase::LoadButton( const char* filePath )
{
	if( ImGui::Button( LOAD_BUTTON_TEXT )) ParameterLoading( filePath );
}

//------------------------------------.
// メッセージの描画.
//------------------------------------.
void CEditorBase::MessageRender()
{
	if( m_MessageText.empty() == true ){
		ImGui::NewLine();
		return;
	}

	ImGui::Text( m_MessageText.c_str() );
	m_MessageRenderCount += GetDeltaTime();

	if( m_MessageRenderCount < MESSAGE_RENDER_TIME ) return;
	m_MessageRenderCount	= FOR_INIT_ZERO_FLOAT;
	m_MessageText			= "";
}

//------------------------------------.
// パラメータの書き込み.
//------------------------------------.
bool CEditorBase::SetParameterWritingMsg( const bool& isSucceeded )
{
	if( isSucceeded == true ){
		m_MessageText = u8"書き込みに成功しました。";
	} else {
		m_MessageText = u8"書き込みに失敗しました。";
	}
	return isSucceeded;
}

//------------------------------------.
// パラメータの読み込み.
//------------------------------------.
bool CEditorBase::SetParameterLoadingMsg( const bool& isSucceeded )
{
	if( isSucceeded == true ){
		m_MessageText = u8"読み込みに成功しました。";
	} else {
		m_MessageText = u8"読み込みに失敗しました。";
	}
	return isSucceeded;
}

