#include "Weapon.h"
#include "..\..\..\..\Common\Mesh\DX9StaticMesh\DX9StaticMesh.h"
#include "..\..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\..\Object\GameObject\Actor\Bullet\Bullet.h"

CWeapon::CWeapon()
	: m_pStaticMesh	( nullptr )
	, m_pBullet		()
	, m_Type		( EType::DefaultWepon )
	, m_ListSize	()
	, m_Status		()
	, m_ShotCnt		()
{
}

CWeapon::~CWeapon()
{
}

// �������֐�.
bool CWeapon::Init()
{
	// �K�v�Ȕz�񐔂����߂�.
	m_ListSize = m_Status.BulletsNum - m_Status.BulletsNum / static_cast<int>( DISP_TIME / m_Status.Interval ) + 1;

	// �e�����z���p��.
	m_pBullet.resize( m_ListSize );

	// �e�̐ݒ�.
	for ( int i = 0; i < m_ListSize; i++ ){
		m_pBullet[i] = std::make_unique<CBullet>();
	}
	return true;
}

// �X�V�֐�.
void CWeapon::Update( const float& deltaTime )
{
	// �e�̍X�V.
	if ( m_ShotCnt < m_Status.Interval ) m_ShotCnt += deltaTime;
	for ( int i = 0; i < m_ListSize; i++ ){
		m_pBullet[i]->Update( deltaTime );
	}
	UpdateCollision();
}

// �`��֐�.
void CWeapon::Render()
{
	// ���f���̕`��.
	m_pStaticMesh->Render();

	// �e�̕`��.
	for ( int i = 0; i < m_ListSize; i++ ){
		m_pBullet[i]->Render();
	}
}

// �����蔻��֐�.
void CWeapon::Collision( CActor * pActor )
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

void CWeapon::Shot( D3DXVECTOR3 MoveVec )
{
	int cnt = 0;
	// �e�Ԋu���Ԋu���󂢂Ă�����.
	if ( m_ShotCnt >= m_Status.Interval ){
		for ( int i = 0; i < m_ListSize; i++ ){
			if ( m_pBullet[i]->GetDisp() == false ){
				m_ShotCnt = 0.0f;

				// �X�e�[�^�X�̐ݒ�.
				m_pBullet[i]->SetStatus( m_Status );

				// �e�̔���.
				if ( m_Type == EType::Missile )
					m_pBullet[i]->HomingShot( m_Tranceform.Position, D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );
				else
					m_pBullet[i]->NormalShot( m_Tranceform.Position, MoveVec );

				// �e�������炷.
				if ( m_Type != EType::DefaultWepon ){
					m_Status.BulletsNum--;
					if ( m_Status.BulletsNum <= 0 ){
						/*
						//	�������C�ɖ߂�����������.
						*/
					}
				}
				cnt++;

				if( m_Type != EType::Missile || cnt >= 8 ) break;
			}
		}
	}
}

// �ړ�.
void CWeapon::Move( D3DXVECTOR3 Pos, D3DXVECTOR3 Rot )
{
	// ���f���̈ړ�.
	m_pStaticMesh->SetPosition( Pos );
	m_Tranceform.Position = Pos;

	// ���f������]������.
	m_pStaticMesh->SetRotation( Rot );
	m_Tranceform.Rotation = Rot;
}
