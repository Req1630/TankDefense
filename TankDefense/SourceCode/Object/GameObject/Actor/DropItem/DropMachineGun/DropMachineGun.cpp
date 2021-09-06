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

// �쐬�֐�.
void CDropMachineGun::Create()
{
	// ���f���̐ݒ�.
	m_pStaticMesh		= CMeshResorce::GetStatic( "item_machingun" );
	m_pBillboardSprite	= CSpriteResource::GetSprite( "MachineGunName" );

	// �擾����̐ݒ�.
	m_pGetWeapom		= std::make_shared<CMachineGun>();
}