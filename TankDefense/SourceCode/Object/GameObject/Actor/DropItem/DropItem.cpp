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

// 初期化関数.
bool CDropItem::Init()
{
	InitCollision();
	return true;
}

// 更新関数.
void CDropItem::Update( const float& deltaTime )
{
	UpdateCollision();
}

// 描画関数.
void CDropItem::Render()
{
	if ( m_Disp == false ) return;

	// アイテムの表示.
	m_pStaticMesh->SetPosition( m_Tranceform.Position );
	m_pStaticMesh->Render();

	//// アイテム名をビルボードで表示.
	//SSpriteRenderState ConfuSprite	= m_pBillboardSprite->GetRenderState();
	//ConfuSprite.Tranceform.Position = { m_Tranceform.Position.x, m_Tranceform.Position.y + 7.0f, m_Tranceform.Position.z };
	//ConfuSprite.Tranceform.Scale	= { 30.0f, 30.0f, 30.0f };
	//m_pBillboardSprite->Render3D( true, &ConfuSprite );
}

// Sprite3D描画関数.
void CDropItem::Sprite3DRender()
{
	if ( m_Disp == false ) return;

	// アイテム名をビルボードで表示.
	SSpriteRenderState ConfuSprite	= m_pBillboardSprite->GetRenderState();
	ConfuSprite.Tranceform.Position = { m_Tranceform.Position.x, m_Tranceform.Position.y + 7.0f, m_Tranceform.Position.z };
	ConfuSprite.Tranceform.Scale	= { 30.0f, 30.0f, 30.0f };

	m_pBillboardSprite->Render3D( true, &ConfuSprite );
}

// 当たり判定関数.
void CDropItem::Collision( CActor* pActor )
{
	if ( m_Disp == false ) return;
	if ( pActor->GetObjectTag() != EObjectTag::Player ) return;

	// 当たっていない場合.
	if ( coll::IsSphereToSphere(
		m_pCollisions->GetCollision<CSphere>(),
		pActor->GetCollision<CSphere>() ) == false ) return;

	// 武器を入手.
	m_pPlayer->ChangeWeapon( m_pGetWeapom );
	m_Disp = false;
}

// 当たり判定の初期化.
void CDropItem::InitCollision()
{
	m_pCollisions = std::make_unique<CCollisions>();
	m_pCollisions->InitCollision( ECollNo::Sphere );
	m_pCollisions->GetCollision<CSphere>()->SetRadius( 1.5f );
}

// 当たり判定の座標や、半径などの更新.
void CDropItem::UpdateCollision()
{
	m_pCollisions->GetCollision<CSphere>()->SetPosition( m_Tranceform.Position );
}

// ドロップ関数関数.
void CDropItem::Drop( D3DXVECTOR3 DropPos )
{
	// 指定した場所で表示する.
	m_Tranceform.Position	= DropPos;
	m_Disp					= true;
}
