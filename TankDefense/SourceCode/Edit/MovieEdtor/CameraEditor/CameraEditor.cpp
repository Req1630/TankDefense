#include "CameraEditor.h"
#include "..\..\..\Object\CameraBase\MovieCamera\MovieCamera.h"
#include "..\..\..\Utility\BitFlagManager\BitFlagManager.h"
#include "..\..\..\Utility\ImGuiManager\ImGuiManager.h"
#include "..\..\..\Utility\Input\Input.h"
#include "..\..\..\Object\CameraBase\CameraManager\CameraManager.h"
#include "..\..\..\Object\Movie\MovieDataLoader\MovieDataLoader.h"

namespace
{
	const float CAMERA_MOVE_SPEED				= 8.0f;		// �J�����̈ړ����x.
	const float CAMERA_POS_CAMERA_LOOK_DISTANCE = 15.0f;	// �J�����Ǝ��_�̋���.
	const float CONTROLLER_MOVE_SPEED			= 4.0f;		// �R���g���[���[�̈ړ����x.
	const float RADIAN_THRESHOLD_X_MAX			= static_cast<float>(D3DXToRadian(360.0f));	// ���W�A��X�̍ő債�����l.
	const float RADIAN_THRESHOLD_X_MIN			= static_cast<float>(D3DXToRadian(0.0f));	// ���W�A��X�̍ŏ��������l.
	const float RADIAN_THRESHOLD_Y_MAX			= static_cast<float>(D3DXToRadian(90.0f));	// ���W�A��Y�̍ő債�����l.
	const float RADIAN_THRESHOLD_Y_MIN			= static_cast<float>(D3DXToRadian(-90.0f));	// ���W�A��Y�̍ŏ��������l.
	const D3DXVECTOR3	INIT_POSITION			= { 0.0f, 10.0f, -30.0f };	// �������W.
}

CCameraEditor::CCameraEditor()
	: m_pCamera				( std::make_unique<CMovieCamera>() )
	, m_MovieCameraList		()
	, m_CameraState			()
	, m_pMovieMoveCamera	( nullptr )
	, m_pMovieShakeCamera	( nullptr )
	, m_Radian				( 0.0f, 0.0f )
	, m_NowSelectIndex		( -1 )
	, m_DeltaTime			( 0.0f )
	, m_IsCameraControll	( false )
	, m_IsCameraPlaying		( false )
{
}

CCameraEditor::~CCameraEditor()
{
}

//-----------------------------.
// �X�V����.
//-----------------------------.
void CCameraEditor::Update()
{
	m_DeltaTime = GetDeltaTime();

	CameraController();	// �J��������.
	CameraPlaying();	// �J�����Đ�.
}

//-----------------------------.
// ImGui�`��.
//-----------------------------.
bool CCameraEditor::ImGuiRender()
{
#if 0
	static std::vector<std::string> m;
	if( ImGui::Button("TEST") ){
//		m = CCameraDataConverter::ToString( m_MovieCameraList );
		CMovieDataLoader::DataWriting( m_MovieCameraList );
		CMovieDataLoader::DataLoading();
	}
	if( m.empty() == false ){
		for( auto& a : m )
			ImGui::TextWrapped(a.c_str());
	}
#endif

	ComboDraw();

	PlayDraw();

	ImGui::Separator();

	ChangeMoveCamera();
	ImGui::Indent();
	if( ImGui::TreeNode( u8"�ړ��J�����̈ʒu���" ) ){
		PositionDraw( u8"Position", m_pCamera->GetPosition() );
		ImGui::SameLine();
		PositionDraw( u8"Look Position", m_pCamera->GetLookPosition() );
		ImGui::TreePop();
	}
	ImGui::Unindent();



	if( m_pMovieMoveCamera	== nullptr ) return false;
	if( m_pMovieShakeCamera	== nullptr ) return false;
	if( m_NowSelectIndex < 0 ) return false;

	MoveCameraSettigDraw( u8"���ƂȂ�J�����̐ݒ�", m_pMovieMoveCamera->StartState );
	ImGui::Separator();

	if( ImGui::TreeNode( u8"�ړ����̐ݒ�" ) ){
		MoveCameraDraw();
		ImGui::TreePop();
	}

	if( ImGui::TreeNode( u8"�h����̐ݒ�" ) ){
		ShakeCameraDraw();
		ImGui::TreePop();
	}

	

	return true;
}

//-----------------------------.
// ���f���`��.
//-----------------------------.
void CCameraEditor::ModelRender()
{
}

//-----------------------------.
// �J��������.
//-----------------------------.
void CCameraEditor::CameraController()
{
	if( m_IsCameraControll == false ) return;
	if( m_IsCameraPlaying == true ) return;

	PositionUpdate();
	LookPositionUpdate();

	m_pCamera->SetPosition( m_CameraState.Position );
	m_pCamera->SetLookPosition( m_CameraState.LookPosition );
	CCameraManager::ChangeCamera( m_pCamera.get() );
}

//-----------------------------.
// �J�����Đ�.
//-----------------------------.
void CCameraEditor::CameraPlaying()
{
	if( m_IsCameraPlaying == false ) return;

	m_pCamera->Update( m_DeltaTime );

	m_IsCameraPlaying = m_pCamera->IsPlaying();
}

//-----------------------------.
// ���W�̍X�V.
//-----------------------------.
void CCameraEditor::PositionUpdate()
{
	D3DXMATRIX mRot;
	D3DXMatrixRotationYawPitchRoll( &mRot, m_Radian.x, 0.0f, 0.0f );
	// ���x�N�g����p��.
	D3DXVECTOR3 vecAxisX( CAMERA_MOVE_SPEED, 0.0f, 0.0f );	// X��.
	D3DXVECTOR3 vecAxisZ( 0.0f, 0.0f, CAMERA_MOVE_SPEED );	// Z��.
	// X���x�N�g�����̂��̂���]��Ԃɂ��ϊ�����.
	D3DXVec3TransformCoord( &vecAxisX, &vecAxisX, &mRot );
	// Z���x�N�g�����̂��̂���]��Ԃɂ��ϊ�����.
	D3DXVec3TransformCoord( &vecAxisZ, &vecAxisZ, &mRot );

	// �O�i.
	if( CKeyInput::IsHold('W') == true || CXInput::GetLeftThumbY() > IDLE_THUMB_MAX )
		m_CameraState.Position += vecAxisZ * m_DeltaTime;
	// ���.
	if( CKeyInput::IsHold('S') == true || CXInput::GetLeftThumbY() < IDLE_THUMB_MIN )
		m_CameraState.Position -= vecAxisZ * m_DeltaTime;
	// �E�Ɉړ�.
	if( CKeyInput::IsHold('D') == true || CXInput::GetLeftThumbX() > IDLE_THUMB_MAX )
		m_CameraState.Position += vecAxisX * m_DeltaTime;
	// ���Ɉړ�.
	if( CKeyInput::IsHold('A') == true || CXInput::GetLeftThumbX() < IDLE_THUMB_MIN )
		m_CameraState.Position -= vecAxisX * m_DeltaTime;
	// �㏸.
	if( CKeyInput::IsHold('E') == true || CXInput::GetRightTrigger() > IDLE_TIGGER_MAX ) 
		m_CameraState.Position.y += CAMERA_MOVE_SPEED * m_DeltaTime;
	// ���~.
	if( CKeyInput::IsHold('Q') == true || CXInput::GetLeftTrigger() > IDLE_TIGGER_MAX )
		m_CameraState.Position.y -= CAMERA_MOVE_SPEED * m_DeltaTime;
}

//-----------------------------.
// ���_���W�̍X�V.
//-----------------------------.
void CCameraEditor::LookPositionUpdate()
{
	float xSub = 0.0f;
	float ySub = 0.0f;

	if( CXInput::GetRightThumbY() > IDLE_THUMB_MAX ) ySub = CInput::GetAxisValue( EAxisBind::R_Forward )	* -CONTROLLER_MOVE_SPEED;
	if( CXInput::GetRightThumbY() < IDLE_THUMB_MIN ) ySub = CInput::GetAxisValue( EAxisBind::R_Forward )	* -CONTROLLER_MOVE_SPEED;
	if( CXInput::GetRightThumbX() > IDLE_THUMB_MAX ) xSub = CInput::GetAxisValue( EAxisBind::R_Right )		*  CONTROLLER_MOVE_SPEED;
	if( CXInput::GetRightThumbX() < IDLE_THUMB_MIN ) xSub = CInput::GetAxisValue( EAxisBind::R_Right )		*  CONTROLLER_MOVE_SPEED;

	// �}�E�X����ʓ��Ȃ�.
	if( CInput::IsScreenMiddleMouse() == true && CKeyInput::IsHold(VK_LBUTTON) == true ){
		// �}�E�X�̌��݂̍��W�Ɖߋ��̍��W���������l���Z�o.
		if( ySub == 0.0f ) ySub = CInput::GetMousePosisionY() - CInput::GetMouseOldPosisionY();
		if( xSub == 0.0f ) xSub = CInput::GetMousePosisionX() - CInput::GetMouseOldPosisionX();
	}

	m_Radian.x += xSub * 0.005f;
	m_Radian.y -= ySub * 0.005f;
	// �K��l�𒴂��Ȃ��悤����.
	if( m_Radian.x > RADIAN_THRESHOLD_X_MAX ) m_Radian.x = RADIAN_THRESHOLD_X_MIN;
	if( m_Radian.x < RADIAN_THRESHOLD_X_MIN ) m_Radian.x = RADIAN_THRESHOLD_X_MAX;
	if( m_Radian.y > RADIAN_THRESHOLD_Y_MAX ) m_Radian.y = RADIAN_THRESHOLD_Y_MAX;
	if( m_Radian.y < RADIAN_THRESHOLD_Y_MIN ) m_Radian.y = RADIAN_THRESHOLD_Y_MIN;

	// �����ʒu���Z�o.
	m_CameraState.LookPosition.x = m_CameraState.Position.x + ( sinf(m_Radian.x) * CAMERA_POS_CAMERA_LOOK_DISTANCE );
	m_CameraState.LookPosition.y = m_CameraState.Position.y + ( sinf(m_Radian.y) * CAMERA_POS_CAMERA_LOOK_DISTANCE );
	m_CameraState.LookPosition.z = m_CameraState.Position.z + ( cosf(m_Radian.x) * CAMERA_POS_CAMERA_LOOK_DISTANCE );
}

//-----------------------------.
// �J�����ړ������̐؂�ւ�.
//-----------------------------.
void CCameraEditor::ChangeMoveCamera()
{
	if( ImGui::Button( u8"�J�������ړ�������" ) ){
		m_IsCameraControll = true;
		// ImGui�ŃR���g���[���[����𖳌���.
		CImGuiManager::DisableGamepad();
		ImGui::GetStyle().Colors[ImGuiCol_WindowBg].w = 0.4f;
	}
	if( CInput::IsMomentPress(EKeyBind::Edit_BackMenu) ){
		m_IsCameraControll = false;
		// ImGui�ŃR���g���[���[�����L����.
		CImGuiManager::EnableGamepad();
		ImGui::GetStyle().Colors[ImGuiCol_WindowBg].w = 0.9f;
	}
	ImGui::SameLine();
	CImGuiManager::HelpMarker(
		u8"�{�^�����������Ƃ� �J�����𑀍삷�邱�Ƃł�\n"
		u8"���������ʒu�� ���ƂȂ���W�Ȃǂ� �ݒ肷�邱�Ƃ��ł���\n"
		u8"�ȉ��̃{�^���������� �G�f�B�^��ʂɖ߂��\n"
		u8" GamePad -> Select �{�^��\n keyboard -> Back Space �L�["
	);
}

//-----------------------------.
// �R���{�̕\��.
//-----------------------------.
void CCameraEditor::ComboDraw()
{
	ImGui::PushItemWidth( 160.0f );
	std::string noName = "No_" + std::to_string(m_NowSelectIndex+1);
	if( m_NowSelectIndex < 0 ) noName = "None";
	if( ImGui::BeginCombo( "##2", noName.c_str() ) ){
		int i = 0;
		for( auto& actorMesh : m_MovieCameraList ){
			const bool isSelected = ( i == m_NowSelectIndex );
			noName =  "No_" + std::to_string(i+1);

			if( ImGui::Selectable( noName.c_str(), isSelected ) ) m_NowSelectIndex = i;
			if( isSelected ) ImGui::SetItemDefaultFocus();

			i++;
		}
		if( m_NowSelectIndex >= 0 ){
			m_pMovieMoveCamera	= &m_MovieCameraList[m_NowSelectIndex].MoveState;
			m_pMovieShakeCamera	= &m_MovieCameraList[m_NowSelectIndex].ShakeState;
		}
		ImGui::EndCombo();
	}
	ImGui::PopItemWidth();

	ImGui::SameLine();
	if( ImGui::Button( u8"�V�����ǉ�" ) ){
		m_MovieCameraList.emplace_back();
		m_NowSelectIndex	= m_MovieCameraList.size()-1;
		m_pMovieMoveCamera	= &m_MovieCameraList.back().MoveState;
		m_pMovieShakeCamera	= &m_MovieCameraList.back().ShakeState;
	}
	ImGui::SameLine();
	CImGuiManager::HelpMarker( 
		u8"�{�^�����������Ƃ� �V�����J������ǉ����邱�Ƃ��ł���\n"
		u8"�܂� ���X�g����w��̃J������I�������\n�w�肵���J������ �ҏW���邱�Ƃ��ł���\n"
	);
}

//-----------------------------.
// �ړ��J�����̕\��.
//-----------------------------.
void CCameraEditor::MoveCameraDraw()
{
	ImGui::BulletText( u8"�ړ����Ԃ̐ݒ�" );
	ImGui::Indent();

	ImGui::PushItemWidth( 150.0f );
	ImGui::DragFloat( u8"�ʒu �ړ�����(�b)", &m_pMovieMoveCamera->PosMoveTime,		0.1f, 0.0f, 180.0f );
	ImGui::DragFloat( u8"���_ �ړ�����(�b)", &m_pMovieMoveCamera->LookPosMoveTime,	0.1f, 0.0f, 180.0f );
	ImGui::PopItemWidth();

	ImGui::Unindent();

	
	MoveCameraSettigDraw( u8"�I���ʒu�̐ݒ�", m_pMovieMoveCamera->EndState );
}

//-----------------------------.
// �ړ��J�����ݒ�̕\��.
//-----------------------------.
void CCameraEditor::MoveCameraSettigDraw( const char* msg, CCameraBase::SCameraState& state )
{
	ImGui::BulletText( msg );
	ImGui::PushID( msg );
	ImGui::Indent();

	if( ImGui::Button( u8" ���݂̈ʒu��ۑ�" ) ){
		state.Position = m_CameraState.Position;
	}
	ImGui::SameLine();
	if( ImGui::Button( u8" ���݂̒����ʒu��ۑ�" ) ){
		state.LookPosition = m_CameraState.LookPosition;
	}

	if( ImGui::Button( u8"�ۑ������ʒu�����ړ��J�����̍��W�ɐݒ�" ) ){
		m_CameraState = state;
		m_pCamera->SetPosition( state.Position );
		m_pCamera->SetLookPosition( state.LookPosition );
	}

	ImGui::Indent();
	if( ImGui::TreeNode( u8"�ʒu���" ) ){
		PositionDraw( u8"Position", state.Position );
		ImGui::SameLine();
		PositionDraw( u8"Look Position", state.LookPosition );
		ImGui::TreePop();
	}
	ImGui::Unindent();

	ImGui::PopID();
	ImGui::Unindent();
}

//-----------------------------.
// �h��J�����̕\��.
//-----------------------------.
void CCameraEditor::ShakeCameraDraw()
{
	const char* chekboxMsgList[] =
	{
		u8"���W�̏c�����̗h��",
		u8"���W�̉������̗h��",
		u8"���_ ���W�̏c�����̗h��",
		u8"���_ ���W�̉������̗h��",
	};

	int i = 0;
	for( auto& state : m_pMovieShakeCamera->ShakeState ){
		ImGui::Checkbox( chekboxMsgList[i], &state.IsPlaying );
		if( state.IsPlaying == false ){
			i++; 
			continue;
		}

		ImGui::Indent();
		ImGui::PushItemWidth( 100.0f );
		ImGui::PushID( i );
		if( ImGui::TreeNode( u8"�ݒ���##1" ) ){
			ImGui::DragFloat(	u8"�U�ꕝ",		&state.Amplitube,	0.1f, 0.0f, 180.0f );
			ImGui::DragFloat(	u8"���g��",		&state.Frequency,	0.1f, 0.0f, 180.0f );
			ImGui::DragFloat(	u8"�h�ꎞ��",	&state.Time,		0.1f, 0.0f, 180.0f );
			ImGui::Checkbox(	u8"�������邩",	&state.IsAttenuation );
			if( i < 2 ){
				if( ImGui::Button(	u8"���݂̈ʒu�������W�Ƃ��Đݒ�" ) ){
					state.BasePosition = m_CameraState.Position;
				}
				PositionDraw( u8"Position", state.BasePosition );
			} else {
				if( ImGui::Button(	u8"���݂̎��_�ʒu�������W�Ƃ��Đݒ�" ) ){
					state.BasePosition = m_CameraState.LookPosition;
				}
				PositionDraw( u8"Look Position", state.BasePosition );
			}
			ImGui::TreePop();
		}

		ImGui::PopID();
		ImGui::PopItemWidth();
		ImGui::Unindent();
		i++;
	}
}

//-----------------------------.
// �Đ��̕\��.
//-----------------------------.
void CCameraEditor::PlayDraw()
{
	if( ImGui::Button( u8"�Đ�" ) && m_IsCameraPlaying == false ){
		if( m_pMovieMoveCamera	== nullptr ) return;
		if( m_pMovieShakeCamera	== nullptr ) return;
		m_IsCameraPlaying	= true;
		m_pCamera->SetCameraState( { *m_pMovieMoveCamera, *m_pMovieShakeCamera } );
		m_pCamera->Play();
		CCameraManager::ChangeCamera( m_pCamera.get() );
	}
	ImGui::SameLine();
	CImGuiManager::HelpMarker( 
		u8"�{�^�����������Ƃ� ���ݑI�����Ă���J�������̓������m�F���邱�Ƃ��ł���\n"
	);
}

//-----------------------------.
// ���W�̕\��.
//-----------------------------.
void CCameraEditor::PositionDraw( const char* c, const D3DXVECTOR3& pos )
{
	std::string msg = c;
	msg += "\n x : %f\n y : %f\n z : %f\n";
	ImGui::Text( msg.c_str(), pos.x, pos.y, pos.z );
}