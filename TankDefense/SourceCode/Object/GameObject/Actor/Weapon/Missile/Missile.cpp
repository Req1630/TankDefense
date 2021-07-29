#include "Missile.h"
#include "..\..\..\..\..\Common\Mesh\DX9StaticMesh\DX9StaticMesh.h"
#include "..\..\..\..\..\Resource\MeshResource\MeshResource.h"

CMissile::CMissile()
{
	Init();
}

CMissile::~CMissile()
{
}

// 初期化関数.
bool CMissile::Init()
{
	// スタティックメッシュの取得.
	m_pStaticMesh = CMeshResorce::GetStatic( "houtou" );

	// 武器の種類の設定.
	m_Type = EType::Missile;

	// ステータスの設定.
	m_Status.BulletsNum = 24;
	m_Status.Attack		= 5.0f;
	m_Status.Speed		= 0.2f;
	m_Status.Interval	= 1.0f;

	CWeapon::Init();
	return true;
}
