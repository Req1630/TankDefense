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

// ì¬ŠÖ”.
void CDropMissile::Create()
{
	// ƒ‚ƒfƒ‹‚Ìİ’è.
	m_pStaticMesh		= CMeshResorce::GetStatic( "item_tadan" );
	m_pBillboardSprite	= CSpriteResource::GetSprite( "MissileName" );

	// æ“¾•Ší‚Ìİ’è.
	m_pGetWeapom		= std::make_shared<CMissile>();
}