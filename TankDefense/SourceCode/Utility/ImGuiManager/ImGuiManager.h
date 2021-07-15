/**
* @file ImGuiManager.h.
* @brief ImGui���Ǘ�����N���X.
* @author ���c���.
*/
#ifndef IMGUI_MANAGER_H
#define IMGUI_MANAGER_H
// �x���ɂ��ẴR�[�h���͂𖳌��ɂ���.4005:�Ē�`.
#pragma warning(disable:4005)

#include <Windows.h>
#include <D3DX11.h>
#include <D3D11.h>

#include <memory>
#include <queue>
#include <functional>

// ���C�u�����ǂݍ���.
#pragma comment( lib, "winmm.lib" )
#pragma comment( lib, "d3dx11.lib" )
#pragma comment( lib, "d3d11.lib" )

#include "..\..\..\Library\ImGui\imgui.h"
#include "..\..\..\Library\ImGui\imgui_impl_dx11.h"
#include "..\..\..\Library\ImGui\imgui_impl_win32.h"
#include "..\..\..\Library\ImGui\imgui_internal.h"
#include "..\..\Utility\FrameRate\FrameRate.h"


class CImGuiManager
{
public:
	struct stSuccess
	{
		float	FrameTime;		// �J�E���g.
		float	RenderTime;		// �`�掞��.
		bool	IsSucceeded;	// �������ǂ���.
		stSuccess()
			: FrameTime		( 0.0f )
			, RenderTime	( 5.0f )
			, IsSucceeded	( false )
		{}
		void Render()
		{
			if( IsSucceeded == false ) return;
			ImGui::Text( u8"����" );
			ImGui::SameLine();
			FrameTime += static_cast<float>(CFrameRate::GetDeltaTime());
			if( FrameTime > RenderTime ) return;
			FrameTime = 0.0f;
			IsSucceeded = false;
		}
	} typedef SSuccess;

public:
	CImGuiManager();
	~CImGuiManager();

	// �C���X�^���X�̎擾.
	static CImGuiManager* GetInstance();

	// �������֐�.
	static HRESULT Init( 
		HWND hWnd, 
		ID3D11Device* pDevice11,
		ID3D11DeviceContext* pContext11 );

	// ����֐�.
	static HRESULT Release();

	// �t���[���̐ݒ�.
	static void SetingNewFrame();

	// �`��.
	static void Render();

	// �`��֐���ǉ�����.
	static void PushRenderProc( std::function<void()> proc );

	// �w���v�}�[�N�̕\��.
	static void HelpMarker( const char* desc );

	// �Q�[���p�b�h��L���ɂ���.
	static void EnableGamepad();
	// �Q�[���p�b�h�𖳌��ɂ���.
	static void DisableGamepad();

private:
	std::queue<std::function<void()>> m_RenderQueue;
	bool m_IsRender;
};

#endif	// #ifndef IMGUI_MANAGER_H.