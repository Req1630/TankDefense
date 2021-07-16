#include "..\SceneList.h"
#include "..\..\..\Utility\Input\Input.h"
#include "..\..\..\Object\CameraBase\CameraManager\CameraManager.h"
#include "..\..\..\Edit\StageEditor\StageEditor.h"
#include "..\..\..\Edit\EnemySpawnRangeEditor\EnemySpawnRangeEditor.h"
#include "..\..\..\Edit\MovieEdtor\MovieEdtor.h"
#include "..\..\..\Edit\ActorEditor\ActorEditor.h"
#include "..\..\..\Edit\EditPlayer\EditPlayer.h"

CEdit::CEdit( CSceneManager* pSceneManager )
	: CSceneBase				( pSceneManager )
	, m_pEditPlayer				( std::make_unique<CEditPlayer>() )
	, m_Editors					()
	, m_pActiveEditor			( nullptr )
	, m_WindowFlags				( ImGuiWindowFlags_None )
{
	m_WindowFlags |= ImGuiWindowFlags_NoTitleBar;	// タイトルバーを消す.

	m_Editors.emplace_back( std::make_unique<CStageEditor>() );
	m_Editors.emplace_back( std::make_unique<CEnemySpawnRangeEditor>() );
	m_Editors.emplace_back( std::make_unique<CMovieEditor>() );
	m_Editors.emplace_back( std::make_unique<CActorEditor>() );
	
	m_pActiveEditor = m_Editors.front().get();
}

CEdit::~CEdit()
{
}

//============================.
//	読込関数.
//============================.
bool CEdit::Load()
{
	if( m_pEditPlayer->Init() == false ) return false;
	for( auto& editor : m_Editors ){
		if( editor->Init() == false ) return false;
		editor->SetEditPlayer( m_pEditPlayer.get() );
	}
	//	フリーカメラを有効化する.
//	CCameraManager::SetActiveFreeCamera();
	return true;
}

//============================.
//	更新関数.
//============================.
void CEdit::Update()
{
	m_pActiveEditor->Update();
	//if( CKeyInput::IsMomentPress('E') == true ){
	//	SetSceneChange();
	//}
}

//============================.
// モデル描画関数.
//============================.
void CEdit::ModelRender()
{
	m_pActiveEditor->ModelRender();
	m_pActiveEditor->EffectRneder();
}

//============================.
// 画像描画関数.
//============================.
void CEdit::SpriteRender()
{
	m_pActiveEditor->WidgetRender();

	// ウィンドウのサイズを取得.
	const ImVec2 wnd_size = 
	{
		static_cast<float>(CDirectX11::GetWndWidth()), 
		static_cast<float>(CDirectX11::GetWndHeight())
	};
	// ImGuiのウィンドウを計算.
	const float imWnd_size = wnd_size.x/3.5f;

	// メインビューポートの座標を取得し、ImGuiの座標を設定.
	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImVec2 pos = viewport->WorkPos;
	pos.x = (pos.x+wnd_size.x)-imWnd_size;

	// ImGuiのウィンドウサイズと座標を設定.
	ImGui::SetNextWindowPos(pos);
	ImGui::SetNextWindowSize(ImVec2(imWnd_size, wnd_size.y ));

	// ImGuiウィンドウの描画開始.
	ImGui::Begin( "Edit", nullptr, m_WindowFlags );

	CEditorBase* oldEdit = m_pActiveEditor;

	// 各エディタ画面をタブ形式で描画.
	if( ImGui::BeginTabBar("TabBar") ){
		for( auto& editor : m_Editors ){
			if( editor->ImGuiRender() == true ){
				m_pActiveEditor = editor.get();
			}
		}
		if( oldEdit != m_pActiveEditor ){
			m_pActiveEditor->OnImGuiGamepad();
		}

		ImGui::EndTabBar();
	}
	// ImGuiウィンドウの描画終了.
	ImGui::End();

	// ImGuiデモウィンドウ表示.
	ImGui::ShowDemoWindow();
}
