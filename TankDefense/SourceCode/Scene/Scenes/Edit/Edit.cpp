#include "..\SceneList.h"
#include "..\..\..\Utility\Input\Input.h"
#include "..\..\..\Object\CameraBase\CameraManager\CameraManager.h"
#include "..\..\..\Edit\StageEditor\StageEditor.h"

CEdit::CEdit( CSceneManager* pSceneManager )
	: CSceneBase		( pSceneManager )
	, m_StageEditor		( std::make_unique<CStageEditor>() )
	, m_WindowFlags		( ImGuiWindowFlags_None )
{
	m_WindowFlags |= ImGuiWindowFlags_NoTitleBar;	// タイトルバーを消す.
}

CEdit::~CEdit()
{
}

//============================.
//	読込関数.
//============================.
bool CEdit::Load()
{
	if( m_StageEditor->Init() == false ) return false;
	//	フリーカメラを有効化する.
//	CCameraManager::SetActiveFreeCamera();
	return true;
}

//============================.
//	更新関数.
//============================.
void CEdit::Update()
{
	if( CKeyInput::IsMomentPress('G') == true ){
		SetSceneChange();
	}
	m_StageEditor->Update();
}

//============================.
// モデル描画関数.
//============================.
void CEdit::ModelRender()
{
	m_StageEditor->ModelRender();
	m_StageEditor->EffectRneder();
}

//============================.
// 画像描画関数.
//============================.
void CEdit::SpriteRender()
{
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

	// 各エディタ画面をタブ形式で描画.
	if( ImGui::BeginTabBar("TabBar") ){
		m_StageEditor->ImGuiRender();

		ImGui::EndTabBar();
	}
	// ImGuiウィンドウの描画終了.
	ImGui::End();

	// ImGuiデモウィンドウ表示.
	ImGui::ShowDemoWindow();
}
