#include "DropItem.h"
#include "..\..\..\..\Common\D3DX\D3DX11.h"
#include "..\..\..\..\Utility\Input\Input.h"
#include "..\..\..\..\Common\Mesh\RayMesh\RayMesh.h"
#include "..\..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\Actor\Player\Player.h"

CDropItem::CDropItem()
	: m_pStaticMesh			( nullptr )
	, m_pPlayer				( nullptr )
	, m_Disp				()
	, m_pGetWeapom			( nullptr )
	, m_pBillboardSprite	( nullptr )
{
	m_ObjectTag = EObjectTag::DropItem;
}


CDropItem::~CDropItem()
{
}

// �������֐�.
bool CDropItem::Init()
{
	InitCollision();
	return true;
}

// �X�V�֐�.
void CDropItem::Update( const float& deltaTime )
{
	UpdateCollision();
}

// �`��֐�.
void CDropItem::Render()
{
	if ( m_Disp == false ) return;

	// �A�C�e���̕\��.
	m_pStaticMesh->SetPosition( m_Tranceform.Position );
	m_pStaticMesh->Render();

	//// �A�C�e�������r���{�[�h�ŕ\��.
	//SSpriteRenderState ConfuSprite	= m_pBillboardSprite->GetRenderState();
	//ConfuSprite.Tranceform.Position = { m_Tranceform.Position.x, m_Tranceform.Position.y + 7.0f, m_Tranceform.Position.z };
	//ConfuSprite.Tranceform.Scale	= { 30.0f, 30.0f, 30.0f };
	//m_pBillboardSprite->Render3D( true, &ConfuSprite );
}

// Sprite3D�`��֐�.
void CDropItem::Sprite3DRender()
{
	if ( m_Disp == false ) return;

	// �A�C�e�������r���{�[�h�ŕ\��.
	SSpriteRenderState ConfuSprite	= m_pBillboardSprite->GetRenderState();
	ConfuSprite.Tranceform.Position = { m_Tranceform.Position.x, m_Tranceform.Position.y + 7.0f, m_Tranceform.Position.z };
	ConfuSprite.Tranceform.Scale	= { 30.0f, 30.0f, 30.0f };

	m_pBillboardSprite->Render3D( true, &ConfuSprite );
}

// �����蔻��֐�.
void CDropItem::Collision( CActor* pActor )
{
	if ( m_Disp == false ) return;
	if ( pActor->GetObjectTag() != EObjectTag::Player ) return;

	// �������Ă��Ȃ��ꍇ.
	if ( coll::IsSphereToSphere(
		m_pCollisions->GetCollision<CSphere>(),
		pActor->GetCollision<CSphere>() ) == false ) return;

	// ��������.
	m_pPlayer->ChangeWeapon( m_pGetWeapom );
	m_Disp = false;
}

// �����蔻��̏�����.
void CDropItem::InitCollision()
{
	m_pCollisions = std::make_unique<CCollisions>();
	m_pCollisions->InitCollision( ECollNo::Sphere );
	m_pCollisions->GetCollision<CSphere>()->SetRadius( 1.5f );
}

// �����蔻��̍��W��A���a�Ȃǂ̍X�V.
void CDropItem::UpdateCollision()
{
	m_pCollisions->GetCollision<CSphere>()->SetPosition( m_Tranceform.Position );
}

// �h���b�v�֐��֐�.
void CDropItem::Drop( D3DXVECTOR3 DropPos )
{
	// �w�肵���ꏊ�ŕ\������.
	m_Tranceform.Position	= DropPos;
	m_Disp					= true;
}
