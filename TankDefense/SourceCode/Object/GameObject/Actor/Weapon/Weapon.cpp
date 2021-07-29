#include "Weapon.h"
#include "..\..\..\..\Common\Mesh\DX9StaticMesh\DX9StaticMesh.h"
#include "..\..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\..\Object\GameObject\Actor\Bullet\Bullet.h"

CWeapon::CWeapon()
	: m_pStaticMesh	( nullptr )
	, m_pBullet		()
	, m_Type		( EType::DefaultWepon )
	, m_ListSize	()
	, m_Status		()
	, m_ShotCnt		()
{
}

CWeapon::~CWeapon()
{
}

// 初期化関数.
bool CWeapon::Init()
{
	// 必要な配列数を求める.
	m_ListSize = m_Status.BulletsNum - m_Status.BulletsNum / static_cast<int>( DISP_TIME / m_Status.Interval ) + 1;

	// 弾数分配列を用意.
	m_pBullet.resize( m_ListSize );

	// 弾の設定.
	for ( int i = 0; i < m_ListSize; i++ ){
		m_pBullet[i] = std::make_unique<CBullet>();
	}
	return true;
}

// 更新関数.
void CWeapon::Update( const float& deltaTime )
{
	// 弾の更新.
	if ( m_ShotCnt < m_Status.Interval ) m_ShotCnt += deltaTime;
	for ( int i = 0; i < m_ListSize; i++ ){
		m_pBullet[i]->Update( deltaTime );
	}
	UpdateCollision();
}

// 描画関数.
void CWeapon::Render()
{
	// モデルの描画.
	m_pStaticMesh->Render();

	// 弾の描画.
	for ( int i = 0; i < m_ListSize; i++ ){
		m_pBullet[i]->Render();
	}
}

// 当たり判定関数.
void CWeapon::Collision( CActor * pActor )
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

void CWeapon::Shot( D3DXVECTOR3 MoveVec )
{
	int cnt = 0;
	// 弾間隔分間隔が空いていたら.
	if ( m_ShotCnt >= m_Status.Interval ){
		for ( int i = 0; i < m_ListSize; i++ ){
			if ( m_pBullet[i]->GetDisp() == false ){
				m_ShotCnt = 0.0f;

				// ステータスの設定.
				m_pBullet[i]->SetStatus( m_Status );

				// 弾の発射.
				if ( m_Type == EType::Missile )
					m_pBullet[i]->HomingShot( m_Tranceform.Position, D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );
				else
					m_pBullet[i]->NormalShot( m_Tranceform.Position, MoveVec );

				// 弾数を減らす.
				if ( m_Type != EType::DefaultWepon ){
					m_Status.BulletsNum--;
					if ( m_Status.BulletsNum <= 0 ){
						/*
						//	武器を主砲に戻す処理を入れる.
						*/
					}
				}
				cnt++;

				if( m_Type != EType::Missile || cnt >= 8 ) break;
			}
		}
	}
}

// 移動.
void CWeapon::Move( D3DXVECTOR3 Pos, D3DXVECTOR3 Rot )
{
	// モデルの移動.
	m_pStaticMesh->SetPosition( Pos );
	m_Tranceform.Position = Pos;

	// モデルを回転させる.
	m_pStaticMesh->SetRotation( Rot );
	m_Tranceform.Rotation = Rot;
}
