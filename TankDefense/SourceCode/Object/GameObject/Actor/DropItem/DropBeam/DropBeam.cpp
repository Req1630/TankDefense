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

// ì¬ŠÖ”.
void CDropBeam::Create()
{
	// ƒ‚ƒfƒ‹‚Ìİ’è.
	m_pStaticMesh		= CMeshResorce::GetStatic( "item_beam" );
	m_pBillboardSprite	= CSpriteResource::GetSprite( "BeamName" );

	// æ“¾•Ší‚Ìİ’è.
	m_pGetWeapom		= std::make_shared<CBeam>();
}