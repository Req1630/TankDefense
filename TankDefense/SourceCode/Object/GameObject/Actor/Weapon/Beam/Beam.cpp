#include "Beam.h"
#include "..\..\..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"
#include "..\..\..\..\..\Resource\MeshResource\MeshResource.h"

CBeam::CBeam()
{
	// ���˂���e�̃^�O��ݒ�.
	m_ObjectTag = EObjectTag::Beam;

	Create();
	Init();
}

CBeam::~CBeam()
{
}

// �������֐�.
bool CBeam::Init()
{
	// ����̎�ނ̐ݒ�.
	m_Type = EType::Beam;

	// �X�e�[�^�X�̐ݒ�.
	m_Status.BulletsNum = 1;
	m_Status.Attack		= 10.0f;
	m_Status.Speed		= 5.0f;
	m_Status.Interval	= 1.0f;

	// ���O�̐ݒ�.
	m_BoneName			= "beam1";
	m_BulletName		= "normal_tama";
	m_ShotName			= "beam1";

	CWeapon::Init();
	return true;
}

// �쐬�֐�.
void CBeam::Create()
{
	// �X�L�����b�V���̎擾.
	m_pSkinMesh_Body	= CMeshResorce::GetSkin( "beam_houtou_s" );
	m_pSkinMesh_Weapon	= CMeshResorce::GetSkin( "normal_s" );
	m_pSkinMesh_Body	->SetAnimSpeed( GetDeltaTime<double>() );
	m_pSkinMesh_Weapon	->SetAnimSpeed( GetDeltaTime<double>() );
}
