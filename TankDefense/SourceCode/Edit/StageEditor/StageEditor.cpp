#include "StageEditor.h"

CStageEditor::CStageEditor()
{
}

CStageEditor::~CStageEditor()
{
}

//------------------------------------.
// ������.
//------------------------------------.
bool CStageEditor::Init()
{
	return true;
}

//------------------------------------.
// �X�V.
//------------------------------------.
void CStageEditor::Update()
{
}

//------------------------------------.
// ImGui�`��.
//------------------------------------.
void CStageEditor::ImGuiRender()
{
	if( BeginTab("StageEdit") == false ) return;


	EndTab();
}

//------------------------------------.
// ���f���`��.
//------------------------------------.
void CStageEditor::ModelRender()
{
}

//------------------------------------.
// �G�t�F�N�g�`��.
//------------------------------------.
void CStageEditor::EffectRneder()
{
}

//------------------------------------.
// �p�����[�^�̏�������.
//------------------------------------.
void CStageEditor::ParameterWriting( const char* filePath )
{
}

//------------------------------------.
// �p�����[�^�̓ǂݍ���.
//------------------------------------.
void CStageEditor::ParameterLoading( const char* filePath )
{
}