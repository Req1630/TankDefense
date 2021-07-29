#include "DefaultWepon.h"
#include "..\..\..\..\..\Common\Mesh\DX9StaticMesh\DX9StaticMesh.h"
#include "..\..\..\..\..\Resource\MeshResource\MeshResource.h"

CDefaultWepon::CDefaultWepon()
{
	Init();
}

CDefaultWepon::~CDefaultWepon()
{
}

// 初期化関数.
bool CDefaultWepon::Init()
{
	// スタティックメッシュの取得.
	m_pStaticMesh = CMeshResorce::GetStatic( "houtou" );

	// 武器の種類の設定.
	m_Type = EType::DefaultWepon;

	// ステータスの設定.
	m_Status.BulletsNum = 10;
	m_Status.Attack		= 5.0f;
	m_Status.Speed		= 0.5f;
	m_Status.Interval	= 1.0f;

	CWeapon::Init();
	return true;
}
