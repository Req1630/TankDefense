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

// �������֐�.
bool CExplosion::Init()
{
	m_pStaticMesh = CMeshResorce::GetStatic( "Explosion" );
	m_pStaticMesh->SetAlpha( 0.2f );
	m_ExplosionCnt = 0;
	m_IsExplosion = false;

	InitCollision();
	return true;
}

// �X�V�֐�.
void CExplosion::Update( const float & deltaTime )
{
	// ���������Ƃ�.
	if ( m_IsExplosion == true ) m_ExplosionCnt++;
	// ��������2�b�o�����甚��������.
	if ( m_ExplosionCnt < 2.0*FPS ) return;
	m_IsExplosion = false;	// ����������.
	m_ExplosionCnt = 0;		// �����J�E���g��������.

	UpdateCollision();
}

// �`��֐�.
void CExplosion::Render()
{
	m_pStaticMesh->SetBlend( true );
	if ( m_IsExplosion == true ) m_pStaticMesh->Render();
	m_pStaticMesh->SetBlend( false );
}

// �����蔻��֐�.
inline void CExplosion::Collision( CActor * pActor )
{
}

// �����蔻��̏�����.
void CExplosion::InitCollision()
{
	m_pCollisions = std::make_unique<CCollisions>();
	m_pCollisions->InitCollision( ECollNo::Sphere );
	m_pCollisions->GetCollision<CSphere>()->SetRadius( 8.5f );
}

// �����蔻��̍��W��A���a�Ȃǂ̍X�V.
//	Update�֐��̍Ō�ɌĂ�.
void CExplosion::UpdateCollision()
{
	m_pCollisions->GetCollision<CSphere>()->SetPosition( m_Tranceform.Position );
}

// �����̈ʒu�ݒ�.
void CExplosion::SetExplosionPos( D3DXVECTOR3 pos )
{
	m_Tranceform.Position = pos;
	m_Tranceform.Position.y += 4.0f;
	m_pStaticMesh->SetPosition( m_Tranceform.Position );
}
