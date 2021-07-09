#include "Main.h"
#include <Windows.h>

#include "..\Common\D3DX\D3DX11.h"
#include "..\Utility\ImGuiManager\ImGuiManager.h"
#include "..\Utility\Log\Log.h"
#include "..\Utility\Input\Input.h"
#include "..\Utility\XAudio2\SoundManager.h"

// ImGui�Ŏg�p.
extern LRESULT ImGui_ImplWin32_WndProcHandler( HWND, UINT, WPARAM, LPARAM );

std::unique_ptr<CMain> pMain = std::make_unique<CMain>();

int WINAPI WinMain( 
	HINSTANCE hInstance, 
	HINSTANCE hInstancePrev, 
	LPSTR pCmdLine, 
	int nCmdShow )
{
#ifdef _DEBUG
	// ���������[�N���o
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif	// #ifdef _DEBUG.

	CLog::OpenLogTextFile();
	
	if( pMain == nullptr ) return 0;

	// �E�B���h�E�̍쐬.
	if( FAILED( pMain->InitWindow( hInstance ) )) return 0;
	// ���C������������.
	if( FAILED( pMain->Init() )) return 0;

	// ���C�����[�v.
	pMain->Loop();

	// ���C���������.
	pMain->Release();
	
	CLog::CloseLogTextFile();

	ShowCursor( TRUE );	// �}�E�X��\������.

	return 0;
}

LRESULT CALLBACK WndProc(
	HWND hWnd, 
	UINT uMsg, 
	WPARAM wParam, 
	LPARAM lParam )
{
	if( ImGui_ImplWin32_WndProcHandler( hWnd, uMsg, wParam, lParam ) )
		return true;
	IMGUI_API
	switch( uMsg ){
	case WM_CREATE:
		break;

	case WM_KEYDOWN:
		// �L�[�ʂ̏���.
		switch( static_cast<char>(wParam) ){
		case VK_ESCAPE:	// ESC�L�[.
#ifdef _DEBUG
			if( MessageBox( nullptr, "�Q�[�����I�����܂����H", "�x��", MB_YESNO ) == IDYES ){ 
				PostQuitMessage(0); 
			}
			break;
#endif	// #ifdef _DEBUG.
		break;
		}
		break;
	case WM_MOUSEWHEEL:	// �}�E�X�z�C�[���̓���.
		{
			const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
			CInput::SetMouseWheelDelta( delta );
		}
		break;
	case WM_CLOSE:
		CSoundManager::Release();
		DestroyWindow( hWnd );
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_SIZE:
		CDirectX11::Resize();
		break;
	}
	return DefWindowProc( hWnd, uMsg, wParam, lParam );
}