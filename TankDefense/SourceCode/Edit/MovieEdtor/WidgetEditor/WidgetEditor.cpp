#include "WidgetEditor.h"
#include "..\..\..\Utility\ImGuiManager\ImGuiManager.h"
#include "..\..\..\Utility\Input\Input.h"
#include "..\..\..\Resource\SpriteResource\SpriteResource.h"

namespace
{
	constexpr float BUTTON_MOVE_SPEED	= 0.5f;
	constexpr float AXIS_MOVE_SPEED		= 2.0f;
}

CWidgetEditor::CWidgetEditor()
	: m_pWidgetList			()
	, m_SpriteNameList		()
	, m_NowPushSelectName	()
	, m_NowSelectIndex		( -1 )
	, m_IsAllWidgetRender	( true )
	, m_IsImGuiGamepad		( true )
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
	
	if( m_IsPlay == true ){
		for( auto& w : m_pWidgetList ) w->Update( GetDeltaTime() );
	}

	WidgetControll();
}

//----------------------------------.
// ImGui�`��.
//----------------------------------.
bool CWidgetEditor::ImGuiRender()
{
	PushWidgetDraw();
	SelectEidtParameterDraw();
	ImGui::Checkbox( u8"���ׂẲ摜��`�悷��", &m_IsAllWidgetRender );
	if( ImGui::Button( u8"�������m�F����" ) ){
		for( auto& w : m_pWidgetList ) w->Play();
		m_IsPlay = true;
	}
	ImGui::SameLine();
	if( ImGui::Button( u8"�������~����" ) ){
		for( auto& w : m_pWidgetList ) w->Reset();
		m_IsPlay = false;
	}

	ImGui::Separator();
	WidgetParameterDraw();

	return true;
}

//----------------------------------.
// �摜�̕`��.
//----------------------------------.
void CWidgetEditor::SpriteRender()
{
	if( m_NowSelectIndex < 0 ) return;

	m_pWidgetList[m_NowSelectIndex]->SimpleRender();

	if( m_IsAllWidgetRender == false ) return;

	// ���X�g������Ă���摜�����ׂĕ`��.
	int i = 0;
	for( auto& w : m_pWidgetList ){
		if( i++ == m_NowSelectIndex ) continue;
		w->SimpleRender();
	}
}

//----------------------------------.
// �E�B�W�F�b�g���̎擾.
//----------------------------------.
std::vector<SMovieWidget> CWidgetEditor::GetWidgetStateList()
{
	std::vector<SMovieWidget> stateList;
	for( auto& w : m_pWidgetList ){
		stateList.emplace_back( w->GetMovieWidgetState() );
	}
	return stateList;
}

//----------------------------------.
// �E�B�W�F�b�g���̎擾.
//----------------------------------.
void CWidgetEditor::SetWidgetStateList( const std::vector<SMovieWidget>& stateList )
{
	const int stateSize		= stateList.size();
	const int widgetSize	= m_pWidgetList.size();
	const int diff			= stateSize - widgetSize;
	if( diff == 0 ){

	// stateSize �̕�������.
	} else if( diff >= 1 ){
		for( int i = 0; i < diff; i++ ){
			m_pWidgetList.emplace_back( std::make_unique<CMovieWidget>() );
		}
	
	// widgetSize �̕�������.
	} else if( diff <= -1 ){
		for( int i = 0; i < abs(diff); i++ ){
			m_pWidgetList.pop_back();
		}
	}
	m_pWidgetList.shrink_to_fit();

	for( int i = 0; i < stateSize; i++ ){
		m_pWidgetList[i]->SetMovieWidgetState( stateList[i] );
		m_pWidgetList[i]->Init();
	}
	if( m_pWidgetList.empty() == false ){
		m_NowSelectState = stateList[0];
		m_NowSelectIndex = 0;
	} else {
		m_NowSelectIndex = -1;
	}
}

//----------------------------------.
// ����֐�.
//----------------------------------.
void CWidgetEditor::WidgetControll()
{
	if( m_IsImGuiGamepad == true ) return;

	// ���X�e�B�b�N�̒l���󂯎��.
	const D3DXVECTOR2 moveValue =
		{ CInput::GetAxisValue( EAxisBind::L_Right ), CInput::GetAxisValue( EAxisBind::L_Forward ) };

	if( moveValue.x >  INPUT_AXIS_DEAD_ZONE ) m_NowSelectState.Position.x +=  AXIS_MOVE_SPEED;
	if( moveValue.x < -INPUT_AXIS_DEAD_ZONE ) m_NowSelectState.Position.x += -AXIS_MOVE_SPEED;
	if( moveValue.y >  INPUT_AXIS_DEAD_ZONE ) m_NowSelectState.Position.y += -AXIS_MOVE_SPEED;
	if( moveValue.y < -INPUT_AXIS_DEAD_ZONE ) m_NowSelectState.Position.y +=  AXIS_MOVE_SPEED;

	if( CXInput::IsHold( XINPUT_GAMEPAD_DPAD_LEFT ) )	m_NowSelectState.Position.x += -BUTTON_MOVE_SPEED;
	if( CXInput::IsHold( XINPUT_GAMEPAD_DPAD_RIGHT ) )	m_NowSelectState.Position.x +=  BUTTON_MOVE_SPEED;
	if( CXInput::IsHold( XINPUT_GAMEPAD_DPAD_UP ) )		m_NowSelectState.Position.y += -BUTTON_MOVE_SPEED;
	if( CXInput::IsHold( XINPUT_GAMEPAD_DPAD_DOWN ) )	m_NowSelectState.Position.y +=  BUTTON_MOVE_SPEED;

	if( CInput::IsMomentPress( EKeyBind::Edit_BackMenu ) ) OnImGuiGamepad();
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
		ImGui::SameLine();
		if( ImGui::Button( u8"�ʒu�𓮂��� *GamePad�̂�" ) ){
			OffImGuiGamepad();
		}
		ImGui::Indent();
		ImGui::DragFloat( u8"X", &m_NowSelectState.Position.x, 0.5f, -1280.00f, 1280.0f );
		ImGui::DragFloat( u8"Y", &m_NowSelectState.Position.y, 0.5f, -1280.00f, 1280.0f );
		ImGui::Unindent();

		ImGui::Separator();
		AnimationParameterDraw();

		ImGui::Separator();
		FadeParameterDraw();

		ImGui::PopItemWidth();
		ImGui::TreePop();
	}

	if( m_IsPlay == true ) return;

	m_pWidgetList[m_NowSelectIndex]->SetMovieWidgetState( m_NowSelectState );
	m_pWidgetList[m_NowSelectIndex]->SettingRenderState( true );
}

//------------------------------------.
// �A�j���[�V�����p�����[�^�̕\��.
//------------------------------------.
void CWidgetEditor::AnimationParameterDraw()
{
	// �A�j���[�V�������̎擾.
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
}

//------------------------------------.
// �t�F�[�h�p�����[�^�̕\��.
//------------------------------------.
void CWidgetEditor::FadeParameterDraw()
{
	// �t�F�[�h���̎擾.
	SMovieFadeState& fadeState = m_NowSelectState.FadeState;

	const char* fadeNameList[] =
	{
		u8"���Ȃ�",
		u8"�C��",
		u8"�A�E�g",
		u8"�J��Ԃ�",
	};

	if( ImGui::BeginCombo( u8"�t�F�[�h��Ԃ�I��", fadeNameList[fadeState.FadeFlag] ) ){
		int i = 0;
		for( auto& n : fadeNameList ){
			const bool isSelected = ( n == fadeNameList[i] );
			if( ImGui::Selectable( n, isSelected ) )
				fadeState.FadeFlag = i;
			if( isSelected ) ImGui::SetItemDefaultFocus();
			i++;
		}

		ImGui::EndCombo();
	}

	if( fadeState.FadeFlag == SMovieFadeState::None ) return;

	ImGui::DragFloat( u8"�t�F�[�h���x",		&fadeState.FadeSpeed,	0.01f,	0.0f,	20.0f );
	ImGui::DragFloat( u8"�t�F�[�h�ő�l",	&fadeState.AlphaMax,	0.01f,	0.0f,	01.0f );
	ImGui::DragFloat( u8"�t�F�[�h�ŏ��l",	&fadeState.AlphaMin,	0.01f,	0.0f,	01.0f );

}

//------------------------------------.
// ImGui�ŃQ�[���p�b�h�����L���ɂ���.
//------------------------------------.
void CWidgetEditor::OnImGuiGamepad()
{
	// ImGui�ŃR���g���[���[�����L����.
	CImGuiManager::EnableGamepad();
	ImGui::GetStyle().Colors[ImGuiCol_WindowBg].w = 0.9f;
	m_IsImGuiGamepad = true;
}

//------------------------------------.
// ImGui�ŃQ�[���p�b�h����𖳌��ɂ���.
//------------------------------------.
void CWidgetEditor::OffImGuiGamepad()
{
	// ImGui�ŃR���g���[���[����𖳌���.
	CImGuiManager::DisableGamepad();
	ImGui::GetStyle().Colors[ImGuiCol_WindowBg].w = 0.4f;
	m_IsImGuiGamepad = false;
}