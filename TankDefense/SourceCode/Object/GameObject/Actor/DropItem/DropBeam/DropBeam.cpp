#include "DropBeam.h"
#include "..\..\..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\Weapon\Beam\Beam.h"

CDropBeam::CDropBeam()
{
	Create();
	Init();
}

CDropBeam::~CDropBeam()
{
}

// �쐬�֐�.
void CDropBeam::Create()
{
	// ���f���̐ݒ�.
	m_pStaticMesh		= CMeshResorce::GetStatic( "item_beam" );
	m_pBillboardSprite	= CSpriteResource::GetSprite( "BeamName" );

	// �擾����̐ݒ�.
	m_pGetWeapom		= std::make_shared<CBeam>();
}