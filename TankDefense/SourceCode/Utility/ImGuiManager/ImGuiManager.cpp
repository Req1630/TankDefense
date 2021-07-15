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

// �C���X�^���X�̎擾.
CImGuiManager* CImGuiManager::GetInstance()
{
	static std::unique_ptr<CImGuiManager> pInstance = std::make_unique<CImGuiManager>();
	return pInstance.get();
}

//----------------------.
// �������֐�.
//----------------------.
HRESULT CImGuiManager::Init( 
	HWND hWnd, 
	ID3D11Device* pDevice11,
	ID3D11DeviceContext* pContext11 )
{
	IMGUI_CHECKVERSION();

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	// �t�H���g��ǂݍ���.
	ImFont* font = io.Fonts->AddFontFromFileTTF(
		FONT_FILE_PATH, FONT_SIZE,
		nullptr, 
		io.Fonts->GetGlyphRangesJapanese());

	// �Q�[���p�b�h�̎g�p��������.
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;	// Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;		// Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;		// Enable Multi-Viewport / Platform Windows
	ImGuiStyle& style = ImGui::GetStyle();

	style.ScaleAllSizes( FONT_SIZE*0.06f ); // UI�̑傫�����ꊇ�ŕύX�ł��܂��B
	io.FontGlobalScale = FONT_SIZE*0.06f; // �t�H���g�̑傫�����ꊇ�ŕύX�ł��܂��B
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
// ����֐�.
//----------------------.
HRESULT CImGuiManager::Release()
{
	// ImGui�̉��.
	ImGui_ImplWin32_Shutdown();
	ImGui_ImplDX11_Shutdown();
	ImGui::DestroyContext();

	return S_OK;
}

//----------------------.
// �t���[���̐ݒ�.
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
// �`��.
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
// �`��֐���ǉ�����.
//----------------------.
void CImGuiManager::PushRenderProc( std::function<void()> proc )
{
	if( GetInstance()->m_IsRender == false ) return;
	// �֐��̒ǉ�.
	GetInstance()->m_RenderQueue.push( proc );
}

//----------------------.
// �w���v�}�[�N�̕\��.
//----------------------.
void CImGuiManager::HelpMarker( const char* desc )
{
	// �R���g���[���[�ɂ��Ή����������̂�,
	//	�{�^���ŕ\������B���̍ہA�{�^���̐F�����ׂď���.
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
// �Q�[���p�b�h��L���ɂ���.
//-----------------------------.
void CImGuiManager::EnableGamepad()
{
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;	// �Q�[���p�b�h.
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;	// �L�[�{�[�h.
}

//-----------------------------.
// �Q�[���p�b�h�𖳌��ɂ���.
//-----------------------------.
void CImGuiManager::DisableGamepad()
{
	ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NavEnableGamepad;	// �Q�[���p�b�h.
	ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NavEnableKeyboard;	// �L�[�{�[�h.
}