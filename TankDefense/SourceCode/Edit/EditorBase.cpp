#include "EditorBase.h"
#include "..\Utility\FileManager\FileManager.h"

namespace
{
	constexpr	char	SAVE_BUTTON_TEXT[]	= u8"�ۑ�";	// �ۑ��{�^���̃e�L�X�g.
	constexpr	char	LOAD_BUTTON_TEXT[]	= u8"�Ǎ�";	// �Ǎ��{�^���̃e�L�X�g.
	constexpr	float	MESSAGE_RENDER_TIME	= 3.0f;		// ���b�Z�[�W�`�掞��.
}

//------------------------------------.
// ImGui�^�u�̊J�n.
//------------------------------------.
bool CEditorBase::BeginTab( const char* tabName )
{
	return ImGui::BeginTabItem( tabName );
}

//------------------------------------.
// ImGui�^�u�̏I��.
//------------------------------------.
void CEditorBase::EndTab()
{
	ImGui::EndTabItem();
}

//------------------------------------.
// �ۑ��{�^��.
//------------------------------------.
void CEditorBase::SaveButton( const char* filePath )
{
	if( ImGui::Button( SAVE_BUTTON_TEXT )) ParameterWriting( filePath );
}

//------------------------------------.
// �Ǎ��{�^��.
//------------------------------------.
void CEditorBase::LoadButton( const char* filePath )
{
	if( ImGui::Button( LOAD_BUTTON_TEXT )) ParameterLoading( filePath );
}

//------------------------------------.
// ���b�Z�[�W�̕`��.
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
// �p�����[�^�̏�������.
//------------------------------------.
bool CEditorBase::SetParameterWritingMsg( const bool& isSucceeded )
{
	if( isSucceeded == true ){
		m_MessageText = u8"�������݂ɐ������܂����B";
	} else {
		m_MessageText = u8"�������݂Ɏ��s���܂����B";
	}
	return isSucceeded;
}

//------------------------------------.
// �p�����[�^�̓ǂݍ���.
//------------------------------------.
bool CEditorBase::SetParameterLoadingMsg( const bool& isSucceeded )
{
	if( isSucceeded == true ){
		m_MessageText = u8"�ǂݍ��݂ɐ������܂����B";
	} else {
		m_MessageText = u8"�ǂݍ��݂Ɏ��s���܂����B";
	}
	return isSucceeded;
}

