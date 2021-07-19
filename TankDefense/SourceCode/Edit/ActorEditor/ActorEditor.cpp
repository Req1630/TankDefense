#include "ActorEditor.h"
#include "EachActorEditor/PlayerEditor/PlayerEditor.h"

namespace
{
	// �G�f�B�^�A�N�^�[�����X�g.
	const char* EDIT_ACTOR_NAME_LIST[] =
	{
		u8"Player",
	};
};

CActorEditor::CActorEditor()
	: m_EditFuncList	( EEditFuncNo_Max )
	, m_pPlayerEdit		( std::make_unique<CPlayerEditor>() )
	, m_NowSelectActor	( EEditActorNo_None )
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

	ChangeActor( m_NowSelectActor );

	return true;
}

//---------------------------------.
// �X�V.
//---------------------------------.
void CActorEditor::Update()
{
	m_EditFuncList[EEditFuncNo_Update]();
}

//---------------------------------.
// ImGui�`��.
//---------------------------------.
bool CActorEditor::ImGuiRender()
{
	if( BeginTab("ActorEdit") == false ) return false;

	ImGui::Text( u8"�ҏW�������A�N�^�[��I��" );
	if( ImGui::BeginCombo( "##1", EDIT_ACTOR_NAME_LIST[m_NowSelectActor] ) ){

		for( int i = 0; i < EEditActorNo_Max; i++ ){
			const bool isSelected = ( i == m_NowSelectActor );
			if( ImGui::Selectable( EDIT_ACTOR_NAME_LIST[i], isSelected ) ) ChangeActor( i );

			if( isSelected ) ImGui::SetItemDefaultFocus();
		}

		ImGui::EndCombo();
	}
	SaveButton( "" ); 	ImGui::SameLine();
	LoadButton( "" ); 	ImGui::SameLine();
	MessageRender();

	ImGui::Separator();

	m_EditFuncList[EEditFuncNo_ImGuiRender]();

	EndTab();

	return true;
}

//---------------------------------.
// ���f���`��.
//---------------------------------.
void CActorEditor::ModelRender()
{
	m_EditFuncList[EEditFuncNo_ModelRender]();
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
// �A�N�^�[�̐؂�ւ�.
//---------------------------------.
void CActorEditor::ChangeActor( const int& selectNo )
{
	m_NowSelectActor = selectNo;

	switch( m_NowSelectActor )
	{
	case EEditActorNo_Player:
		SetEditFunction( m_pPlayerEdit.get() );
		break;
	default:
		break;
	}
}

//---------------------------------.
// �e�֐��̐ݒ�.
//---------------------------------.
template<class T>
void CActorEditor::SetEditFunction( T* t )
{
	this->m_EditFuncList[EEditFuncNo_Update]		= [t](){		t->Update();		return true;	};
	this->m_EditFuncList[EEditFuncNo_ImGuiRender]	= [t](){ return	t->ImGuiRender();					};
	this->m_EditFuncList[EEditFuncNo_ModelRender]	= [t](){		t->ModelRender();	return true;	};
	this->m_EditFuncList[EEditFuncNo_Writing]		= [t](){ return	t->WritingParam();					};
	this->m_EditFuncList[EEditFuncNo_Reading]		= [t](){ return	t->ReadingParam();					};
}

//---------------------------------.
// �p�����[�^�̏�������.
//---------------------------------.
void CActorEditor::ParameterWriting( const char* filePath )
{
	SetParameterWritingMsg( m_EditFuncList[EEditFuncNo_Writing]() );
}

//---------------------------------.
// �p�����[�^�̓ǂݍ���.
//---------------------------------.
void CActorEditor::ParameterLoading( const char* filePath )
{
	SetParameterLoadingMsg( m_EditFuncList[EEditFuncNo_Reading]() );
}
