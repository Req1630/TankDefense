#include "CameraEditor.h"
#include "..\..\..\Object\CameraBase\MovieCamera\MovieCamera.h"
#include "..\..\..\Utility\BitFlagManager\BitFlagManager.h"
#include "..\..\..\Utility\ImGuiManager\ImGuiManager.h"
#include "..\..\..\Utility\Input\Input.h"
#include "..\..\..\Object\CameraBase\CameraManager\CameraManager.h"

namespace
{
	const float CAMERA_MOVE_SPEED				= 8.0f;		// カメラの移動速度.
	const float CAMERA_POS_CAMERA_LOOK_DISTANCE = 15.0f;	// カメラと視点の距離.
	const float CONTROLLER_MOVE_SPEED			= 4.0f;		// コントローラーの移動速度.
	const float RADIAN_THRESHOLD_X_MAX			= static_cast<float>(D3DXToRadian(360.0f));	// ラジアンXの最大しきい値.
	const float RADIAN_THRESHOLD_X_MIN			= static_cast<float>(D3DXToRadian(0.0f));	// ラジアンXの最小しきい値.
	const float RADIAN_THRESHOLD_Y_MAX			= static_cast<float>(D3DXToRadian(90.0f));	// ラジアンYの最大しきい値.
	const float RADIAN_THRESHOLD_Y_MIN			= static_cast<float>(D3DXToRadian(-90.0f));	// ラジアンYの最小しきい値.
	const D3DXVECTOR3	INIT_POSITION			= { 0.0f, 10.0f, -30.0f };	// 初期座標.
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
// 更新処理.
//-----------------------------.
void CCameraEditor::Update()
{
	m_DeltaTime = GetDeltaTime();

	CameraController();	// カメラ操作.
	CameraPlaying();	// カメラ再生.
}

//-----------------------------.
// ImGui描画.
//-----------------------------.
bool CCameraEditor::ImGuiRender()
{
	ChangeMoveCamera();
	if( ImGui::TreeNode( u8"現在の位置情報##1" ) ){
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

	if( ImGui::TreeNode( u8"移動情報の設定" ) ){
		MoveCameraDraw();
		ImGui::TreePop();
	}

	if( ImGui::TreeNode( u8"揺れ情報の設定" ) ){
		ShakeCameraDraw();
		ImGui::TreePop();
	}

	if( ImGui::Button( u8"再生" ) && m_IsCameraPlaying == false ){
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
// モデル描画.
//-----------------------------.
void CCameraEditor::ModelRender()
{
}

//-----------------------------.
// カメラ操作.
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
// カメラ再生.
//-----------------------------.
void CCameraEditor::CameraPlaying()
{
	if( m_IsCameraPlaying == false ) return;

	m_pCamera->Update( m_DeltaTime );

	m_IsCameraPlaying = m_pCamera->IsPlaying();
}

//-----------------------------.
// 座標の更新.
//-----------------------------.
void CCameraEditor::PositionUpdate()
{
	D3DXMATRIX mRot;
	D3DXMatrixRotationYawPitchRoll( &mRot, m_Radian.x, 0.0f, 0.0f );
	// 軸ベクトルを用意.
	D3DXVECTOR3 vecAxisX( CAMERA_MOVE_SPEED, 0.0f, 0.0f );	// X軸.
	D3DXVECTOR3 vecAxisZ( 0.0f, 0.0f, CAMERA_MOVE_SPEED );	// Z軸.
	// X軸ベクトルそのものを回転状態により変換する.
	D3DXVec3TransformCoord( &vecAxisX, &vecAxisX, &mRot );
	// Z軸ベクトルそのものを回転状態により変換する.
	D3DXVec3TransformCoord( &vecAxisZ, &vecAxisZ, &mRot );

	// 前進.
	if( CKeyInput::IsHold('W') == true || CXInput::GetLeftThumbY() > IDLE_THUMB_MAX )
		m_CameraState.Position += vecAxisZ * m_DeltaTime;
	// 後退.
	if( CKeyInput::IsHold('S') == true || CXInput::GetLeftThumbY() < IDLE_THUMB_MIN )
		m_CameraState.Position -= vecAxisZ * m_DeltaTime;
	// 右に移動.
	if( CKeyInput::IsHold('D') == true || CXInput::GetLeftThumbX() > IDLE_THUMB_MAX )
		m_CameraState.Position += vecAxisX * m_DeltaTime;
	// 左に移動.
	if( CKeyInput::IsHold('A') == true || CXInput::GetLeftThumbX() < IDLE_THUMB_MIN )
		m_CameraState.Position -= vecAxisX * m_DeltaTime;
	// 上昇.
	if( CKeyInput::IsHold('E') == true || CXInput::GetRightTrigger() > IDLE_TIGGER_MAX ) 
		m_CameraState.Position.y += CAMERA_MOVE_SPEED * m_DeltaTime;
	// 下降.
	if( CKeyInput::IsHold('Q') == true || CXInput::GetLeftTrigger() > IDLE_TIGGER_MAX )
		m_CameraState.Position.y -= CAMERA_MOVE_SPEED * m_DeltaTime;
}

//-----------------------------.
// 視点座標の更新.
//-----------------------------.
void CCameraEditor::LookPositionUpdate()
{
	float xSub = 0.0f;
	float ySub = 0.0f;

	if( CXInput::GetRightThumbY() > IDLE_THUMB_MAX ) ySub = CInput::GetAxisValue( EAxisBind::R_Forward )	* -CONTROLLER_MOVE_SPEED;
	if( CXInput::GetRightThumbY() < IDLE_THUMB_MIN ) ySub = CInput::GetAxisValue( EAxisBind::R_Forward )	* -CONTROLLER_MOVE_SPEED;
	if( CXInput::GetRightThumbX() > IDLE_THUMB_MAX ) xSub = CInput::GetAxisValue( EAxisBind::R_Right )		*  CONTROLLER_MOVE_SPEED;
	if( CXInput::GetRightThumbX() < IDLE_THUMB_MIN ) xSub = CInput::GetAxisValue( EAxisBind::R_Right )		*  CONTROLLER_MOVE_SPEED;

	// マウスが画面内なら.
	if( CInput::IsScreenMiddleMouse() == true && CKeyInput::IsHold(VK_LBUTTON) == true ){
		// マウスの現在の座標と過去の座標を引いた値を算出.
		if( ySub == 0.0f ) ySub = CInput::GetMousePosisionY() - CInput::GetMouseOldPosisionY();
		if( xSub == 0.0f ) xSub = CInput::GetMousePosisionX() - CInput::GetMouseOldPosisionX();
	}

	m_Radian.x += xSub * 0.005f;
	m_Radian.y -= ySub * 0.005f;
	// 規定値を超えないよう調整.
	if( m_Radian.x > RADIAN_THRESHOLD_X_MAX ) m_Radian.x = RADIAN_THRESHOLD_X_MIN;
	if( m_Radian.x < RADIAN_THRESHOLD_X_MIN ) m_Radian.x = RADIAN_THRESHOLD_X_MAX;
	if( m_Radian.y > RADIAN_THRESHOLD_Y_MAX ) m_Radian.y = RADIAN_THRESHOLD_Y_MAX;
	if( m_Radian.y < RADIAN_THRESHOLD_Y_MIN ) m_Radian.y = RADIAN_THRESHOLD_Y_MIN;

	// 注視位置を算出.
	m_CameraState.LookPosition.x = m_CameraState.Position.x + ( sinf(m_Radian.x) * CAMERA_POS_CAMERA_LOOK_DISTANCE );
	m_CameraState.LookPosition.y = m_CameraState.Position.y + ( sinf(m_Radian.y) * CAMERA_POS_CAMERA_LOOK_DISTANCE );
	m_CameraState.LookPosition.z = m_CameraState.Position.z + ( cosf(m_Radian.x) * CAMERA_POS_CAMERA_LOOK_DISTANCE );
}

//-----------------------------.
// カメラ移動処理の切り替え.
//-----------------------------.
void CCameraEditor::ChangeMoveCamera()
{
	if( ImGui::Button( u8"カメラを移動させる" ) ){
		m_IsCameraControll = true;
		// ImGuiでコントローラー操作を無効化.
		CImGuiManager::DisableGamepad();
		ImGui::GetStyle().Colors[ImGuiCol_WindowBg].w = 0.4f;
	}
	if( CInput::IsMomentPress(EKeyBind::Edit_BackMenu) ){
		m_IsCameraControll = false;
		// ImGuiでコントローラー操作を有効化.
		CImGuiManager::EnableGamepad();
		ImGui::GetStyle().Colors[ImGuiCol_WindowBg].w = 0.9f;
	}
}

//-----------------------------.
// 移動カメラの表示.
//-----------------------------.
void CCameraEditor::MoveCameraDraw()
{
	if( ImGui::Button( u8"現在の位置を開始位置として保存" ) ){
		m_MovieMoveCamera.StartState.Position = m_CameraState.Position;
	}

	if( ImGui::Button( u8"現在の注視位置を開始位置として保存" ) ){
		m_MovieMoveCamera.StartState.LookPosition = m_CameraState.LookPosition;
	}

	if( ImGui::Button( u8"開始位置の情報をカメラに設定" ) ){
		m_pCamera->SetPosition( m_MovieMoveCamera.StartState.Position );
		m_pCamera->SetLookPosition( m_MovieMoveCamera.StartState.LookPosition );
	}

	if( ImGui::TreeNode( u8"位置情報##2" ) ){
		ImGui::Text( u8"開始位置\n x : %f\n y : %f\n z : %f\n", 
					 m_MovieMoveCamera.StartState.Position.x,
					 m_MovieMoveCamera.StartState.Position.y,
					 m_MovieMoveCamera.StartState.Position.z );
		ImGui::Text( u8"開始視点位置\n x : %f\n y : %f\n z : %f\n", 
					 m_MovieMoveCamera.StartState.LookPosition.x,
					 m_MovieMoveCamera.StartState.LookPosition.y,
					 m_MovieMoveCamera.StartState.LookPosition.z );
		ImGui::TreePop();
	}

	ImGui::Separator();

	if( ImGui::Button( u8"現在の位置を終了位置として保存" ) ){
		m_MovieMoveCamera.EndState.Position = m_CameraState.Position;
	}

	if( ImGui::Button( u8"現在の注視位置を終了位置として保存" ) ){
		m_MovieMoveCamera.EndState.LookPosition = m_CameraState.LookPosition;
	}
	if( ImGui::Button( u8"終了位置の情報をカメラに設定" ) ){
		m_pCamera->SetPosition( m_MovieMoveCamera.EndState.Position );
		m_pCamera->SetLookPosition( m_MovieMoveCamera.EndState.LookPosition );
	}

	if( ImGui::TreeNode( u8"位置情報##3" ) ){
		ImGui::Text( u8"終了位置\n x : %f\n y : %f\n z : %f\n", 
					 m_MovieMoveCamera.EndState.Position.x,
					 m_MovieMoveCamera.EndState.Position.y,
					 m_MovieMoveCamera.EndState.Position.z );
		ImGui::Text( u8"終了視点位置\n x : %f\n y : %f\n z : %f\n", 
					 m_MovieMoveCamera.EndState.LookPosition.x,
					 m_MovieMoveCamera.EndState.LookPosition.y,
					 m_MovieMoveCamera.EndState.LookPosition.z );
		ImGui::TreePop();
	}

	ImGui::Separator();

	ImGui::PushItemWidth( 150.0f );
	ImGui::DragFloat( u8"位置移動時間(秒)", &m_MovieMoveCamera.PosMoveTime,		0.1f, 0.1f, 180.0f );
	ImGui::DragFloat( u8"視点移動時間(秒)", &m_MovieMoveCamera.LookPosMoveTime,	0.1f, 0.1f, 180.0f );
	ImGui::PopItemWidth();
}

//-----------------------------.
// 揺れカメラの表示.
//-----------------------------.
void CCameraEditor::ShakeCameraDraw()
{
	const char* chekboxMsgList[] =
	{
		u8"座標の縦方向の揺れ",
		u8"座標の横方向の揺れ",
		u8"視点座標の縦方向の揺れ",
		u8"視点座標の横方向の揺れ",
	};

	shake_pos_flag flag = EShakePosFlag_PosVer;
	int i = 0;
	for( auto& state : m_MovieShakeCamera.ShakeState ){
		ImGui::CheckboxFlags( chekboxMsgList[i], &m_ShakePosFlag, flag );
		ImGui::Indent();
		if( bit::IsBitFlag( m_ShakePosFlag, flag ) ){
			ImGui::PushItemWidth( 100.0f );
			ImGui::PushID( i );
			if( ImGui::TreeNode( u8"設定情報##1" ) ){
				ImGui::DragFloat(	u8"振れ幅",		&state.Amplitube );
				ImGui::DragFloat(	u8"周波数",		&state.Frequency );
				ImGui::DragFloat(	u8"揺れ時間",	&state.Time );
				ImGui::Checkbox(	u8"減衰するか",	&state.IsAttenuation );
				if( i < 2 ){
					if( ImGui::Button(	u8"現在の位置を基底座標として設定" ) ){
						state.BasePosition = m_CameraState.Position;
					}
					ImGui::Text( u8"Position \n x : %f\n y : %f\n z : %f\n", 
								 state.BasePosition.x,
								 state.BasePosition.y,
								 state.BasePosition.z );
				} else {
					if( ImGui::Button(	u8"現在の視点位置を基底座標として設定" ) ){
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