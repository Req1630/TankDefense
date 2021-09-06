#include "Weapon.h"
#include "..\..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"
#include "..\..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\..\Object\GameObject\Actor\Weapon\Bullet\BulletManager\BulletManager.h"

CWeapon::CWeapon()
	: m_pBulletMng			()
	, m_ShotCnt				()
	, m_pSkinMesh_Body		( nullptr )
	, m_pSkinMesh_Weapon	( nullptr )
	, m_Type				( EType::DefaultWepon )
	, m_Status				()
	, m_BoneName			()
{
}

CWeapon::~CWeapon()
{
}

// �������֐�.
bool CWeapon::Init()
{
	InitCollision();
	return true;
}

// �X�V�֐�.
void CWeapon::Update( const float& deltaTime )
{
	// �e�̍X�V.
	if ( m_ShotCnt < m_Status.Interval ) m_ShotCnt += deltaTime;
	DebugUpdate();
	UpdateCollision();
}

// �`��֐�.
void CWeapon::Render()
{
	if ( m_pSkinMesh_Body	== nullptr ) return;
	if ( m_pSkinMesh_Weapon == nullptr ) return;

	// ���f���̕`��.
	m_pSkinMesh_Body->SetTranceform( m_Tranceform );
	m_pSkinMesh_Body->Render();
	m_pSkinMesh_Weapon->Render();
}

// �����蔻��֐�.
void CWeapon::Collision( CActor* pActor )
{
}

// �����蔻��̏�����.
void CWeapon::InitCollision()
{
}

// �����蔻��̍��W��A���a�Ȃǂ̍X�V.
void CWeapon::UpdateCollision()
{
}

// �e�̔��ˊ֐�.
//	���e�������Ȃ����ꍇ"false"��Ԃ�.
bool CWeapon::Shot( D3DXVECTOR3 MoveVec )
{
	D3DXVECTOR3 ShotPos = m_Tranceform.Position;
	m_pSkinMesh_Weapon->GetPosFromBone( m_ShotName.c_str(), &ShotPos );

	// �e�Ԋu���Ԋu���󂢂Ă�����.
	if ( m_ShotCnt >= m_Status.Interval ){
		m_ShotCnt = 0.0f;

		//�����ɔ��˂���e�̐�.
		int ShotNum = 1;
		if ( m_Type == EType::Missile ) ShotNum = 8;

		for ( int i = 0; i < ShotNum; i++ ){
			// �e�̔���.
			if ( m_Type == EType::Missile )
				m_pBulletMng->HomingShot( m_BulletName, EObjectTag::PlayerBullet, m_Status, ShotPos, D3DXVECTOR3( 0.0f, 0.0f, 0.0f ), m_Tranceform.Rotation );
			else
				m_pBulletMng->NormalShot( m_BulletName, EObjectTag::PlayerBullet, m_Status, ShotPos, m_Tranceform.Rotation, MoveVec );

			// �e�������炷.
			if ( m_Type != EType::DefaultWepon ){
				m_Status.BulletsNum--;

				// ����̕ύX�̂��m�点.
				if ( m_Status.BulletsNum <= 0 ) return false;
			}
		}
	}
	return true;
}

// �ړ��֐�.
void CWeapon::Move( D3DXVECTOR3 BodyPos, D3DXVECTOR3 Rot )
{
	// �ړ��A��].
	m_Tranceform.Position = BodyPos;
	m_Tranceform.Rotation = Rot;

	// ���W�̐ݒ�.
	D3DXVECTOR3 BonePos = m_Tranceform.Position;
	m_pSkinMesh_Body->GetPosFromBone( m_BoneName.c_str(), &BonePos );

	// ���f���ɔ��f������.
	if ( m_pSkinMesh_Body == nullptr ) return;
	m_pSkinMesh_Body->SetPosition( BodyPos );
	m_pSkinMesh_Body->SetRotation( Rot );
	if ( m_pSkinMesh_Weapon == nullptr ) return;
	m_pSkinMesh_Weapon->SetPosition( BonePos );
	m_pSkinMesh_Weapon->SetRotation( Rot );
}

// �f�o�b�N�̍X�V�֐�.
void CWeapon::DebugUpdate()
{
	CDebugText::PushText( "Bullet", "------------------" );
	CDebugText::PushText( "Bullet", "----  Bullet  ----" );
	CDebugText::PushText( "Bullet", "------------------" );
	CDebugText::PushText( "Bullet", "Type             : ", m_Type );
	CDebugText::PushText( "Bullet", "BulletsNum       : ", m_Status.BulletsNum );
	CDebugText::PushText( "Bullet", "Attack           : ", m_Status.Attack );
	CDebugText::PushText( "Bullet", "Speed            : ", m_Status.Speed );
	CDebugText::PushText( "Bullet", "Interval         : ", m_Status.Interval );
	CDebugText::PushText( "Bullet", "ShotCnt          : ", m_ShotCnt );
	CDebugText::PushText( "Bullet", "------------------" );
}