#include "Enemy.h"
#include "..\..\..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"
#include "..\..\..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\..\..\..\Utility\Input\Input.h"
#include "..\..\Bullet\EnemyBullet\EnemyBullet.h"

CEnemy::CEnemy()
	: m_pEnemyBase		()
	, m_pEnemyBullet	()
	, m_HasAimPlayer	( false )
	, m_OldHasAimPlayer	( false )

{
	m_pEnemyBullet = std::make_shared<CEnemyBullet>();
	m_ObjectTag = EObjectTag::Enemy;
	Init();
}

CEnemy::~CEnemy()
{
}

// �������֐�.
bool CEnemy::Init()
{
	if ( m_pEnemyBullet->Init() == false ) return false;	// �G���G�̒e�̏�����.

	m_pSkinMesh = CMeshResorce::GetSkin( "b_s" );
	m_pSkinMesh->SetAnimSpeed( GetDeltaTime<double>() );
	m_pSkinMesh->SetPosition( D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );
	
	Spawn( D3DXVECTOR3( 0.0f, -2.5f ,5.0f ) );
	m_IsDelete = false;

	InitCollision();	// �����蔻��̏�����.
	
	return true;
}

// �X�V�֐�.
void CEnemy::Update( const float& deltaTime )
{
	m_DeltaTime = deltaTime;

	CurrentStateUpdate();	// ���݂̏�ԍX�V.
	// �X�|�[��.
	if ( CKeyInput::IsMomentPress( 'R' ) == true ) m_IsDelete = false;
	// ���S.
	if ( CKeyInput::IsMomentPress( 'F' ) == true ) m_IsDelete = true;
	// ����.
	if ( CKeyInput::IsMomentPress( 'T' ) == true ) m_IsFirght = true;

	//CDebugText::PushText( "Enemy", "---------------------------------------------" );
	//CDebugText::PushText( "Enemy", "-------------------- Enemy ------------------" );
	//CDebugText::PushText( "Enemy", "---------------------------------------------" );
	//CDebugText::PushText( "Enemy", "Pos : ", m_Tranceform.Position.x, ", ", m_Tranceform.Position.y, ", ", m_Tranceform.Position.z );
	//CDebugText::PushText( "Enemy", "Rot : ", m_Tranceform.Rotation.x, ", ", m_Tranceform.Rotation.y, ", ", m_Tranceform.Rotation.z );
	//CDebugText::PushText( "Enemy", "'R' IsDelete = false, 'F' IsDelete = true" );
	//CDebugText::PushText( "Enemy", "IsDelete : ", m_IsDelete == true ? "true" : "false" );
	//CDebugText::PushText( "Enemy", "m_HasAimPlayer : ", m_HasAimPlayer == true ? "true" : "false" );
	//CDebugText::PushText( "Enemy", "m_ShotFlg : ", m_pEnemyBullet->GetShotflg() == true ? "true" : "false" );

	UpdateCollision();	// �����蔻��̍X�V.
}

// �`��֐�.
void CEnemy::Render()
{
	// ��ʂ̊O�Ȃ�I��.
	if ( IsDisplayOut() == true ) return;

	m_pSkinMesh->SetPosition( m_Tranceform.Position );
	m_pSkinMesh->SetRotation( m_Tranceform.Rotation );
	m_pSkinMesh->SetScale( m_Tranceform.Scale );
	if ( m_IsDelete == false ) m_pSkinMesh->Render();
	m_pEnemyBullet->Render();
}

// �����蔻��֐�.
void CEnemy::Collision( CActor* pActor )
{
	if ( pActor == nullptr ) return;
	if ( m_pCollisions == nullptr ) return;
	if ( m_pCollisions->GetCollision<CSphere>() == nullptr ) return;
	if ( m_pEnemyBullet->GetCollision<CSphere>() == nullptr ) return;

	PlayerCollsion( pActor );
	SpecialEnemy_1Collsion( pActor );
	SpecialEnemy_2Collsion( pActor );
	//EnemyBulletCollsion( pActor );
}

// �����蔻��̏�����.
void CEnemy::InitCollision()
{
	m_pCollisions = std::make_unique<CCollisions>();
	m_pCollisions->InitCollision( ECollNo::Sphere );
	m_pCollisions->GetCollision<CSphere>()->SetRadius( 1.5f );
}

// �����蔻��̍��W��A���a�Ȃǂ̍X�V.
//	Update�֐��̍Ō�ɌĂ�.
void CEnemy::UpdateCollision()
{
	m_pCollisions->GetCollision<CSphere>()->SetPosition( D3DXVECTOR3( m_Tranceform.Position.x, m_Tranceform.Position.y + 4.0f, m_Tranceform.Position.z ) );
}

// �X�|�[��.
bool CEnemy::Spawn( const D3DXVECTOR3& spawnPos )
{
	// ���ɃX�|�[���ς݂Ȃ�I��.
	if ( m_NowState != Enemy::EEnemyState::None ) return true;
	m_Tranceform.Position = spawnPos;			// �X�|�[�����W�̐ݒ�.
	//m_LifePoint = pPARAMETER->LifeMax;		// �̗͂̐ݒ�.
	m_NowState = Enemy::EEnemyState::Spawn;		// ���݂̏�Ԃ��X�|�[���ɕύX.
	return true;
}

// ������W�̐ݒ�.
void CEnemy::SetTargetPos( CActor& actor )
{
	// �v���C���[�̍��W���擾.
	CEnemyBase::SetTargetPos( actor );
}

// �G�l�~�[���W�̐ݒ�.
void CEnemy::SetEnemyPos( CActor & actor )
{
	// �G�l�~�[����Ȃ���ΏI��.
	if ( actor.GetObjectTag() != EObjectTag::Enemy ) return;
	m_pEnemyBullet->SetTargetPos( actor );		// �G�l�~�[�̍��W���擾.
}

// �X�|�[����.
void CEnemy::Spawning()
{
	CEnemyBase::Spawning();
}

// �ړ�.
void CEnemy::Move()
{
	SearchRange();	// �v���C���[��_�����G����.
	// �v���C���[��ǐ�.
	if ( m_HasAimPlayer == true ) {
		TargetRotation( m_MoveSpeed );							// ��].
		CEnemyBase::VectorMove( m_MoveSpeed, ATTACK_LENGTH );	// �ړ�.
		Attack();												// �U��.
		CEnemyBase::WaitMove();									// �ҋ@.
	}
	// �v���C���[����������.
	else {
		if ( m_IsRand == false ) {
			Rand_Pos();		// �����_���ɍ��W�����߂�.
		}
		else {
			RandTargetRotation( m_MoveSpeed );	// ��].
			RandVectorMove( m_MoveSpeed );		// �ړ�.
			CEnemyBase::WaitMove();				// �ҋ@.
		}
	}
}

// ����.
void CEnemy::Fright()
{
	CEnemyBase::Fright();
}

// ���S.
void CEnemy::Death()
{
	CEnemyBase::Death();
}

// �U���֐�.
void CEnemy::Attack()
{
	if ( m_NowMoveState != Enemy::EMoveState::Attack ) return;

	// ��]�̕␳��������ׂɕK�v.
	SetMoveVector( m_TargetPosition );
	// �e�̑傫���̏�����.
	if ( m_pEnemyBullet->GetShotflg() == false ) m_pEnemyBullet->SetBulletScale();

	// �e�̑傫���̊g��.
	m_pEnemyBullet->Update( m_DeltaTime );
	// �e���傫���Ȃ��Ă���Ԃ́A��]�̕␳��������.
	if ( m_pEnemyBullet->GetScale().x < 0.8f ) {
		m_pEnemyBullet->SetBulletPos( m_Tranceform.Position, m_MoveVec3 );
		if( CActor::TargetRotation( m_MoveVec3, 0.1f, TOLERANCE_RADIAN, 0.0f ) == false ) return;
	}

	// �e����苗����ԂƏ����鏈��.
	if (D3DXVec3Length( &D3DXVECTOR3( m_pEnemyBullet->GetPosition() - m_Tranceform.Position ) ) >= 10.0f ) {
		m_pEnemyBullet->Setshotflg( false );
	}

	// �U�����s������ҋ@��ԂɑJ�ڂ���.
	if ( m_pEnemyBullet->GetShotflg() == true ) return;
	m_NowMoveState = Enemy::EMoveState::Wait;
}

// �v���C���[�Ƃ̓����蔻��.
void CEnemy::PlayerCollsion( CActor* pActor )
{
	// �I�u�W�F�N�g�̃^�O���v���C���[����Ȃ���ΏI��.
	if ( pActor->GetObjectTag() != EObjectTag::Player ) return;

	// �G���G�ƃv���C���[�̓����蔻��̎���.
	if ( coll::IsSphereToSphere( m_pCollisions->GetCollision<CSphere>(), pActor->GetCollision<CSphere>() ) == false ) return;

	float Radius = pActor->GetCollision<CSphere>()->GetRadius();	//���a.

	D3DXVECTOR3 Pos[2]{ m_Tranceform.Position, pActor->GetPosition() };

	D3DXVECTOR3 Distance = Pos[0] - Pos[1];// �G���G�ƃv���C���[�̋����ivector�j

	float inDistance = ( Radius )-( D3DXVec3Length( &Distance ) );// �G���G�̎h�����Ă��钷��

	D3DXVECTOR3 vecPushOut;// �����o��vector
	D3DXVec3Normalize( &vecPushOut, &Distance );// �o0 - �o1�̐��K���޸�� 

	// �޸�قɒ����������Ďh�����������݂��������߂�
	Pos[0] -= vecPushOut * inDistance / 12;
	Pos[1] -= -vecPushOut * inDistance / 12;

	//m_Tranceform.Position = Pos[0];
	pActor->SetPosition( Pos[1] );
}

// ����ȓG1�Ƃ̓����蔻��.
void CEnemy::SpecialEnemy_1Collsion( CActor * pActor )
{
	// �I�u�W�F�N�g�̃^�O������ȓG1����Ȃ���ΏI��.
	if ( pActor->GetObjectTag() != EObjectTag::SpecialEnemy_1 ) return;

	// �G���G�Ɠ���ȓG1�̓����蔻��̎���.
	if ( coll::IsSphereToSphere( m_pCollisions->GetCollision<CSphere>(), pActor->GetCollision<CSphere>() ) == false ) return;

	float Radius = pActor->GetCollision<CSphere>()->GetRadius();	// ���a.

	D3DXVECTOR3 Pos[2]{ m_Tranceform.Position, pActor->GetPosition() };

	D3DXVECTOR3 Distance = Pos[0] - Pos[1];// �G���G�Ɠ���ȓG1�̋����ivector�j

	float inDistance = ( Radius )-( D3DXVec3Length( &Distance ) );// �G���G�̎h�����Ă��钷��

	D3DXVECTOR3 vecPushOut;// �����o��vector
	D3DXVec3Normalize( &vecPushOut, &Distance );// �o0 - �o1�̐��K���޸�� 

	// �޸�قɒ����������Ďh�����������݂��������߂�
	Pos[0] -= vecPushOut * inDistance / 12;
	Pos[1] -= -vecPushOut * inDistance / 12;

	//m_Tranceform.Position = Pos[0];
	pActor->SetPosition( Pos[1] );
}

// ����ȓG2�Ƃ̓����蔻��.
void CEnemy::SpecialEnemy_2Collsion( CActor * pActor )
{
	// �I�u�W�F�N�g�̃^�O������ȓG2����Ȃ���ΏI��.
	if ( pActor->GetObjectTag() != EObjectTag::SpecialEnemy_2 ) return;

	// �G���G�Ɠ���ȓG2�̓����蔻��̎���.
	if ( coll::IsSphereToSphere( m_pCollisions->GetCollision<CSphere>(), pActor->GetCollision<CSphere>() ) == false ) return;

	float Radius = pActor->GetCollision<CSphere>()->GetRadius();	// ���a.

	D3DXVECTOR3 Pos[2]{ m_Tranceform.Position, pActor->GetPosition() };

	D3DXVECTOR3 Distance = Pos[0] - Pos[1];// �G���G�Ɠ���ȓG2�̋����ivector�j

	float inDistance = ( Radius )-( D3DXVec3Length( &Distance ) );// �G���G�̎h�����Ă��钷��

	D3DXVECTOR3 vecPushOut;// �����o��vector
	D3DXVec3Normalize( &vecPushOut, &Distance );// �o0 - �o1�̐��K���޸�� 

	// �޸�قɒ����������Ďh�����������݂��������߂�
	Pos[0] -= vecPushOut * inDistance / 12;
	Pos[1] -= -vecPushOut * inDistance / 12;

	//m_Tranceform.Position = Pos[0];
	pActor->SetPosition( Pos[1] );
}

// �v���C���[�ƎG���G�̒e�̓����蔻��.
void CEnemy::EnemyBulletCollsion( CActor * pActor )
{
	// �I�u�W�F�N�g�̃^�O���v���C���[����Ȃ���ΏI��.
	if ( pActor->GetObjectTag() != EObjectTag::Player ) return;
	if ( m_NowMoveState != Enemy::EMoveState::Attack ) return;	// �U����Ԃ���Ȃ���ΏI��.
	if ( m_NowState == Enemy::EEnemyState::Death ) return;	// ���S���Ă�����I��.
	if ( m_NowState == Enemy::EEnemyState::Fright ) return;	// ���ݏ�ԂȂ�I��.

	// �G���G�̒e�ƃv���C���[�̓����蔻��.
	if ( coll::IsSphereToSphere( m_pEnemyBullet->GetCollision<CSphere>(), pActor->GetCollision<CSphere>() ) == false ) return;

	// �v���C���[������.
	pActor->SetDelete( [&]( bool& isDead )
	{
		isDead = true;
	});

	//// ���݂̈ړ��x�N�g����ݒ�.
	//pActor->SetVector( m_MoveVector );
	//// �v���C���[�̗̑͂����炷.
	//pActor->LifeCalculation( [&]( float& life, bool& isAttack )
	//{
	//	life -= pPARAMETER->AttackPower;
	//	isAttack = true;
	//});
}

// ���G�͈�.
void CEnemy::SearchRange()
{
	if ( m_NowMoveState == Enemy::EMoveState::Attack ) return;

	// �v���C���[�Ƃ̋������v�Z.
	float PlayerLength = D3DXVec3Length( &D3DXVECTOR3( m_TargetPosition - m_Tranceform.Position ) );

	m_OldHasAimPlayer = m_HasAimPlayer;
	// �v���C���[�̑_���͈͂��r.
	if ( PlayerLength <= 30.0f ) {
		// ���G�͈͓��Ƀv���C���[������ꍇ.
		m_HasAimPlayer = true;
		m_IsRand = false;
		SetMoveVector( m_TargetPosition );
	}
	else {
		// ���G�͈͊O�̏ꍇ.
		m_HasAimPlayer = false;
		SetRandMoveVector( m_RandPos );
	}
	if ( m_OldHasAimPlayer == m_HasAimPlayer ) return;
	m_NowMoveState = Enemy::EMoveState::Wait;
}
