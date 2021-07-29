#include "MachineGun.h"
#include "..\..\..\..\..\Common\Mesh\DX9StaticMesh\DX9StaticMesh.h"
#include "..\..\..\..\..\Resource\MeshResource\MeshResource.h"

CMachineGun::CMachineGun()
{
	Init();
}

CMachineGun::~CMachineGun()
{
}

// 初期化関数.
bool CMachineGun::Init()
{
	// スタティックメッシュの取得.
	m_pStaticMesh = CMeshResorce::GetStatic( "houtou" );

	// 武器の種類の設定.
	m_Type = EType::MachineGun;

	// ステータスの設定.
	m_Status.BulletsNum = 100;
	m_Status.Attack		= 1.0f;
	m_Status.Speed		= 2.0f;
	m_Status.Interval	= 0.1f;

	CWeapon::Init();
	return true;
}
