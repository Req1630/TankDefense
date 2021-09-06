#include "Player.h"
#include <DirectXMath.h>	// XMConvertToRadians()で必要.
#include "..\..\..\..\Utility\Input\Input.h"
#include "..\..\..\..\Common\Mesh\RayMesh\RayMesh.h"
#include "..\..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"
#include "..\..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\..\Object\CameraBase\CameraManager\CameraManager.h"
#include "..\..\..\..\Object\CameraBase\RotLookAtCenter\RotLookAtCenter.h"
#include "..\Weapon\Weapon.h"
#include "..\Weapon\DefaultWepon\DefaultWepon.h"
#include "..\Weapon\MachineGun\MachineGun.h"
#include "..\Weapon\Missile\Missile.h"
#include "..\Weapon\Beam\Beam.h"

namespace{
	const float ROT_REST_RANG	= DirectX::XMConvertToRadians( 60.0f );	// 回転の制限角度(度).
}

CPlayer::CPlayer()
	: CPlayer( nullptr )
{
}

CPlayer::CPlayer( const std::shared_ptr<CBulletManager> pBulletMng )
	: m_pSkinMesh		( nullptr )
	, m_pLookCamera		( nullptr )
	, m_pBulletMng		( pBulletMng )
	, m_pWeapon			( nullptr )
	, m_pDefaultWepon	( nullptr )
	, m_MoveVec			()
	, m_DireVec			()
	, m_AlignmentPos	()
	, m_Status			()
{
	m_ObjectTag = enObjectTagList::Player;

	Create();
	Init();
}

CPlayer::~CPlayer()
{
}

// 初期化関数.
bool CPlayer::Init()
{
	// 初期化.
	m_MoveVec		= { 0.0f, 0.0f, 0.0f };
	m_DireVec		= { 0.0f, 0.0f, 0.0f };
	m_AlignmentPos	= { 0.0f, 0.0f, 0.0f };

	// 主砲をセット.
	m_pWeapon		= m_pDefaultWepon;

	// ステータスの設定.
	m_Status.Hp		= 100.0f;
	m_Status.Speed	= 0.2f;

	InitCollision();
	return true;
}

// 更新関数.
void CPlayer::Update( const float& deltaTime )
{
	Controller();					// 操作.
	CameraController();				// カメラの操作.
	AttackController();				// 攻撃操作.

	Move();							// 移動.
	CameraUpdate();					// カメラの更新.

	m_pWeapon->Update( deltaTime );	// 武器の更新.
	DebugUpdate();					// デバックの更新.

	UpdateCollision();
}

// 描画関数.
void CPlayer::Render()
{
	// モデルの描画.
	m_pSkinMesh->SetTranceform( m_Tranceform );
	m_pSkinMesh->SetShadowDepth( 0.1f );	// ←の関数で自分自身にかける影の濃さを設定できる.
	m_pSkinMesh->Render();
	m_pWeapon->Render();
}

// 当たり判定関数.
void CPlayer::Collision( CActor* pActor )
{
}

// 当たり判定の初期化.
void CPlayer::InitCollision()
{
	m_pCollisions = std::make_unique<CCollisions>();
	m_pCollisions->InitCollision( ECollNo::Sphere );
	m_pCollisions->GetCollision<CSphere>()->SetRadius( 1.5f );
}

// 当たり判定の座標や、半径などの更新.
void CPlayer::UpdateCollision()
{
	m_pCollisions->GetCollision<CSphere>()->SetPosition( m_Tranceform.Position );
}

// 作成関数.
void CPlayer::Create()
{
	// カメラの設定.
	m_pLookCamera	= std::make_unique<CRotLookAtCenter>();
	CCameraManager::ChangeCamera( m_pLookCamera.get() );
	m_pLookCamera	->SetHeight( 10.0f );

	// 武器の設定.
	m_pDefaultWepon = std::make_shared<CDefaultWepon>();
	m_pDefaultWepon	->SetBulletMng( m_pBulletMng );

	// スタティックメッシュの取得.
	m_pSkinMesh		= CMeshResorce::GetSkin( "syatai_s" );
	m_pSkinMesh->SetAnimSpeed( GetDeltaTime<double>() );
}

// 操作関数.
void CPlayer::Controller()
{
	// 向きベクトルの更新.
	m_DireVec.x = sinf( m_CameraRot.y );
	m_DireVec.z = cosf( m_CameraRot.y );

	// 移動ベクトルの更新.
	m_MoveVec = { 0.0f, 0.0f, 0.0f };
	if ( CKeyInput::IsPress( VK_UP		) ) m_MoveVec =  m_DireVec;
	if ( CKeyInput::IsPress( VK_DOWN	) ) m_MoveVec = -m_DireVec;
}

// カメラの操作関数.
void CPlayer::CameraController()
{
	if ( CKeyInput::IsPress( VK_LEFT	) ) m_pLookCamera->DegreeHorizontalLeftMove();
	if ( CKeyInput::IsPress( VK_RIGHT	) ) m_pLookCamera->DegreeHorizontalRightMove();
}

// 攻撃関数.
void CPlayer::AttackController()
{
	// 弾の発射.
	if ( CKeyInput::IsPress( VK_SPACE ) ){
		if ( m_pWeapon->Shot( m_DireVec ) == false ){
			// 武器を主砲に戻す.
			ChangeWeapon( m_pDefaultWepon );
		};
	}
}

// 移動関数.
void CPlayer::Move()
{
	D3DXVECTOR3* Pos = &m_Tranceform.Position;
	D3DXVECTOR3* Rot = &m_Tranceform.Rotation;

	// 移動ベクトルで移動.
	*Pos -= m_MoveVec * m_Status.Speed;

	// ボーン座標を取得.
	D3DXVECTOR3 BoneBodyPos		= m_Tranceform.Position;
	m_pSkinMesh->GetPosFromBone( "tyuusinjiku", &BoneBodyPos );

	// 武器の移動.
	m_pWeapon->Move( BoneBodyPos, m_CameraRot );
	
	// 移動している場合
	const float WRotY		= m_pWeapon->GetRotation().y;
	const float CMoveSpd	= m_pLookCamera->GetMoveSpeed();
	if ( m_MoveVec != D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) ){
		// カメラの回転速度の2倍の速度で回転し向きを揃える.
		if ( Rot->y > WRotY ) Rot->y -= CMoveSpd * 2.0f;
		if ( Rot->y < WRotY ) Rot->y += CMoveSpd * 2.0f;
	}
	// 停止いている場合.
	else{
		if ( Rot->y - WRotY >= ROT_REST_RANG )
			Rot->y -= CMoveSpd;
		else if ( Rot->y - WRotY <= -ROT_REST_RANG )
			Rot->y += CMoveSpd;
	}
}

// カメラの更新関数.
void CPlayer::CameraUpdate()
{
	// カメラを設定.
	m_pLookCamera->RotationLookAtObject( m_Tranceform.Position, m_Status.Speed );

	// 上下.
	m_pLookCamera->SetLookPosition( m_Tranceform.Position + m_AlignmentPos );

	// カメラの回転値の更新.
	m_CameraRot = D3DXVECTOR3( 0.0f, m_pLookCamera->GetRadianX(), 0.0f );
}

// デバックの更新関数.
void CPlayer::DebugUpdate()
{
#ifdef _DEBUG
	const D3DXVECTOR3 Pos = m_Tranceform.Position;
	const D3DXVECTOR3 Rot = m_Tranceform.Rotation;
	CDebugText::PushText( "Player", "------------------" );
	CDebugText::PushText( "Player", "----  Player  ----" );
	CDebugText::PushText( "Player", "------------------" );
	CDebugText::PushText( "Player", "L_Ctrl + " );
	CDebugText::PushText( "Player", "'1' GetDefaultWepon, '2' GetMachineGun, '3' GetMissile, '4' GetBeam" );
	CDebugText::PushText( "Player", "'U' AlignmentPos_UP, 'D' AlignmentPos_Down" );
	CDebugText::PushText( "Player", "-------------------" );
	CDebugText::PushText( "Player", "Pos     : ", Pos.x, ", ", Pos.y, ", ", Pos.z );
	CDebugText::PushText( "Player", "Ros     : ", Rot.x, ", ", Rot.y, ", ", Rot.z );
	CDebugText::PushText( "Player", "MoveVec : ", m_MoveVec.x, ", ", m_MoveVec.y, ", ", m_MoveVec.z );
	CDebugText::PushText( "Player", "DireVec : ", m_DireVec.x, ", ", m_DireVec.y, ", ", m_DireVec.z );
	CDebugText::PushText( "Player", "Weapon  : ", m_pWeapon->GetObjectTag() == EObjectTag::DefaultWepon ? "DefaultWepon" : m_pWeapon->GetObjectTag() == EObjectTag::MachineGun ? "MachineGun" : m_pWeapon->GetObjectTag() == EObjectTag::Missile ? "Missile" : "Beam" );
	CDebugText::PushText( "Player", "HP      : ", m_Status.Hp );
	CDebugText::PushText( "Player", "Speed   : ", m_Status.Speed );

	if ( !CKeyInput::IsPress( VK_LCONTROL ) ) return;
	if ( CKeyInput::IsMomentPress( '1' ) ) ChangeWeapon( m_pDefaultWepon	);

	if ( CKeyInput::IsPress( 'U' ) ) m_AlignmentPos.y += 0.1f;
	if ( CKeyInput::IsPress( 'D' ) ) m_AlignmentPos.y -= 0.1f;
#endif
}

// 武器の変更関数.
void CPlayer::ChangeWeapon( const std::shared_ptr<CWeapon> pWeapon )
{
	// 武器の変更.
	m_pWeapon = pWeapon;
	m_pWeapon->SetBulletMng( m_pBulletMng );

	// 武器の初期化.
	m_pWeapon->Init();
}
