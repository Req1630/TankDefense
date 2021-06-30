#include "CameraEditor.h"
#include "..\..\..\Object\CameraBase\MovieCamera\MovieCamera.h"
#include "..\..\..\Utility\BitFlagManager\BitFlagManager.h"
#include "..\..\..\Utility\ImGuiManager\ImGuiManager.h"
#include "..\..\..\Utility\Input\Input.h"
#include "..\..\..\Object\CameraBase\CameraManager\CameraManager.h"

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
	, m_CameraState			()
	, m_MovieMoveCamera		()
	, m_MovieShakeCamera	()
	, m_Radian				( 0.0f, 0.0f )
	, m_DeltaTime			( 0.0f )
	, m_ShakePosFlag		( EShakePosFlag_None )
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
	ChangeMoveCamera();
	if( ImGui::TreeNode( u8"���݂̈ʒu���##1" ) ){
		ImGui::Text( u8"Position \n x : %f\n y : %f\n z : %f\n", 
			m_pCamera->GetPosition().x,
			m_pCamera->GetPosition().y,
			m_pCamera->GetPosition().z );
		ImGui::Text( u8"Look Position \n x : %f\n y : %f\n z : %f\n", 
			m_pCamera->GetLookPosition().x,
			m_pCamera->GetLookPosition().y,
			m_pCamera->GetLookPosition().z );
		ImGui::TreePop();
	}
	ImGui::Separator();

	if( ImGui::TreeNode( u8"�ړ����̐ݒ�" ) ){
		MoveCameraDraw();
		ImGui::TreePop();
	}

	if( ImGui::TreeNode( u8"�h����̐ݒ�" ) ){
		ShakeCameraDraw();
		ImGui::TreePop();
	}

	if( ImGui::Button( u8"�Đ�" ) && m_IsCameraPlaying == false ){
		m_IsCameraPlaying	= true;
		shake_pos_flag flag = EShakePosFlag_PosVer;
		int i = 0;
		SMovieShakeCamera shakeCamera;
		for( auto& state : m_MovieShakeCamera.ShakeState ){
			ImGui::Indent();
			if( bit::IsBitFlag( m_ShakePosFlag, flag ) ){
				shakeCamera.ShakeState[i] = state;
			}
			flag <<= 1; i++;
		}
		m_pCamera->SetCameraState( { m_MovieMoveCamera, shakeCamera } );
		m_pCamera->Play();
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
}

//-----------------------------.
// �ړ��J�����̕\��.
//-----------------------------.
void CCameraEditor::MoveCameraDraw()
{
	if( ImGui::Button( u8"���݂̈ʒu���J�n�ʒu�Ƃ��ĕۑ�" ) ){
		m_MovieMoveCamera.StartState.Position = m_CameraState.Position;
	}

	if( ImGui::Button( u8"���݂̒����ʒu���J�n�ʒu�Ƃ��ĕۑ�" ) ){
		m_MovieMoveCamera.StartState.LookPosition = m_CameraState.LookPosition;
	}

	if( ImGui::Button( u8"�J�n�ʒu�̏����J�����ɐݒ�" ) ){
		m_pCamera->SetPosition( m_MovieMoveCamera.StartState.Position );
		m_pCamera->SetLookPosition( m_MovieMoveCamera.StartState.LookPosition );
	}

	if( ImGui::TreeNode( u8"�ʒu���##2" ) ){
		ImGui::Text( u8"�J�n�ʒu\n x : %f\n y : %f\n z : %f\n", 
					 m_MovieMoveCamera.StartState.Position.x,
					 m_MovieMoveCamera.StartState.Position.y,
					 m_MovieMoveCamera.StartState.Position.z );
		ImGui::Text( u8"�J�n���_�ʒu\n x : %f\n y : %f\n z : %f\n", 
					 m_MovieMoveCamera.StartState.LookPosition.x,
					 m_MovieMoveCamera.StartState.LookPosition.y,
					 m_MovieMoveCamera.StartState.LookPosition.z );
		ImGui::TreePop();
	}

	ImGui::Separator();

	if( ImGui::Button( u8"���݂̈ʒu���I���ʒu�Ƃ��ĕۑ�" ) ){
		m_MovieMoveCamera.EndState.Position = m_CameraState.Position;
	}

	if( ImGui::Button( u8"���݂̒����ʒu���I���ʒu�Ƃ��ĕۑ�" ) ){
		m_MovieMoveCamera.EndState.LookPosition = m_CameraState.LookPosition;
	}
	if( ImGui::Button( u8"�I���ʒu�̏����J�����ɐݒ�" ) ){
		m_pCamera->SetPosition( m_MovieMoveCamera.EndState.Position );
		m_pCamera->SetLookPosition( m_MovieMoveCamera.EndState.LookPosition );
	}

	if( ImGui::TreeNode( u8"�ʒu���##3" ) ){
		ImGui::Text( u8"�I���ʒu\n x : %f\n y : %f\n z : %f\n", 
					 m_MovieMoveCamera.EndState.Position.x,
					 m_MovieMoveCamera.EndState.Position.y,
					 m_MovieMoveCamera.EndState.Position.z );
		ImGui::Text( u8"�I�����_�ʒu\n x : %f\n y : %f\n z : %f\n", 
					 m_MovieMoveCamera.EndState.LookPosition.x,
					 m_MovieMoveCamera.EndState.LookPosition.y,
					 m_MovieMoveCamera.EndState.LookPosition.z );
		ImGui::TreePop();
	}

	ImGui::Separator();

	ImGui::PushItemWidth( 150.0f );
	ImGui::DragFloat( u8"�ʒu�ړ�����(�b)", &m_MovieMoveCamera.PosMoveTime,		0.1f, 0.1f, 180.0f );
	ImGui::DragFloat( u8"���_�ړ�����(�b)", &m_MovieMoveCamera.LookPosMoveTime,	0.1f, 0.1f, 180.0f );
	ImGui::PopItemWidth();
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
		u8"���_���W�̏c�����̗h��",
		u8"���_���W�̉������̗h��",
	};

	shake_pos_flag flag = EShakePosFlag_PosVer;
	int i = 0;
	for( auto& state : m_MovieShakeCamera.ShakeState ){
		ImGui::CheckboxFlags( chekboxMsgList[i], &m_ShakePosFlag, flag );
		ImGui::Indent();
		if( bit::IsBitFlag( m_ShakePosFlag, flag ) ){
			ImGui::PushItemWidth( 100.0f );
			ImGui::PushID( i );
			if( ImGui::TreeNode( u8"�ݒ���##1" ) ){
				ImGui::DragFloat(	u8"�U�ꕝ",		&state.Amplitube );
				ImGui::DragFloat(	u8"���g��",		&state.Frequency );
				ImGui::DragFloat(	u8"�h�ꎞ��",	&state.Time );
				ImGui::Checkbox(	u8"�������邩",	&state.IsAttenuation );
				if( i < 2 ){
					if( ImGui::Button(	u8"���݂̈ʒu�������W�Ƃ��Đݒ�" ) ){
						state.BasePosition = m_CameraState.Position;
					}
					ImGui::Text( u8"Position \n x : %f\n y : %f\n z : %f\n", 
								 state.BasePosition.x,
								 state.BasePosition.y,
								 state.BasePosition.z );
				} else {
					if( ImGui::Button(	u8"���݂̎��_�ʒu�������W�Ƃ��Đݒ�" ) ){
						state.BasePosition = m_CameraState.LookPosition;
					}
					ImGui::Text( u8"Look Position \n x : %f\n y : %f\n z : %f\n", 
								 state.BasePosition.x,
								 state.BasePosition.y,
								 state.BasePosition.z );
				}
				ImGui::TreePop();
			}
			ImGui::PopID();
			ImGui::PopItemWidth();
		}
		ImGui::Unindent();
		flag <<= 1; i++;
	}
}