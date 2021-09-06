#include "Beam.h"
#include "..\..\..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"
#include "..\..\..\..\..\Resource\MeshResource\MeshResource.h"

CBeam::CBeam()
{
	// 発射する弾のタグを設定.
	m_ObjectTag = EObjectTag::Beam;

	Create();
	Init();
}

CBeam::~CBeam()
{
}

// 初期化関数.
bool CBeam::Init()
{
	// 武器の種類の設定.
	m_Type = EType::Beam;

	// ステータスの設定.
	m_Status.BulletsNum = 1;
	m_Status.Attack		= 10.0f;
	m_Status.Speed		= 5.0f;
	m_Status.Interval	= 1.0f;

	// 名前の設定.
	m_BoneName			= "beam1";
	m_BulletName		= "normal_tama";
	m_ShotName			= "beam1";

	CWeapon::Init();
	return true;
}

// 作成関数.
void CBeam::Create()
{
	// スキンメッシュの取得.
	m_pSkinMesh_Body	= CMeshResorce::GetSkin( "beam_houtou_s" );
	m_pSkinMesh_Weapon	= CMeshResorce::GetSkin( "normal_s" );
	m_pSkinMesh_Body	->SetAnimSpeed( GetDeltaTime<double>() );
	m_pSkinMesh_Weapon	->SetAnimSpeed( GetDeltaTime<double>() );
}
