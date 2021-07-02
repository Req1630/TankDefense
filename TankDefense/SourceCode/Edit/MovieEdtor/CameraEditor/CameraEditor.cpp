#include "CameraEditor.h"
#include "..\..\..\Object\CameraBase\MovieCamera\MovieCamera.h"
#include "..\..\..\Utility\BitFlagManager\BitFlagManager.h"
#include "..\..\..\Utility\ImGuiManager\ImGuiManager.h"
#include "..\..\..\Utility\Input\Input.h"
#include "..\..\..\Object\CameraBase\CameraManager\CameraManager.h"
#include "..\..\..\Object\Movie\MovieDataLoader\MovieDataLoader.h"

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
	if( ImGui::TreeNode( u8"移動カメラの位置情報" ) ){
		PositionDraw( u8"Position", m_pCamera->GetPosition() );
		ImGui::SameLine();
		PositionDraw( u8"Look Position", m_pCamera->GetLookPosition() );
		ImGui::TreePop();
	}
	ImGui::Unindent();



	if( m_pMovieMoveCamera	== nullptr ) return false;
	if( m_pMovieShakeCamera	== nullptr ) return false;
	if( m_NowSelectIndex < 0 ) return false;

	MoveCameraSettigDraw( u8"基底となるカメラの設定", m_pMovieMoveCamera->StartState );
	ImGui::Separator();

	if( ImGui::TreeNode( u8"移動情報の設定" ) ){
		MoveCameraDraw();
		ImGui::TreePop();
	}

	if( ImGui::TreeNode( u8"揺れ情報の設定" ) ){
		ShakeCameraDraw();
		ImGui::TreePop();
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
	ImGui::SameLine();
	CImGuiManager::HelpMarker(
		u8"ボタンを押すことで カメラを操作することでき\n"
		u8"動かした位置に 基底となる座標などを 設定することができる\n"
		u8"以下のボタンを押せば エディタ画面に戻れる\n"
		u8" GamePad -> Select ボタン\n keyboard -> Back Space キー"
	);
}

//-----------------------------.
// コンボの表示.
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
	if( ImGui::Button( u8"新しく追加" ) ){
		m_MovieCameraList.emplace_back();
		m_NowSelectIndex	= m_MovieCameraList.size()-1;
		m_pMovieMoveCamera	= &m_MovieCameraList.back().MoveState;
		m_pMovieShakeCamera	= &m_MovieCameraList.back().ShakeState;
	}
	ImGui::SameLine();
	CImGuiManager::HelpMarker( 
		u8"ボタンを押すことで 新しいカメラを追加することができる\n"
		u8"また リストから指定のカメラを選択すれば\n指定したカメラを 編集することができる\n"
	);
}

//-----------------------------.
// 移動カメラの表示.
//-----------------------------.
void CCameraEditor::MoveCameraDraw()
{
	ImGui::BulletText( u8"移動時間の設定" );
	ImGui::Indent();

	ImGui::PushItemWidth( 150.0f );
	ImGui::DragFloat( u8"位置 移動時間(秒)", &m_pMovieMoveCamera->PosMoveTime,		0.1f, 0.0f, 180.0f );
	ImGui::DragFloat( u8"視点 移動時間(秒)", &m_pMovieMoveCamera->LookPosMoveTime,	0.1f, 0.0f, 180.0f );
	ImGui::PopItemWidth();

	ImGui::Unindent();

	
	MoveCameraSettigDraw( u8"終了位置の設定", m_pMovieMoveCamera->EndState );
}

//-----------------------------.
// 移動カメラ設定の表示.
//-----------------------------.
void CCameraEditor::MoveCameraSettigDraw( const char* msg, CCameraBase::SCameraState& state )
{
	ImGui::BulletText( msg );
	ImGui::PushID( msg );
	ImGui::Indent();

	if( ImGui::Button( u8" 現在の位置を保存" ) ){
		state.Position = m_CameraState.Position;
	}
	ImGui::SameLine();
	if( ImGui::Button( u8" 現在の注視位置を保存" ) ){
		state.LookPosition = m_CameraState.LookPosition;
	}

	if( ImGui::Button( u8"保存した位置情報を移動カメラの座標に設定" ) ){
		m_CameraState = state;
		m_pCamera->SetPosition( state.Position );
		m_pCamera->SetLookPosition( state.LookPosition );
	}

	ImGui::Indent();
	if( ImGui::TreeNode( u8"位置情報" ) ){
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
// 揺れカメラの表示.
//-----------------------------.
void CCameraEditor::ShakeCameraDraw()
{
	const char* chekboxMsgList[] =
	{
		u8"座標の縦方向の揺れ",
		u8"座標の横方向の揺れ",
		u8"視点 座標の縦方向の揺れ",
		u8"視点 座標の横方向の揺れ",
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
		if( ImGui::TreeNode( u8"設定情報##1" ) ){
			ImGui::DragFloat(	u8"振れ幅",		&state.Amplitube,	0.1f, 0.0f, 180.0f );
			ImGui::DragFloat(	u8"周波数",		&state.Frequency,	0.1f, 0.0f, 180.0f );
			ImGui::DragFloat(	u8"揺れ時間",	&state.Time,		0.1f, 0.0f, 180.0f );
			ImGui::Checkbox(	u8"減衰するか",	&state.IsAttenuation );
			if( i < 2 ){
				if( ImGui::Button(	u8"現在の位置を基底座標として設定" ) ){
					state.BasePosition = m_CameraState.Position;
				}
				PositionDraw( u8"Position", state.BasePosition );
			} else {
				if( ImGui::Button(	u8"現在の視点位置を基底座標として設定" ) ){
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
// 再生の表示.
//-----------------------------.
void CCameraEditor::PlayDraw()
{
	if( ImGui::Button( u8"再生" ) && m_IsCameraPlaying == false ){
		if( m_pMovieMoveCamera	== nullptr ) return;
		if( m_pMovieShakeCamera	== nullptr ) return;
		m_IsCameraPlaying	= true;
		m_pCamera->SetCameraState( { *m_pMovieMoveCamera, *m_pMovieShakeCamera } );
		m_pCamera->Play();
		CCameraManager::ChangeCamera( m_pCamera.get() );
	}
	ImGui::SameLine();
	CImGuiManager::HelpMarker( 
		u8"ボタンを押すことで 現在選択しているカメラ情報の動きを確認することができる\n"
	);
}

//-----------------------------.
// 座標の表示.
//-----------------------------.
void CCameraEditor::PositionDraw( const char* c, const D3DXVECTOR3& pos )
{
	std::string msg = c;
	msg += "\n x : %f\n y : %f\n z : %f\n";
	ImGui::Text( msg.c_str(), pos.x, pos.y, pos.z );
}