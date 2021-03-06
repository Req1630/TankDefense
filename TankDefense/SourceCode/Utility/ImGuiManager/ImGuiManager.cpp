#include "ImGuiManager.h"
#include "..\Input\Input.h"

namespace
{
	constexpr char	FONT_FILE_PATH[]	= "Data\\Sprite\\_Font\\mplus-1p-medium.ttf";
	constexpr float	FONT_SIZE			= 18.0f;
};

CImGuiManager::CImGuiManager()
	: m_RenderQueue	()
#ifdef _DEBUG
	, m_IsRender	( true )
#else	// #ifdef _DEBUG.
	, m_IsRender	( false )
#endif	// #ifdef _DEBUG.
{
}

CImGuiManager::~CImGuiManager()
{
}

// インスタンスの取得.
CImGuiManager* CImGuiManager::GetInstance()
{
	static std::unique_ptr<CImGuiManager> pInstance = std::make_unique<CImGuiManager>();
	return pInstance.get();
}

//----------------------.
// 初期化関数.
//----------------------.
HRESULT CImGuiManager::Init( 
	HWND hWnd, 
	ID3D11Device* pDevice11,
	ID3D11DeviceContext* pContext11 )
{
	IMGUI_CHECKVERSION();

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	// フォントを読み込む.
	ImFont* font = io.Fonts->AddFontFromFileTTF(
		FONT_FILE_PATH, FONT_SIZE,
		nullptr, 
		io.Fonts->GetGlyphRangesJapanese());

	// ゲームパッドの使用を許可する.
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;	// Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;		// Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;		// Enable Multi-Viewport / Platform Windows
	ImGuiStyle& style = ImGui::GetStyle();

	style.ScaleAllSizes( FONT_SIZE*0.06f ); // UIの大きさを一括で変更できます。
	io.FontGlobalScale = FONT_SIZE*0.06f; // フォントの大きさを一括で変更できます。
	ImGui::StyleColorsDark();

	if( io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable ){
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 0.9f;
	}

	if( ImGui_ImplWin32_Init( hWnd ) == false ) return E_FAIL;
	if( ImGui_ImplDX11_Init( pDevice11, pContext11 ) == false ) return E_FAIL;

	return S_OK;
}

//----------------------.
// 解放関数.
//----------------------.
HRESULT CImGuiManager::Release()
{
	// ImGuiの解放.
	ImGui_ImplWin32_Shutdown();
	ImGui_ImplDX11_Shutdown();
	ImGui::DestroyContext();

	return S_OK;
}

//----------------------.
// フレームの設定.
//----------------------.
void CImGuiManager::SetingNewFrame()
{
	if( CKeyInput::IsHold(VK_LCONTROL) == true &&  CKeyInput::IsMomentPress(VK_F6) == true )
		GetInstance()->m_IsRender = !GetInstance()->m_IsRender;
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

//----------------------.
// 描画.
//----------------------.
void CImGuiManager::Render()
{
	if( GetInstance()->m_IsRender == true ){
		const int queueSize = static_cast<int>(GetInstance()->m_RenderQueue.size());
		for( int i = 0; i < queueSize; i++ ){
			std::function<void()> proc = GetInstance()->m_RenderQueue.front();
			proc();
			GetInstance()->m_RenderQueue.pop();
		}
	}
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData( ImGui::GetDrawData() );
	ImGuiIO& io = ImGui::GetIO();
	if( io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable ){
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}
}

//----------------------.
// 描画関数を追加する.
//----------------------.
void CImGuiManager::PushRenderProc( std::function<void()> proc )
{
	if( GetInstance()->m_IsRender == false ) return;
	// 関数の追加.
	GetInstance()->m_RenderQueue.push( proc );
}

//----------------------.
// ヘルプマークの表示.
//----------------------.
void CImGuiManager::HelpMarker( const char* desc )
{
	// コントローラーにも対応させたいので,
	//	ボタンで表示する。その際、ボタンの色をすべて消す.
	ImGui::PushStyleColor( ImGuiCol_Text,			ImGui::GetStyle().Colors[ImGuiCol_TextDisabled] );
	ImGui::PushStyleColor( ImGuiCol_Button,			ImVec4( 0.0f, 0.0f, 0.0f, 0.0f ) );
	ImGui::PushStyleColor( ImGuiCol_ButtonHovered,	ImVec4( 0.0f, 0.0f, 0.0f, 0.0f ) );
	ImGui::PushStyleColor( ImGuiCol_ButtonActive,	ImVec4( 0.0f, 0.0f, 0.0f, 0.0f ) );
	ImGui::PushID(desc);
	ImGui::Button("(?)");
	ImGui::PopID();
	ImGui::PopStyleColor( 4 );
	if( ImGui::IsItemHovered() )
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

//-----------------------------.
// ゲームパッドを有効にする.
//-----------------------------.
void CImGuiManager::EnableGamepad()
{
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;	// ゲームパッド.
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;	// キーボード.
}

//-----------------------------.
// ゲームパッドを無効にする.
//-----------------------------.
void CImGuiManager::DisableGamepad()
{
	ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NavEnableGamepad;	// ゲームパッド.
	ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NavEnableKeyboard;	// キーボード.
}