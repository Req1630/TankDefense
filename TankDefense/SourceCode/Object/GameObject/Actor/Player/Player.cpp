#include "Player.h"
#include <DirectXMath.h>	// XMConvertToRadians()で必要.
#include "..\..\..\..\Utility\Input\Input.h"
#include "..\..\..\..\Common\Mesh\RayMesh\RayMesh.h"
#include "..\..\..\..\Common\Mesh\DX9StaticMesh\DX9StaticMesh.h"
#include "..\..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\..\Object\CameraBase\CameraManager\CameraManager.h"
#include "..\..\..\..\Object\CameraBase\RotLookAtCenter\RotLookAtCenter.h"
#include "..\Weapon\Weapon.h"
#include "..\Weapon\DefaultWepon\DefaultWepon.h"
#include "..\Weapon\MachineGun\MachineGun.h"
#include "..\Weapon\Missile\Missile.h"

namespace{
	const float ROT_REST_DANG	= 60.0f;	// 回転の制限角度(度).
}

// ラジアン変換関数.
inline float DxToRadian( float Ang ) {
	return DirectX::XMConvertToRadians( Ang );
}

CPlayer::CPlayer()
	: m_pStaticMesh		( nullptr )
	, m_pLookCamera		( nullptr )
	, m_pWeapon			( nullptr )
	, m_pDefaultWepon	( nullptr )
	, m_pMachineGun		( nullptr )
	, m_MoveVec			()
	, m_Status			()
{
	Init();
}

CPlayer::~CPlayer()
{
}

// 初期化関数.
bool CPlayer::Init()
{
	// カメラの設定.
	m_pLookCamera	= std::make_unique<CRotLookAtCenter>();
	CCameraManager::ChangeCamera( m_pLookCamera.get() );
	m_pLookCamera->SetHeight( 10.0f );

	// 武器の設定.
	m_pDefaultWepon = std::make_shared<CDefaultWepon>();
	m_pMachineGun	= std::make_shared<CMachineGun>();
	m_pMissile		= std::make_shared<CMissile>();

	// 主砲をセット.
	m_pWeapon		= m_pMissile;

	// スタティックメッシュの取得.
	m_pStaticMesh = CMeshResorce::GetStatic( "syatai" );

	// ステータスの設定.
	m_Status.Hp		= 100.0f;
	m_Status.Speed	= 0.2f;

	return true;
}

// 更新関数.
void CPlayer::Update( const float & deltaTime )
{
	Controller();					// 操作.
	CameraController();				// カメラの操作.
	AttackController();				// 攻撃操作.

	Move();							// 移動.
	CameraUpdate();					// カメラの更新.

	m_pWeapon->Update( deltaTime );	// 武器の更新.

	if ( CKeyInput::IsMomentPress( 'N' ) )m_pWeapon = m_pDefaultWepon;
	if ( CKeyInput::IsMomentPress( 'M' ) )m_pWeapon = m_pMachineGun;

	UpdateCollision();
}

// 描画関数.
void CPlayer::Render()
{
	/* ↓↓↓↓ 一応描画する直前に座標系を設定してください ↓↓↓↓ */
	m_pStaticMesh->SetRotation( m_Tranceform.Rotation );
	m_pStaticMesh->SetPosition( m_Tranceform.Position );
//	m_pStaticMesh->SetTranceform( m_Tranceform );	// ←のようにもできる.

	// モデルの描画.
	m_pStaticMesh->SetShadowDepth( 0.1f );	// ←の関数で自分自身にかける影の濃さを設定できる.
	m_pStaticMesh->Render();
	m_pWeapon->Render();
}

// 当たり判定関数.
void CPlayer::Collision( CActor* pActor )
{
}

// 当たり判定の初期化.
void CPlayer::InitCollision()
{
}

// 当たり判定の座標や、半径などの更新.
void CPlayer::UpdateCollision()
{
}

// 操作関数.
void CPlayer::Controller()
{
	// 移動ベクトルの初期化.
	m_MoveVec			= { 0.0f, 0.0f, 0.0f };

	if ( CKeyInput::IsPress( VK_UP ) ){
		// 前進用の移動ベクトルを用意.
		m_MoveVec.x		= sinf( m_CameraRot.y );
		m_MoveVec.z		= cosf( m_CameraRot.y );
	}
	if ( CKeyInput::IsPress( VK_DOWN ) ){
		// 後進用の移動ベクトルを用意.
		m_MoveVec.x		= -sinf( m_CameraRot.y );
		m_MoveVec.z		= -cosf( m_CameraRot.y );
	}
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
	if ( CKeyInput::IsPress( VK_SPACE ) ){
		// 弾の移動ベクトルの作成.
		D3DXVECTOR3 ShotMoveVec;
		ShotMoveVec.x = sinf( m_CameraRot.y );
		ShotMoveVec.z = cosf( m_CameraRot.y );

		// 弾の発射.
		m_pWeapon->Shot( ShotMoveVec );
	}
}

// 移動関数.
void CPlayer::Move()
{
	// 移動ベクトルで移動.
	m_Tranceform.Position -= m_MoveVec * m_Status.Speed;

	/* 仮 */
	D3DXVECTOR3 BonePos = m_Tranceform.Position;
	BonePos.y += 1.2f;

	// 武器の移動.
	m_pWeapon->Move( BonePos, m_CameraRot );
	
	// 移動している場合
	if ( m_MoveVec != D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) ){
		// カメラの回転速度の2倍の速度で回転し向きを揃える.
		if ( m_Tranceform.Rotation.y > m_pWeapon->GetRotation().y )
			m_Tranceform.Rotation.y -= m_pLookCamera->GetMoveSpeed() * 2.0f;
		if ( m_Tranceform.Rotation.y < m_pWeapon->GetRotation().y ) 
			m_Tranceform.Rotation.y += m_pLookCamera->GetMoveSpeed() * 2.0f;
	}
	// 停止いている場合.
	else{
		// 移動制限移動なら車体を回転させる.
		if ( m_Tranceform.Rotation.y - m_pWeapon->GetRotation().y >= DxToRadian( ROT_REST_DANG ) )
			m_Tranceform.Rotation.y -= m_pLookCamera->GetMoveSpeed();
		else if ( m_Tranceform.Rotation.y - m_pWeapon->GetRotation().y <= DxToRadian( -ROT_REST_DANG ) )
			m_Tranceform.Rotation.y += m_pLookCamera->GetMoveSpeed();
	}
}

// カメラの更新.
void CPlayer::CameraUpdate()
{
	// カメラを設定.
	m_pLookCamera->RotationLookAtObject( m_Tranceform.Position, m_Status.Speed );

	// 上下.
	static D3DXVECTOR3 AddPos = { 0.0f, 0.0f, 0.0f };
	if ( CKeyInput::IsPress( '1' ) ) AddPos.y += 0.1f;
	if ( CKeyInput::IsPress( '2' ) ) AddPos.y -= 0.1f;
	m_pLookCamera->SetLookPosition( m_Tranceform.Position + AddPos );

	// カメラの回転値の更新.
	m_CameraRot = D3DXVECTOR3( 0.0f, m_pLookCamera->GetRadianX(), 0.0f );
}