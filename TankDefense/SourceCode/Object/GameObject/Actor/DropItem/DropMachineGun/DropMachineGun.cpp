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

// 作成関数.
void CDropMachineGun::Create()
{
	// モデルの設定.
	m_pStaticMesh		= CMeshResorce::GetStatic( "item_machingun" );
	m_pBillboardSprite	= CSpriteResource::GetSprite( "MachineGunName" );

	// 取得武器の設定.
	m_pGetWeapom		= std::make_shared<CMachineGun>();
}