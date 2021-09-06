#include "DropMachineGun.h"
#include "..\..\..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\Weapon\MachineGun\MachineGun.h"

CDropMachineGun::CDropMachineGun()
{
	Create();
	Init();
}

CDropMachineGun::~CDropMachineGun()
{
}

// ì¬ŠÖ”.
void CDropMachineGun::Create()
{
	// ƒ‚ƒfƒ‹‚Ìİ’è.
	m_pStaticMesh		= CMeshResorce::GetStatic( "item_machingun" );
	m_pBillboardSprite	= CSpriteResource::GetSprite( "MachineGunName" );

	// æ“¾•Ší‚Ìİ’è.
	m_pGetWeapom		= std::make_shared<CMachineGun>();
}