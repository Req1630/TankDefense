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

// 作成関数.
void CDropBeam::Create()
{
	// モデルの設定.
	m_pStaticMesh		= CMeshResorce::GetStatic( "item_beam" );
	m_pBillboardSprite	= CSpriteResource::GetSprite( "BeamName" );

	// 取得武器の設定.
	m_pGetWeapom		= std::make_shared<CBeam>();
}