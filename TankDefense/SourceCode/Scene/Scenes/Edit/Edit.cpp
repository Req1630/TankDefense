#include "..\SceneList.h"
#include "..\..\..\Utility\Input\Input.h"
#include "..\..\..\Object\CameraBase\CameraManager\CameraManager.h"
#include "..\..\..\Edit\StageEditor\StageEditor.h"

CEdit::CEdit( CSceneManager* pSceneManager )
	: CSceneBase		( pSceneManager )
	, m_StageEditor		( std::make_unique<CStageEditor>() )
	, m_WindowFlags		( ImGuiWindowFlags_None )
{
	m_WindowFlags |= ImGuiWindowFlags_NoTitleBar;	// �^�C�g���o�[������.
}

CEdit::~CEdit()
{
}

//============================.
//	�Ǎ��֐�.
//============================.
bool CEdit::Load()
{
	if( m_StageEditor->Init() == false ) return false;
	//	�t���[�J������L��������.
//	CCameraManager::SetActiveFreeCamera();
	return true;
}

//============================.
//	�X�V�֐�.
//============================.
void CEdit::Update()
{
	if( CKeyInput::IsMomentPress('G') == true ){
		SetSceneChange();
	}
	m_StageEditor->Update();
}

//============================.
// ���f���`��֐�.
//============================.
void CEdit::ModelRender()
{
	m_StageEditor->ModelRender();
	m_StageEditor->EffectRneder();
}

//============================.
// �摜�`��֐�.
//============================.
void CEdit::SpriteRender()
{
	// �E�B���h�E�̃T�C�Y���擾.
	const ImVec2 wnd_size = 
	{ 
		static_cast<float>(CDirectX11::GetWndWidth()), 
		static_cast<float>(CDirectX11::GetWndHeight())
	};
	// ImGui�̃E�B���h�E���v�Z.
	const float imWnd_size = wnd_size.x/3.5f;

	// ���C���r���[�|�[�g�̍��W���擾���AImGui�̍��W��ݒ�.
	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImVec2 pos = viewport->WorkPos;
	pos.x = (pos.x+wnd_size.x)-imWnd_size;

	// ImGui�̃E�B���h�E�T�C�Y�ƍ��W��ݒ�.
	ImGui::SetNextWindowPos(pos);
	ImGui::SetNextWindowSize(ImVec2(imWnd_size, wnd_size.y ));

	// ImGui�E�B���h�E�̕`��J�n.
	ImGui::Begin( "Edit", nullptr, m_WindowFlags );

	// �e�G�f�B�^��ʂ��^�u�`���ŕ`��.
	if( ImGui::BeginTabBar("TabBar") ){
		m_StageEditor->ImGuiRender();

		ImGui::EndTabBar();
	}
	// ImGui�E�B���h�E�̕`��I��.
	ImGui::End();

	// ImGui�f���E�B���h�E�\��.
	ImGui::ShowDemoWindow();
}
