#include "Weapon.h"
#include "..\..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"
#include "..\..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\..\Object\GameObject\Actor\Weapon\Bullet\BulletManager\BulletManager.h"

CWeapon::CWeapon()
	: m_pBulletMng			()
	, m_ShotCnt				()
	, m_pSkinMesh_Body		( nullptr )
	, m_pSkinMesh_Weapon	( nullptr )
	, m_Type				( EType::DefaultWepon )
	, m_Status				()
	, m_BoneName			()
{
}

CWeapon::~CWeapon()
{
}

// 初期化関数.
bool CWeapon::Init()
{
	InitCollision();
	return true;
}

// 更新関数.
void CWeapon::Update( const float& deltaTime )
{
	// 弾の更新.
	if ( m_ShotCnt < m_Status.Interval ) m_ShotCnt += deltaTime;
	DebugUpdate();
	UpdateCollision();
}

// 描画関数.
void CWeapon::Render()
{
	if ( m_pSkinMesh_Body	== nullptr ) return;
	if ( m_pSkinMesh_Weapon == nullptr ) return;

	// モデルの描画.
	m_pSkinMesh_Body->SetTranceform( m_Tranceform );
	m_pSkinMesh_Body->Render();
	m_pSkinMesh_Weapon->Render();
}

// 当たり判定関数.
void CWeapon::Collision( CActor* pActor )
{
}

// 当たり判定の初期化.
void CWeapon::InitCollision()
{
}

// 当たり判定の座標や、半径などの更新.
void CWeapon::UpdateCollision()
{
}

// 弾の発射関数.
//	※弾が無くなった場合"false"を返す.
bool CWeapon::Shot( D3DXVECTOR3 MoveVec )
{
	D3DXVECTOR3 ShotPos = m_Tranceform.Position;
	m_pSkinMesh_Weapon->GetPosFromBone( m_ShotName.c_str(), &ShotPos );

	// 弾間隔分間隔が空いていたら.
	if ( m_ShotCnt >= m_Status.Interval ){
		m_ShotCnt = 0.0f;

		//同時に発射する弾の数.
		int ShotNum = 1;
		if ( m_Type == EType::Missile ) ShotNum = 8;

		for ( int i = 0; i < ShotNum; i++ ){
			// 弾の発射.
			if ( m_Type == EType::Missile )
				m_pBulletMng->HomingShot( m_BulletName, EObjectTag::PlayerBullet, m_Status, ShotPos, D3DXVECTOR3( 0.0f, 0.0f, 0.0f ), m_Tranceform.Rotation );
			else
				m_pBulletMng->NormalShot( m_BulletName, EObjectTag::PlayerBullet, m_Status, ShotPos, m_Tranceform.Rotation, MoveVec );

			// 弾数を減らす.
			if ( m_Type != EType::DefaultWepon ){
				m_Status.BulletsNum--;

				// 武器の変更のお知らせ.
				if ( m_Status.BulletsNum <= 0 ) return false;
			}
		}
	}
	return true;
}

// 移動関数.
void CWeapon::Move( D3DXVECTOR3 BodyPos, D3DXVECTOR3 Rot )
{
	// 移動、回転.
	m_Tranceform.Position = BodyPos;
	m_Tranceform.Rotation = Rot;

	// 座標の設定.
	D3DXVECTOR3 BonePos = m_Tranceform.Position;
	m_pSkinMesh_Body->GetPosFromBone( m_BoneName.c_str(), &BonePos );

	// モデルに反映させる.
	if ( m_pSkinMesh_Body == nullptr ) return;
	m_pSkinMesh_Body->SetPosition( BodyPos );
	m_pSkinMesh_Body->SetRotation( Rot );
	if ( m_pSkinMesh_Weapon == nullptr ) return;
	m_pSkinMesh_Weapon->SetPosition( BonePos );
	m_pSkinMesh_Weapon->SetRotation( Rot );
}

// デバックの更新関数.
void CWeapon::DebugUpdate()
{
	CDebugText::PushText( "Bullet", "------------------" );
	CDebugText::PushText( "Bullet", "----  Bullet  ----" );
	CDebugText::PushText( "Bullet", "------------------" );
	CDebugText::PushText( "Bullet", "Type             : ", m_Type );
	CDebugText::PushText( "Bullet", "BulletsNum       : ", m_Status.BulletsNum );
	CDebugText::PushText( "Bullet", "Attack           : ", m_Status.Attack );
	CDebugText::PushText( "Bullet", "Speed            : ", m_Status.Speed );
	CDebugText::PushText( "Bullet", "Interval         : ", m_Status.Interval );
	CDebugText::PushText( "Bullet", "ShotCnt          : ", m_ShotCnt );
	CDebugText::PushText( "Bullet", "------------------" );
}