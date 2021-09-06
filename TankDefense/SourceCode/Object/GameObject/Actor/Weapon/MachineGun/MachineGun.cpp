#include "MachineGun.h"
#include "..\..\..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"
#include "..\..\..\..\..\Resource\MeshResource\MeshResource.h"

CMachineGun::CMachineGun()
{
	// ���˂���e�̃^�O��ݒ�.
	m_ObjectTag = EObjectTag::MachineGun;

	Create();
	Init();
}

CMachineGun::~CMachineGun()
{
}

// �������֐�.
bool CMachineGun::Init()
{
	// ����̎�ނ̐ݒ�.
	m_Type = EType::MachineGun;

	// �X�e�[�^�X�̐ݒ�.
	m_Status.BulletsNum = 100;
	m_Status.Attack		= 1.0f;
	m_Status.Speed		= 2.0f;
	m_Status.Interval	= 0.1f;

	// ���O�̐ݒ�.
	m_BoneName			= "machingun";
	m_BulletName		= "machingun_gun";
	m_ShotName			= "shoot4";

	CWeapon::Init();
	return true;
}

// �쐬�֐�.
void CMachineGun::Create()
{
	// �X�L�����b�V���̎擾.
	m_pSkinMesh_Body	= CMeshResorce::GetSkin( "machingun_houtou_s" );
	m_pSkinMesh_Weapon	= CMeshResorce::GetSkin( "normal_s" );
	m_pSkinMesh_Body	->SetAnimSpeed( GetDeltaTime<double>() );
	m_pSkinMesh_Weapon	->SetAnimSpeed( GetDeltaTime<double>() );
}
