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

// �������֐�.
bool CMachineGun::Init()
{
	// �X�^�e�B�b�N���b�V���̎擾.
	m_pStaticMesh = CMeshResorce::GetStatic( "houtou" );

	// ����̎�ނ̐ݒ�.
	m_Type = EType::MachineGun;

	// �X�e�[�^�X�̐ݒ�.
	m_Status.BulletsNum = 100;
	m_Status.Attack		= 1.0f;
	m_Status.Speed		= 2.0f;
	m_Status.Interval	= 0.1f;

	CWeapon::Init();
	return true;
}
