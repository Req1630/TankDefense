#include "DropMissile.h"
#include "..\..\..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\Weapon\Missile\Missile.h"

CDropMissile::CDropMissile()
{
	Create();
	Init();
}

CDropMissile::~CDropMissile()
{
}

// 作成関数.
void CDropMissile::Create()
{
	// モデルの設定.
	m_pStaticMesh		= CMeshResorce::GetStatic( "item_tadan" );
	m_pBillboardSprite	= CSpriteResource::GetSprite( "MissileName" );

	// 取得武器の設定.
	m_pGetWeapom		= std::make_shared<CMissile>();
}