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
	m_WindowFlags |= ImGuiWindowFlags_NoTitleBar;	// �^�C�g���o�[������.

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
//	�Ǎ��֐�.
//============================.
bool CEdit::Load()
{
	if( m_pEditPlayer->Init() == false ) return false;
	for( auto& editor : m_Editors ){
		if( editor->Init() == false ) return false;
		editor->SetEditPlayer( m_pEditPlayer.get() );
	}
	//	�t���[�J������L��������.
//	CCameraManager::SetActiveFreeCamera();
	return true;
}

//============================.
//	�X�V�֐�.
//============================.
void CEdit::Update()
{
	m_pActiveEditor->Update();
	//if( CKeyInput::IsMomentPress('E') == true ){
	//	SetSceneChange();
	//}
}

//============================.
// ���f���`��֐�.
//============================.
void CEdit::ModelRender()
{
	m_pActiveEditor->ModelRender();
	m_pActiveEditor->EffectRneder();
}

//============================.
// �摜�`��֐�.
//============================.
void CEdit::SpriteRender()
{
	m_pActiveEditor->WidgetRender();

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

	CEditorBase* oldEdit = m_pActiveEditor;

	// �e�G�f�B�^��ʂ��^�u�`���ŕ`��.
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
	// ImGui�E�B���h�E�̕`��I��.
	ImGui::End();

	// ImGui�f���E�B���h�E�\��.
	ImGui::ShowDemoWindow();
}
