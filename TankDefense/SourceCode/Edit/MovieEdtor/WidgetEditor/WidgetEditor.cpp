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
// 初期化.
//----------------------------------.
bool CWidgetEditor::Init()
{
	m_SpriteNameList = CSpriteResource::GetSpriteNameList();
	m_NowPushSelectName = m_SpriteNameList.front();
	return true;
}

//----------------------------------.
// 更新.
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
// ImGui描画.
//----------------------------------.
bool CWidgetEditor::ImGuiRender()
{
	PushWidgetDraw();
	SelectEidtParameterDraw();
	ImGui::Checkbox( u8"すべての画像を描画する", &m_IsAllWidgetRender );
	if( ImGui::Button( u8"動きを確認する" ) ){
		for( auto& w : m_pWidgetList ) w->Play();
		m_IsPlay = true;
	}
	ImGui::SameLine();
	if( ImGui::Button( u8"動きを停止する" ) ){
		for( auto& w : m_pWidgetList ) w->Reset();
		m_IsPlay = false;
	}

	ImGui::Separator();
	WidgetParameterDraw();

	return true;
}

//----------------------------------.
// 画像の描画.
//----------------------------------.
void CWidgetEditor::SpriteRender()
{
	if( m_NowSelectIndex < 0 ) return;

	m_pWidgetList[m_NowSelectIndex]->SimpleRender();

	if( m_IsAllWidgetRender == false ) return;

	// リスト化されている画像をすべて描画.
	int i = 0;
	for( auto& w : m_pWidgetList ){
		if( i++ == m_NowSelectIndex ) continue;
		w->SimpleRender();
	}
}

//----------------------------------.
// ウィジェット情報の取得.
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
// ウィジェット情報の取得.
//----------------------------------.
void CWidgetEditor::SetWidgetStateList( const std::vector<SMovieWidget>& stateList )
{
	const int stateSize		= stateList.size();
	const int widgetSize	= m_pWidgetList.size();
	const int diff			= stateSize - widgetSize;
	if( diff == 0 ){

	// stateSize の方が多い.
	} else if( diff >= 1 ){
		for( int i = 0; i < diff; i++ ){
			m_pWidgetList.emplace_back( std::make_unique<CMovieWidget>() );
		}
	
	// widgetSize の方が多い.
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
// 操作関数.
//----------------------------------.
void CWidgetEditor::WidgetControll()
{
	if( m_IsImGuiGamepad == true ) return;

	// 左スティックの値を受け取る.
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
// ウィジェットの追加の表示.
//----------------------------------.
void CWidgetEditor::PushWidgetDraw()
{
	ImGui::Text( u8"追加したい画像を選択" );
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
	if( ImGui::Button( u8"追加" ) ){
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
// 編集するウィジェットの表示.
//----------------------------------.
void CWidgetEditor::SelectEidtParameterDraw()
{
	ImGui::Text( u8"編集したい画像を選択" );

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
// ウィジェットパラメータの表示.
//----------------------------------.
void CWidgetEditor::WidgetParameterDraw()
{
	if( m_NowSelectIndex < 0 ) return;

	ImGui::Text( u8"画像名 : %s", m_NowSelectState.SpriteName.c_str() );

	if( ImGui::TreeNode( u8"パラメータを編集する" ) ){
		ImGui::PushItemWidth( 160.0f );

		ImGui::DragFloat( u8"動作開始時間", &m_NowSelectState.AciveStartTime, 0.1f, 0.0f, 180.0f );

		ImGui::Text( u8" Position" );
		ImGui::SameLine();
		if( ImGui::Button( u8"位置を動かす *GamePadのみ" ) ){
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
// アニメーションパラメータの表示.
//------------------------------------.
void CWidgetEditor::AnimationParameterDraw()
{
	// アニメーション情報の取得.
	SMovieAnimState& animState = m_NowSelectState.AnimState;

	ImGui::Checkbox( u8"横方向にUVスクロールをするか", &animState.IsUVScrollX );
	if( animState.IsUVScrollX  == true ){
		ImGui::Indent();
		ImGui::DragFloat( u8"スクロール速度##1", &animState.UVScrollSpeed.x, 0.0001f, -20.0f, 20.0f, "%.4f" );
		ImGui::Unindent();
	}

	ImGui::Checkbox( u8"縦方向にUVスクロールをするか", &animState.IsUVScrollY );
	if( animState.IsUVScrollY  == true ){
		ImGui::Indent();
		ImGui::DragFloat( u8"スクロール速度##2", &animState.UVScrollSpeed.y, 0.0001f, -20.0f, 20.0f, "%.4f" );
		ImGui::Unindent();
	}

	ImGui::Checkbox( u8"スプライトアニメーションをするか", &animState.IsAnimation );
	if( animState.IsAnimation == true ){
		ImGui::Indent();
		ImGui::DragInt( u8"アニメーション速度", &animState.FrameCountSpeed, 1.0f, 0, 20 );
		ImGui::Unindent();
	}
}

//------------------------------------.
// フェードパラメータの表示.
//------------------------------------.
void CWidgetEditor::FadeParameterDraw()
{
	// フェード情報の取得.
	SMovieFadeState& fadeState = m_NowSelectState.FadeState;

	const char* fadeNameList[] =
	{
		u8"しない",
		u8"イン",
		u8"アウト",
		u8"繰り返し",
	};

	if( ImGui::BeginCombo( u8"フェード状態を選択", fadeNameList[fadeState.FadeFlag] ) ){
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

	ImGui::DragFloat( u8"フェード速度",		&fadeState.FadeSpeed,	0.01f,	0.0f,	20.0f );
	ImGui::DragFloat( u8"フェード最大値",	&fadeState.AlphaMax,	0.01f,	0.0f,	01.0f );
	ImGui::DragFloat( u8"フェード最小値",	&fadeState.AlphaMin,	0.01f,	0.0f,	01.0f );

}

//------------------------------------.
// ImGuiでゲームパッド操作を有効にする.
//------------------------------------.
void CWidgetEditor::OnImGuiGamepad()
{
	// ImGuiでコントローラー操作を有効化.
	CImGuiManager::EnableGamepad();
	ImGui::GetStyle().Colors[ImGuiCol_WindowBg].w = 0.9f;
	m_IsImGuiGamepad = true;
}

//------------------------------------.
// ImGuiでゲームパッド操作を無効にする.
//------------------------------------.
void CWidgetEditor::OffImGuiGamepad()
{
	// ImGuiでコントローラー操作を無効化.
	CImGuiManager::DisableGamepad();
	ImGui::GetStyle().Colors[ImGuiCol_WindowBg].w = 0.4f;
	m_IsImGuiGamepad = false;
}