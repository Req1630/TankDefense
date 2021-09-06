#include "DefaultWepon.h"
#include "..\..\..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"
#include "..\..\..\..\..\Resource\MeshResource\MeshResource.h"

CDefaultWepon::CDefaultWepon()
{
	// 発射する弾のタグを設定.
	m_ObjectTag = EObjectTag::DefaultWepon;

	Create();
	Init();
}

CDefaultWepon::~CDefaultWepon()
{
}

// 初期化関数.
bool CDefaultWepon::Init()
{
	// 武器の種類の設定.
	m_Type = EType::DefaultWepon;

	// ステータスの設定.
	m_Status.BulletsNum = 10;
	m_Status.Attack		= 5.0f;
	m_Status.Speed		= 0.5f;
	m_Status.Interval	= 1.0f;

	// 名前の設定.
	m_BoneName			= "normal1";
	m_BulletName		= "normal_tama";
	m_ShotName			= "shoot_normal";

	CWeapon::Init();
	return true;
}

// 作成関数.
void CDefaultWepon::Create()
{
	// スキンメッシュの取得.
	m_pSkinMesh_Body	= CMeshResorce::GetSkin( "normal_houtou_s" );
	m_pSkinMesh_Weapon	= CMeshResorce::GetSkin( "normal_s" );
	m_pSkinMesh_Body	->SetAnimSpeed( GetDeltaTime<double>() );
	m_pSkinMesh_Weapon	->SetAnimSpeed( GetDeltaTime<double>() );
}
