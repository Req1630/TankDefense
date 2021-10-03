#include "Explosion.h"
#include "..\..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"
#include "..\..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\..\..\Resource\MeshResource\MeshResource.h"

CExplosion::CExplosion()
	: m_pStaticMesh		( nullptr )
	, m_ExplosionCnt	( 0 )
	, m_IsExplosion		( false )

{
	Init();
}

CExplosion::~CExplosion()
{
}

// 初期化関数.
bool CExplosion::Init()
{
	m_pStaticMesh = CMeshResorce::GetStatic( "Explosion" );
	m_pStaticMesh->SetAlpha( 0.2f );
	m_ExplosionCnt = 0;
	m_IsExplosion = false;

	InitCollision();
	return true;
}

// 更新関数.
void CExplosion::Update( const float & deltaTime )
{
	// 爆発したとき.
	if ( m_IsExplosion == true ) m_ExplosionCnt++;
	// 爆発から2秒経ったら爆発を消す.
	if ( m_ExplosionCnt < 2.0*FPS ) return;
	m_IsExplosion = false;	// 爆発を消す.
	m_ExplosionCnt = 0;		// 爆発カウントを初期化.

	UpdateCollision();
}

// 描画関数.
void CExplosion::Render()
{
	m_pStaticMesh->SetBlend( true );
	if ( m_IsExplosion == true ) m_pStaticMesh->Render();
	m_pStaticMesh->SetBlend( false );
}

// 当たり判定関数.
inline void CExplosion::Collision( CActor * pActor )
{
}

// 当たり判定の初期化.
void CExplosion::InitCollision()
{
	m_pCollisions = std::make_unique<CCollisions>();
	m_pCollisions->InitCollision( ECollNo::Sphere );
	m_pCollisions->GetCollision<CSphere>()->SetRadius( 8.5f );
}

// 当たり判定の座標や、半径などの更新.
//	Update関数の最後に呼ぶ.
void CExplosion::UpdateCollision()
{
	m_pCollisions->GetCollision<CSphere>()->SetPosition( m_Tranceform.Position );
}

// 爆発の位置設定.
void CExplosion::SetExplosionPos( D3DXVECTOR3 pos )
{
	m_Tranceform.Position = pos;
	m_Tranceform.Position.y += 4.0f;
	m_pStaticMesh->SetPosition( m_Tranceform.Position );
}
