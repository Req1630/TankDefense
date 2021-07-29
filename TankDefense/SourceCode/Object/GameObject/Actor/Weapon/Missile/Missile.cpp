#include "Missile.h"
#include "..\..\..\..\..\Common\Mesh\DX9StaticMesh\DX9StaticMesh.h"
#include "..\..\..\..\..\Resource\MeshResource\MeshResource.h"

CMissile::CMissile()
{
	Init();
}

CMissile::~CMissile()
{
}

// �������֐�.
bool CMissile::Init()
{
	// �X�^�e�B�b�N���b�V���̎擾.
	m_pStaticMesh = CMeshResorce::GetStatic( "houtou" );

	// ����̎�ނ̐ݒ�.
	m_Type = EType::Missile;

	// �X�e�[�^�X�̐ݒ�.
	m_Status.BulletsNum = 24;
	m_Status.Attack		= 5.0f;
	m_Status.Speed		= 0.2f;
	m_Status.Interval	= 1.0f;

	CWeapon::Init();
	return true;
}
