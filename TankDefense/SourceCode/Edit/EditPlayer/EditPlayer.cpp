#include "EditPlayer.h"
#include "..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"
#include "..\..\Object\CameraBase\RotLookAtCenter\RotLookAtCenter.h"
#include "..\..\Object\CameraBase\CameraManager\CameraManager.h"
#include "..\..\Utility\Input\Input.h"

namespace
{
	constexpr char	MESH_NAME[]			= "a_s";
	constexpr float ACTOR_ROT_SPEED		= 0.1f;
	constexpr float	CAMERA_HEIGHT		= 10.0f;
	constexpr float	CAMERA_LENGTH		= 20.0f;
	constexpr float	CAMERA_LOOK_HEIGHT	= 7.0f;
	constexpr float	CAMERA_ZOOM_SPEED	= 0.2f;
	constexpr float	CAMERA_ZOOM_MAX		= 100.0f;
	constexpr float	CAMERA_ZOOM_MIN		= 1.0f;
};

CEditPlayer::CEditPlayer()
	: m_pLookCamera		( std::make_unique<CRotLookAtCenter>() )
	, m_pSkinMesh		( nullptr )
	, m_MoveVector		( 0.0f, 0.0f, 0.0f )
	, m_PutTranceform	()
	, m_MoveSpeed		( 0.2f )
	, m_CameraMoveSpeed	( 0.05f )
	, m_CameraLength	( CAMERA_LENGTH )
	, m_IsPut			( false )
	, m_IsRotController	( true )
{
}

CEditPlayer::~CEditPlayer()
{
}

//-------------------------------------.
// 初期化関数.
//-------------------------------------.
bool CEditPlayer::Init()
{
	if( mesh::AttachSkinMesh( &m_pSkinMesh, MESH_NAME ) == false ) return false;

	m_pLookCamera->SetHeight( CAMERA_HEIGHT );
	m_pLookCamera->SetRightMoveSpeed( m_CameraMoveSpeed );
	m_pLookCamera->SetLeftMoveSpeed( -m_CameraMoveSpeed );

	Update();
	return true;
}

//-------------------------------------.
// 更新関数.
//-------------------------------------.
void CEditPlayer::Update()
{
	CameraController();		// カメラ操作.
	MoveController();		// 移動操作.
	RotationController();	// 回転操作.
	PutController();		// 置く操作.
	PutPositionUpdate();	// 配置座標の更新.

	//カメラを設定.
	m_pLookCamera->SetLength( m_CameraLength );
	D3DXVECTOR3 lookPos = m_Tranceform.Position;
	lookPos.y += CAMERA_LOOK_HEIGHT;
	m_pLookCamera->RotationLookAtObject( lookPos, 0.1f, true );
	CCameraManager::ChangeCamera( m_pLookCamera.get() );
}

//-------------------------------------.
// 描画関数.
//-------------------------------------.
void CEditPlayer::Render()
{
	m_pSkinMesh->SetAnimSpeed( GetDeltaTime() );
	m_pSkinMesh->SetTranceform( m_Tranceform );
	m_pSkinMesh->SetRasterizerState( ERS_STATE::Wire );
	m_pSkinMesh->Render();
	m_pSkinMesh->SetRasterizerState( ERS_STATE::None );
}

//-------------------------------------.
// カメラ操作.
//-------------------------------------.
void CEditPlayer::CameraController()
{
	const float axisX = CInput::GetAxisValue(EAxisBind::R_Right);
	const float axisY = CInput::GetAxisValue(EAxisBind::R_Forward);
	if( axisX < -INPUT_AXIS_DEAD_ZONE ) m_pLookCamera->DegreeHorizontalLeftMove();
	if( axisX >  INPUT_AXIS_DEAD_ZONE ) m_pLookCamera->DegreeHorizontalRightMove();
	if( axisY < -INPUT_AXIS_DEAD_ZONE ) m_pLookCamera->DegreeVerticalMove( m_CameraMoveSpeed );
	if( axisY >  INPUT_AXIS_DEAD_ZONE ) m_pLookCamera->DegreeVerticalMove( -m_CameraMoveSpeed );

	const BYTE leftTrigger	= CXInput::GetLeftTrigger();
	const BYTE rihgtTrigger	= CXInput::GetRightTrigger();
	if( rihgtTrigger	>= IDLE_TIGGER_MAX )	m_CameraLength += CAMERA_ZOOM_SPEED;
	if( leftTrigger		>= IDLE_TIGGER_MAX )	m_CameraLength -= CAMERA_ZOOM_SPEED;
	if( CKeyInput::IsHold('Z') )	m_CameraLength += CAMERA_ZOOM_SPEED;
	if( CKeyInput::IsHold('X') )	m_CameraLength -= CAMERA_ZOOM_SPEED;

	if( m_CameraLength >= CAMERA_ZOOM_MAX )		m_CameraLength = CAMERA_ZOOM_MAX;
	if( m_CameraLength <= CAMERA_ZOOM_MIN )		m_CameraLength = CAMERA_ZOOM_MIN;
}

//-------------------------------------.
// 移動操作.
//-------------------------------------.
void CEditPlayer::MoveController()
{
	if( CInput::IsHold( EKeyBind::Edit_Up ) ) m_Tranceform.Position.y += 0.1f;
	if( CInput::IsHold( EKeyBind::Edit_Down ) ) m_Tranceform.Position.y -= 0.1f;

	m_MoveVector.x = CInput::GetAxisValue( EAxisBind::L_Right );
	m_MoveVector.z = CInput::GetAxisValue( EAxisBind::L_Forward );

	if( D3DXVec3Length(&m_MoveVector) <= 0.0f ) return;

	m_Tranceform.Rotation.y = atan2f( m_MoveVector.x, m_MoveVector.z ) + m_pLookCamera->GetRadianX();
	//移動ベクトルの更新.
	m_MoveVector.x = sinf( m_Tranceform.Rotation.y );
	m_MoveVector.z = cosf( m_Tranceform.Rotation.y );
	m_Tranceform.Position -= m_MoveVector * m_MoveSpeed;
}

//-------------------------------------.
// 回転操作.
//-------------------------------------.
void CEditPlayer::RotationController()
{
	if( m_IsRotController == false ) return;

	if( CInput::IsHold( EKeyBind::Edit_RightRot ) )	m_PutTranceform.Rotation.y += ACTOR_ROT_SPEED;
	if( CInput::IsHold( EKeyBind::Edit_LeftRot ) )	m_PutTranceform.Rotation.y -= ACTOR_ROT_SPEED;

	if( m_PutTranceform.Rotation.y < 0.0f ) 
		m_PutTranceform.Rotation.y = static_cast<float>(D3DXToRadian(360.0));
	if( m_PutTranceform.Rotation.y > static_cast<float>(D3DXToRadian(360.0)) ) 
		m_PutTranceform.Rotation.y = 0.0f;
}

//-------------------------------------.
// 置く操作.
//-------------------------------------.
void CEditPlayer::PutController()
{
	m_IsPut = CInput::IsMomentPress( EKeyBind::Edit_ActorPut );
}

//-------------------------------------.
// 配置座標の更新.
//-------------------------------------.
void CEditPlayer::PutPositionUpdate()
{
	const float leght = 2.0f;
	m_PutTranceform.Position = m_Tranceform.Position;
//	m_PutTranceform.Position.x -= sinf( m_Tranceform.Rotation.y ) * leght;
//	m_PutTranceform.Position.z -= cosf( m_Tranceform.Rotation.y ) * leght;

#if 0
	auto set = []( float* pPos )
	{
		int v = static_cast<int>(*pPos);
		*pPos = static_cast<float>(v);
	};
	set( &m_PutPosition.x );
	set( &m_PutPosition.y );
	set( &m_PutPosition.z );
#endif
	

}