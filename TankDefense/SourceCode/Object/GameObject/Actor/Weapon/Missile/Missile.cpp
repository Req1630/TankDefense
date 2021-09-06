#include "Missile.h"
#include "..\..\..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"
#include "..\..\..\..\..\Resource\MeshResource\MeshResource.h"

CMissile::CMissile()
{
	// 発射する弾のタグを設定.
	m_ObjectTag = EObjectTag::Missile;

	Create();
	Init();
}

CMissile::~CMissile()
{
}

// 初期化関数.
bool CMissile::Init()
{
	// 武器の種類の設定.
	m_Type = EType::Missile;

	// ステータスの設定.
	m_Status.BulletsNum = 24;
	m_Status.Attack		= 5.0f;
	m_Status.Speed		= 0.2f;
	m_Status.Interval	= 1.0f;
	
	// 名前の設定.
	m_BoneName			= "tadan_y_1_4";
	m_BulletName		= "tadan_gun";
	m_ShotName			= "shoot_r_4";

	CWeapon::Init();
	return true;
}

void CMissile::Create()
{
	// スキンメッシュの取得.
	m_pSkinMesh_Body	= CMeshResorce::GetSkin( "tadan_houtou_s" );
	m_pSkinMesh_Weapon	= CMeshResorce::GetSkin( "tadan_s" );
	m_pSkinMesh_Body	->SetAnimSpeed( GetDeltaTime<double>() );
	m_pSkinMesh_Weapon	->SetAnimSpeed( GetDeltaTime<double>() );
}