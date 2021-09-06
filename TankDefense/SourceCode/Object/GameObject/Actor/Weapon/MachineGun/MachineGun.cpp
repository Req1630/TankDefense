#include "MachineGun.h"
#include "..\..\..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"
#include "..\..\..\..\..\Resource\MeshResource\MeshResource.h"

CMachineGun::CMachineGun()
{
	// 発射する弾のタグを設定.
	m_ObjectTag = EObjectTag::MachineGun;

	Create();
	Init();
}

CMachineGun::~CMachineGun()
{
}

// 初期化関数.
bool CMachineGun::Init()
{
	// 武器の種類の設定.
	m_Type = EType::MachineGun;

	// ステータスの設定.
	m_Status.BulletsNum = 100;
	m_Status.Attack		= 1.0f;
	m_Status.Speed		= 2.0f;
	m_Status.Interval	= 0.1f;

	// 名前の設定.
	m_BoneName			= "machingun";
	m_BulletName		= "machingun_gun";
	m_ShotName			= "shoot4";

	CWeapon::Init();
	return true;
}

// 作成関数.
void CMachineGun::Create()
{
	// スキンメッシュの取得.
	m_pSkinMesh_Body	= CMeshResorce::GetSkin( "machingun_houtou_s" );
	m_pSkinMesh_Weapon	= CMeshResorce::GetSkin( "normal_s" );
	m_pSkinMesh_Body	->SetAnimSpeed( GetDeltaTime<double>() );
	m_pSkinMesh_Weapon	->SetAnimSpeed( GetDeltaTime<double>() );
}
