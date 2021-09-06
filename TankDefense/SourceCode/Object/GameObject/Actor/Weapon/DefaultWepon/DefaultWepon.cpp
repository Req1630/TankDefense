#include "DefaultWepon.h"
#include "..\..\..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"
#include "..\..\..\..\..\Resource\MeshResource\MeshResource.h"

CDefaultWepon::CDefaultWepon()
{
	// ���˂���e�̃^�O��ݒ�.
	m_ObjectTag = EObjectTag::DefaultWepon;

	Create();
	Init();
}

CDefaultWepon::~CDefaultWepon()
{
}

// �������֐�.
bool CDefaultWepon::Init()
{
	// ����̎�ނ̐ݒ�.
	m_Type = EType::DefaultWepon;

	// �X�e�[�^�X�̐ݒ�.
	m_Status.BulletsNum = 10;
	m_Status.Attack		= 5.0f;
	m_Status.Speed		= 0.5f;
	m_Status.Interval	= 1.0f;

	// ���O�̐ݒ�.
	m_BoneName			= "normal1";
	m_BulletName		= "normal_tama";
	m_ShotName			= "shoot_normal";

	CWeapon::Init();
	return true;
}

// �쐬�֐�.
void CDefaultWepon::Create()
{
	// �X�L�����b�V���̎擾.
	m_pSkinMesh_Body	= CMeshResorce::GetSkin( "normal_houtou_s" );
	m_pSkinMesh_Weapon	= CMeshResorce::GetSkin( "normal_s" );
	m_pSkinMesh_Body	->SetAnimSpeed( GetDeltaTime<double>() );
	m_pSkinMesh_Weapon	->SetAnimSpeed( GetDeltaTime<double>() );
}
