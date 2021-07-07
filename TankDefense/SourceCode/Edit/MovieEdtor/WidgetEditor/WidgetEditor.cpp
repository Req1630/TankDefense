#include "WidgetEditor.h"
#include "..\..\..\Utility\ImGuiManager\ImGuiManager.h"
#include "..\..\..\Resource\SpriteResource\SpriteResource.h"

CWidgetEditor::CWidgetEditor()
	: m_pWidgetList			()
	, m_SpriteNameList		()
	, m_NowPushSelectName	()
	, m_NowSelectIndex		( -1 )
{
}

CWidgetEditor::~CWidgetEditor()
{
}

//----------------------------------.
// ������.
//----------------------------------.
bool CWidgetEditor::Init()
{
	m_SpriteNameList = CSpriteResource::GetSpriteNameList();
	m_NowPushSelectName = m_SpriteNameList.front();
	return true;
}

//----------------------------------.
// �X�V.
//----------------------------------.
void CWidgetEditor::Update()
{
	if( m_NowSelectIndex < 0 ) return;
	
}

//----------------------------------.
// ImGui�`��.
//----------------------------------.
bool CWidgetEditor::ImGuiRender()
{
	PushWidgetDraw();
	SelectEidtParameterDraw();

	ImGui::Separator();
	WidgetParameterDraw();

	return true; 
}

//----------------------------------.
// �摜�̕`��.
//----------------------------------.
void CWidgetEditor::SpriteRender()
{
	for( auto& w : m_pWidgetList ) w->SimpleRender();
}

//----------------------------------.
// �E�B�W�F�b�g�̒ǉ��̕\��.
//----------------------------------.
void CWidgetEditor::PushWidgetDraw()
{
	ImGui::Text( u8"�ǉ��������摜��I��" );
	if( ImGui::BeginCombo( "##1", m_NowPushSelectName.c_str() ) ){

		for( auto& n : m_SpriteNameList ){
			const bool isSelected = ( n == m_NowPushSelectName );
			if( ImGui::Selectable( n.c_str(), isSelected ) )
				m_NowPushSelectName = n;

			if( isSelected ) ImGui::SetItemDefaultFocus();
		}

		ImGui::EndCombo();
	}
	ImGui::SameLine();
	if( ImGui::Button( u8"�ǉ�" ) ){
		m_pWidgetList.emplace_back( std::make_unique<CMovieWidget>() );
		SMovieWidget state;
		state.SpriteName = m_NowPushSelectName;
		m_pWidgetList.back()->SetMovieWidgetState( state );
		m_pWidgetList.back()->Init();

		m_NowSelectState = m_pWidgetList.back()->GetMovieWidgetState();

		m_NowSelectIndex = m_pWidgetList.size()-1;
	}
}

//----------------------------------.
// �ҏW����E�B�W�F�b�g�̕\��.
//----------------------------------.
void CWidgetEditor::SelectEidtParameterDraw()
{
	ImGui::Text( u8"�ҏW�������摜��I��" );

	std::string name = "None";
	if( m_NowSelectIndex >= 0 ){
		name = std::to_string(m_NowSelectIndex+1) + "_" + m_NowPushSelectName;
	}

	int i = 0;
	if( ImGui::BeginCombo( "##2", name.c_str() ) ){
		for( auto& w : m_pWidgetList ){
			const SMovieWidget state = w->GetMovieWidgetState();
			const std::string n = std::to_string(i+1) + "_" + state.SpriteName;
			const bool isSelected = ( n == m_NowPushSelectName );
			if( ImGui::Selectable( n.c_str(), isSelected ) ){
				m_NowPushSelectName = state.SpriteName;
				m_NowSelectIndex = i;
				m_NowSelectState = state;
			}
			if( isSelected ) ImGui::SetItemDefaultFocus();
			i++;
		}

		ImGui::EndCombo();
	}
}

//----------------------------------.
// �E�B�W�F�b�g�p�����[�^�̕\��.
//----------------------------------.
void CWidgetEditor::WidgetParameterDraw()
{
	if( m_NowSelectIndex < 0 ) return;

	ImGui::Text( u8"�摜�� : %s", m_NowSelectState.SpriteName.c_str() );

	if( ImGui::TreeNode( u8"�p�����[�^��ҏW����" ) ){

		ImGui::PushItemWidth( 160.0f );

		ImGui::DragFloat( u8"����J�n����", &m_NowSelectState.AciveStartTime, 0.1f, 0.0f, 180.0f );

		ImGui::Text( u8" Position" );
		ImGui::Indent();
		ImGui::DragFloat( u8"X", &m_NowSelectState.Position.x, 0.5f, -1280.00f, 1280.0f );
		ImGui::DragFloat( u8"Y", &m_NowSelectState.Position.y, 0.5f, -1280.00f, 1280.0f );
		ImGui::Unindent();

		SMovieAnimState& animState = m_NowSelectState.AnimState;

		ImGui::Checkbox( u8"��������UV�X�N���[�������邩", &animState.IsUVScrollX );
		if( animState.IsUVScrollX  == true ){
			ImGui::Indent();
			ImGui::DragFloat( u8"�X�N���[�����x##1", &animState.UVScrollSpeed.x, 0.0001f, -20.0f, 20.0f, "%.4f" );
			ImGui::Unindent();
		}

		ImGui::Checkbox( u8"�c������UV�X�N���[�������邩", &animState.IsUVScrollY );
		if( animState.IsUVScrollY  == true ){
			ImGui::Indent();
			ImGui::DragFloat( u8"�X�N���[�����x##2", &animState.UVScrollSpeed.y, 0.0001f, -20.0f, 20.0f, "%.4f" );
			ImGui::Unindent();
		}

		ImGui::Checkbox( u8"�X�v���C�g�A�j���[�V���������邩", &animState.IsAnimation );
		if( animState.IsAnimation == true ){
			ImGui::Indent();
			ImGui::DragInt( u8"�A�j���[�V�������x", &animState.FrameCountSpeed, 1.0f, 0, 20 );
			ImGui::Unindent();
		}
		ImGui::PopItemWidth();

		ImGui::TreePop();
	}

	m_pWidgetList[m_NowSelectIndex]->SetMovieWidgetState( m_NowSelectState );
	m_pWidgetList[m_NowSelectIndex]->SettingRenderState();
}