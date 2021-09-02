#include "Restraint.h"
#include "..\..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"
#include "..\..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\..\..\Resource\MeshResource\MeshResource.h"

CRestraint::CRestraint()
	: m_pStaticMesh		( nullptr )
	, m_Vec3			( 0.0f, 0.0f, 0.0f )
	, m_RestraintCnt	( 0 )
	, m_IsRestraint		( false )

{
	Init();
}

CRestraint::~CRestraint()
{
}

// 初期化関数.
bool CRestraint::Init()
{
	m_pStaticMesh = CMeshResorce::GetStatic( "Restraint" );
	m_pStaticMesh->SetAlpha( 0.5f );
	m_RestraintCnt = 0;
	m_IsRestraint = false;

	InitCollision();
	return true;
}

// 更新関数.
void CRestraint::Update( const float & deltaTime )
{
	UpdateCollision();
}

// 描画関数.
void CRestraint::Render()
{
	m_pStaticMesh->SetBlend( true );
	if ( m_IsRestraint == true ) m_pStaticMesh->Render();
	m_pStaticMesh->SetBlend( false );
}

// 当たり判定関数.
inline void CRestraint::Collision( CActor * pActor )
{
}

// 当たり判定の初期化.
void CRestraint::InitCollision()
{
	m_pCollisions = std::make_unique<CCollisions>();
	m_pCollisions->InitCollision( ECollNo::Sphere );
	m_pCollisions->GetCollision<CSphere>()->SetRadius( 1.0f );
}

// 当たり判定の座標や、半径などの更新.
//	Update関数の最後に呼ぶ.
void CRestraint::UpdateCollision()
{
	m_pCollisions->GetCollision<CSphere>()->SetPosition( m_Tranceform.Position );
}

// 爆発の位置設定.
void CRestraint::SetRestraintPos( D3DXVECTOR3 pos, D3DXVECTOR3 movevec )
{
	m_Vec3 = movevec;
	m_Tranceform.Position = pos - ( movevec * 5.0f );
	m_Tranceform.Position.y += 4.0f;
	m_pStaticMesh->SetPosition( m_Tranceform.Position );
}
