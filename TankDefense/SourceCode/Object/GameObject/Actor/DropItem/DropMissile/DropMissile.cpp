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

// �쐬�֐�.
void CDropMissile::Create()
{
	// ���f���̐ݒ�.
	m_pStaticMesh		= CMeshResorce::GetStatic( "item_tadan" );
	m_pBillboardSprite	= CSpriteResource::GetSprite( "MissileName" );

	// �擾����̐ݒ�.
	m_pGetWeapom		= std::make_shared<CMissile>();
}