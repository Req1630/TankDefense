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

// �������֐�.
bool CRestraint::Init()
{
	m_pStaticMesh = CMeshResorce::GetStatic( "Restraint" );
	m_pStaticMesh->SetAlpha( 0.5f );
	m_RestraintCnt = 0;
	m_IsRestraint = false;

	InitCollision();
	return true;
}

// �X�V�֐�.
void CRestraint::Update( const float & deltaTime )
{
	UpdateCollision();
}

// �`��֐�.
void CRestraint::Render()
{
	m_pStaticMesh->SetBlend( true );
	if ( m_IsRestraint == true ) m_pStaticMesh->Render();
	m_pStaticMesh->SetBlend( false );
}

// �����蔻��֐�.
inline void CRestraint::Collision( CActor * pActor )
{
}

// �����蔻��̏�����.
void CRestraint::InitCollision()
{
	m_pCollisions = std::make_unique<CCollisions>();
	m_pCollisions->InitCollision( ECollNo::Sphere );
	m_pCollisions->GetCollision<CSphere>()->SetRadius( 1.0f );
}

// �����蔻��̍��W��A���a�Ȃǂ̍X�V.
//	Update�֐��̍Ō�ɌĂ�.
void CRestraint::UpdateCollision()
{
	m_pCollisions->GetCollision<CSphere>()->SetPosition( m_Tranceform.Position );
}

// �����̈ʒu�ݒ�.
void CRestraint::SetRestraintPos( D3DXVECTOR3 pos, D3DXVECTOR3 movevec )
{
	m_Vec3 = movevec;
	m_Tranceform.Position = pos - ( movevec * 5.0f );
	m_Tranceform.Position.y += 4.0f;
	m_pStaticMesh->SetPosition( m_Tranceform.Position );
}
