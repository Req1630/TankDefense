#include "CameraEditor.h"
#include "..\..\..\Object\CameraBase\NormalCamera\NormalCamera.h"
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
	: m_pCamera				( std::make_unique<CNormalCamera>() )
	, m_CameraState			()
	, m_MovieCamera			()
	, m_Radian				( 0.0f, 0.0f )
	, m_DeltaTime			( 0.0f )
	, m_PosMoveTime			( 0.0f )
	, m_LookPosMoveTime		( 0.0f )
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
	if( ImGui::TreeNode( u8"情報##1" ) ){
		ImGui::Text( u8"現在の位置\n x : %f\n y : %f\n z : %f\n", 
			m_pCamera->GetPosition().x,
			m_pCamera->GetPosition().y,
			m_pCamera->GetPosition().z );
		ImGui::Text( u8"現在の視点位置\n x : %f\n y : %f\n z : %f\n", 
			m_pCamera->GetLookPosition().x,
			m_pCamera->GetLookPosition().y,
			m_pCamera->GetLookPosition().z );
		ImGui::TreePop();
	}
	ImGui::Separator();

	if( ImGui::Button( u8"現在の位置を開始位置として保存" ) ){
		m_MovieCamera.StartState.Position = m_CameraState.Position;
	}

	if( ImGui::Button( u8"現在の注視位置を開始位置として保存" ) ){
		m_MovieCamera.StartState.LookPosition = m_CameraState.LookPosition;
	}

	if( ImGui::Button( u8"開始位置の情報をカメラに設定" ) ){
		m_pCamera->SetPosition( m_MovieCamera.StartState.Position );
		m_pCamera->SetLookPosition( m_MovieCamera.StartState.LookPosition );
	}

	if( ImGui::TreeNode( u8"情報##2" ) ){
		ImGui::Text( u8"開始位置\n x : %f\n y : %f\n z : %f\n", 
			m_MovieCamera.StartState.Position.x,
			m_MovieCamera.StartState.Position.y,
			m_MovieCamera.StartState.Position.z );
		ImGui::Text( u8"開始視点位置\n x : %f\n y : %f\n z : %f\n", 
			m_MovieCamera.StartState.LookPosition.x,
			m_MovieCamera.StartState.LookPosition.y,
			m_MovieCamera.StartState.LookPosition.z );
		ImGui::TreePop();
	}

	ImGui::Separator();

	if( ImGui::Button( u8"現在の位置を終了位置として保存" ) ){
		m_MovieCamera.EndState.Position = m_CameraState.Position;
	}

	if( ImGui::Button( u8"現在の注視位置を終了位置として保存" ) ){
		m_MovieCamera.EndState.LookPosition = m_CameraState.LookPosition;
	}
	if( ImGui::Button( u8"終了位置の情報をカメラに設定" ) ){
		m_pCamera->SetPosition( m_MovieCamera.EndState.Position );
		m_pCamera->SetLookPosition( m_MovieCamera.EndState.LookPosition );
	}

	if( ImGui::TreeNode( u8"情報##3" ) ){
		ImGui::Text( u8"終了位置\n x : %f\n y : %f\n z : %f\n", 
			m_MovieCamera.EndState.Position.x,
			m_MovieCamera.EndState.Position.y,
			m_MovieCamera.EndState.Position.z );
		ImGui::Text( u8"終了視点位置\n x : %f\n y : %f\n z : %f\n", 
			m_MovieCamera.EndState.LookPosition.x,
			m_MovieCamera.EndState.LookPosition.y,
			m_MovieCamera.EndState.LookPosition.z );
		ImGui::TreePop();
	}

	ImGui::Separator();

	ImGui::PushItemWidth( 150.0f );
	ImGui::DragFloat( u8"位置移動時間(秒)", &m_MovieCamera.PosMoveTime,		0.1f, 0.1f, 180.0f );
	ImGui::DragFloat( u8"視点移動時間(秒)", &m_MovieCamera.LookPosMoveTime,	0.1f, 0.1f, 180.0f );
	ImGui::PopItemWidth();

	if( ImGui::Button( u8"再生" ) ){
		m_IsCameraPlaying	= true;
		m_PosMoveTime		= 1.0f;
		m_LookPosMoveTime	= 1.0f;
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

	m_pCamera->AnySecondsMovePosition( 
			m_MovieCamera.EndState.Position,
			m_MovieCamera.StartState.Position,
			m_PosMoveTime );

	m_pCamera->AnySecondsMoveLookPosition( 
			m_MovieCamera.EndState.LookPosition,
			m_MovieCamera.StartState.LookPosition,
			m_LookPosMoveTime );

	m_PosMoveTime		-= m_DeltaTime / m_MovieCamera.PosMoveTime;
	m_LookPosMoveTime	-= m_DeltaTime / m_MovieCamera.LookPosMoveTime;

	if( m_PosMoveTime		<= 0.0f ) m_PosMoveTime		= 0.0f;
	if( m_LookPosMoveTime	<= 0.0f ) m_LookPosMoveTime	= 0.0f;

	if( m_PosMoveTime <= 0.0f && m_LookPosMoveTime <= 0.0f ){
		// カメラ動作終了.
		m_IsCameraPlaying = false;
	}
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
	if( ImGui::Button( u8"カメラ操作" ) ){
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