#include "EnemyBullet.h"
#include "..\..\..\..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\..\..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\..\..\..\Utility\Input\Input.h"

CEnemyBullet::CEnemyBullet()
	: m_TargetPosition	( 0.0f, 0.0f, 0.0f )
	, m_ShotFlag		( false )
{
	CEnemyBullet::Init();
}

CEnemyBullet::~CEnemyBullet()
{
}

bool CEnemyBullet::Init()
{
	m_pStaticMesh = CMeshResorce::GetStatic( "Sphere" );
	m_pStaticMesh->SetScale( D3DXVECTOR3( 0.1f, 0.1f, 0.1f ) );

	InitCollision();
	return true;
}

void CEnemyBullet::Update( const float& deltaTime )
{
	float speed = 0.2f;
	float add_Scale = 0.05f;
	
	if (m_ShotFlag == true && m_Tranceform.Scale.x >= 1.0f) {
		m_Tranceform.Position -= m_MoveVec * speed;

		m_pStaticMesh->SetPosition( m_Tranceform.Position );
	}
	else {
		m_Tranceform.Scale += { 0.01f, 0.01f, 0.01f };
		m_pStaticMesh->SetScale( m_Tranceform.Scale );
	}

	UpdateCollision();
}

void CEnemyBullet::Render()
{
	if (m_ShotFlag == true) {
		m_pStaticMesh->Render();
	}
}

void CEnemyBullet::Collision( CActor* pActor )
{
}

void CEnemyBullet::InitCollision()
{
	m_pCollisions = std::make_unique<CCollisions>();
	m_pCollisions->InitCollision( ECollNo::Sphere );
	m_pCollisions->GetCollision<CSphere>()->SetRadius( 1.0f );
}

void CEnemyBullet::UpdateCollision()
{
	m_pCollisions->GetCollision<CSphere>()->SetPosition( m_Tranceform.Position );
}

// 弾の表示設定.
void CEnemyBullet::SetBulletScale()
{
	m_Tranceform.Scale = { 0.1f, 0.1f, 0.1f };
	m_pStaticMesh->SetScale( m_Tranceform.Scale );
	m_ShotFlag = true;
}

// 弾の位置設定.
void CEnemyBullet::SetBulletPos( D3DXVECTOR3 Pos, D3DXVECTOR3 MoveVec )
{
	m_MoveVec = MoveVec;
	m_Tranceform.Position = Pos - ( m_MoveVec * 2.0f );
	m_Tranceform.Position.y += 1.5f;
	m_pStaticMesh->SetPosition( m_Tranceform.Position );
}

// 相手座標の設定関数.
void CEnemyBullet::SetTargetPos( CActor& actor )
{
	// エネミーじゃなければ終了.
	if ( actor.GetObjectTag() != EObjectTag::Enemy ) return;
	//if (m_IsInAttack == true) return;
	m_TargetPosition = actor.GetPosition();	// エネミーの座標を取得.
}
