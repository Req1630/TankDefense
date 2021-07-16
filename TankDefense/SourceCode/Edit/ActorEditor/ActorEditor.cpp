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
// ������.
//---------------------------------.
bool CActorEditor::Init()
{
	if( m_pPlayerEdit->Init() == false ) return false;

	return true;
}

//---------------------------------.
// �X�V.
//---------------------------------.
void CActorEditor::Update()
{
	m_pPlayerEdit->Update();
}

//---------------------------------.
// ImGui�`��.
//---------------------------------.
bool CActorEditor::ImGuiRender()
{
	if( BeginTab("ActorEdit") == false ) return false;

	m_pPlayerEdit->ImGuiRender();

	EndTab();
	return true;
}

//---------------------------------.
// ���f���`��.
//---------------------------------.
void CActorEditor::ModelRender()
{
	m_pPlayerEdit->ModelRender();
}

//---------------------------------.
// �G�t�F�N�g�`��.
//---------------------------------.
void CActorEditor::EffectRneder()
{
}

//---------------------------------.
// �E�B�W�F�b�g�`��.
//---------------------------------.
void CActorEditor::WidgetRender()
{
}

//---------------------------------.
// �p�����[�^�̏�������.
//---------------------------------.
void CActorEditor::ParameterWriting( const char* filePath )
{
}

//---------------------------------.
// �p�����[�^�̓ǂݍ���.
//---------------------------------.
void CActorEditor::ParameterLoading( const char* filePath )
{
}
