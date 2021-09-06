#include "SpecialEnemy_1.h"
#include "..\..\..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"
#include "..\..\..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\..\..\..\Utility\Input\Input.h"
#include "..\..\Explosion\Explosion.h"

CSpecialEnemy_1::CSpecialEnemy_1()
	: m_pEnemyBase		()
	, m_pExplosion		( nullptr )

{
	m_pExplosion	= std::make_shared<CExplosion>();
	m_ObjectTag		= EObjectTag::SpecialEnemy_1;
	Init();
}

CSpecialEnemy_1::~CSpecialEnemy_1()
{
}

// �������֐�.
bool CSpecialEnemy_1::Init()
{
	if ( m_pExplosion->Init() == false ) return false;	// �����̏�����.

	m_pSkinMesh = CMeshResorce::GetSkin( "enemy_bomb_s" );
	m_pSkinMesh->SetAnimSpeed( GetDeltaTime<double>() );
	m_pSkinMesh->SetPosition( D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );
	
	Spawn( D3DXVECTOR3( 0.0f, 0.0f ,-20.0f ) );
	m_IsDelete = false;

	InitCollision();	// �����蔻��̏�����.
	
	return true;
}

// �X�V�֐�.
void CSpecialEnemy_1::Update( const float& deltaTime )
{

	m_DeltaTime = deltaTime;

	CurrentStateUpdate();	// ���݂̏�ԍX�V.

	// �X�|�[��.
	if ( CKeyInput::IsMomentPress( 'T' ) == true ) m_IsDelete = false;
	// ���S.
	if ( CKeyInput::IsMomentPress( 'G' ) == true ) m_IsDelete = true;

	DebugUpdate();			// �f�o�b�N�X�V.

	UpdateCollision();	// �����蔻��̍X�V.
}

// �`��֐�.
void CSpecialEnemy_1::Render()
{
	// ��ʂ̊O�Ȃ�I��.
	if ( IsDisplayOut() == true ) return;

	m_pSkinMesh->SetTranceform( m_Tranceform );
	if ( m_IsDelete == false ) m_pSkinMesh->Render();
	
}

// Sprite3D�`��֐�.
void CSpecialEnemy_1::Sprite3DRender()
{
	m_pExplosion->Render();
}

// �����蔻��֐�.
void CSpecialEnemy_1::Collision( CActor* pActor )
{
	if ( pActor == nullptr ) return;
	if ( m_pCollisions == nullptr ) return;
	if ( m_pCollisions->GetCollision<CSphere>() == nullptr ) return;
	if ( m_pExplosion->GetCollision<CSphere>() == nullptr ) return;

	PlayerCollsion( pActor );
	ExplosionCollsion( pActor );
	ExplosionEnemyCollsion( pActor );
	ExplosionSEnemy2Collsion( pActor );
}

// �����蔻��̏�����.
void CSpecialEnemy_1::InitCollision()
{
	m_pCollisions = std::make_unique<CCollisions>();
	m_pCollisions->InitCollision( ECollNo::Sphere );
	m_pCollisions->GetCollision<CSphere>()->SetRadius( 1.5f );
}

// �����蔻��̍��W��A���a�Ȃǂ̍X�V.
//	Update�֐��̍Ō�ɌĂ�.
void CSpecialEnemy_1::UpdateCollision()
{
	m_pCollisions->GetCollision<CSphere>()->SetPosition( D3DXVECTOR3( m_Tranceform.Position.x, m_Tranceform.Position.y + 4.0f, m_Tranceform.Position.z ) );
}

// �X�|�[��.
bool CSpecialEnemy_1::Spawn( const D3DXVECTOR3& spawnPos )
{
	// ���ɃX�|�[���ς݂Ȃ�I��.
	if ( m_NowState != Enemy::EEnemyState::None ) return true;
	m_Tranceform.Position = spawnPos;			// �X�|�[�����W�̐ݒ�.
	//m_LifePoint = pPARAMETER->LifeMax;		// �̗͂̐ݒ�.
	m_NowState = Enemy::EEnemyState::Spawn;		// ���݂̏�Ԃ��X�|�[���ɕύX.
	return true;
}

// ������W�̐ݒ�.
void CSpecialEnemy_1::SetTargetPos( CActor& actor )
{
	// �v���C���[�̍��W���擾.
	CEnemyBase::SetTargetPos( actor );
}

// �X�|�[����.
void CSpecialEnemy_1::Spawning()
{
	CEnemyBase::Spawning();
}

// �ړ�.
void CSpecialEnemy_1::Move()
{
	// �v���C���[��ǐ�.
	SetMoveVector( m_TargetPosition );									// �ړ��x�N�g���̐ݒ�.
	TargetRotation( m_MoveSpeed / 2 );									// ��].
	CEnemyBase::VectorMove( m_MoveSpeed / 2, EXPLOSION_ATTACK_LENGTH );	// �ړ�.
	Attack();															// �U��.
	CEnemyBase::WaitMove();												// �ҋ@.
}

// ����.
void CSpecialEnemy_1::Fright()
{
	CEnemyBase::Fright();
}

// ���S.
void CSpecialEnemy_1::Death()
{
	CEnemyBase::Death();
}

// �U���֐�.
void CSpecialEnemy_1::Attack()
{
	if ( m_NowMoveState != Enemy::EMoveState::Attack ) return;

	if ( m_pExplosion->GetIsExplosion() == false ) m_pExplosion->SetIsExplosion( true );
	m_pExplosion->Update( m_DeltaTime );
	m_pExplosion->SetExplosionPos( m_Tranceform.Position );

	// �U�����s������ҋ@��ԂɑJ�ڂ���.
	if ( m_pExplosion->GetIsExplosion() == true ) return;
	m_NowMoveState = Enemy::EMoveState::Wait;
}

// �v���C���[�Ƃ̓����蔻��.
void CSpecialEnemy_1::PlayerCollsion( CActor* pActor )
{
	// �I�u�W�F�N�g�̃^�O���v���C���[����Ȃ���ΏI��.
	if ( pActor->GetObjectTag() != EObjectTag::Player ) return;

	// �v���C���[�ƃG�l�~�[�̓����蔻��̎���.
	if ( coll::IsSphereToSphere( m_pCollisions->GetCollision<CSphere>(), pActor->GetCollision<CSphere>() ) == false ) return;

	float Radius = pActor->GetCollision<CSphere>()->GetRadius();	//���a.

	D3DXVECTOR3 Pos[2]{ m_Tranceform.Position, pActor->GetPosition() };

	D3DXVECTOR3 Distance = Pos[0] - Pos[1];//�v���C���[�ƎG���G�̋����ivector�j

	float inDistance = ( Radius )-( D3DXVec3Length( &Distance ) );//�v���C���[�̎h�����Ă��钷��

	D3DXVECTOR3 vecPushOut;//�����o��vector
	D3DXVec3Normalize( &vecPushOut, &Distance );//�o0 - �o1�̐��K���޸�� 

	// �޸�قɒ����������Ďh�����������݂��������߂�
	Pos[0].x -= vecPushOut.x * inDistance / 8;
	Pos[0].z -= vecPushOut.z * inDistance / 8;
	Pos[1].x -= -vecPushOut.x * inDistance / 8;
	Pos[1].z -= -vecPushOut.z * inDistance / 8;

	//m_Tranceform.Position = Pos[0];
	pActor->SetPosition( Pos[1] );
}

// �v���C���[�Ɣ����̓����蔻��.
void CSpecialEnemy_1::ExplosionCollsion( CActor * pActor )
{
	// �I�u�W�F�N�g�̃^�O���v���C���[����Ȃ���ΏI��.
	if ( pActor->GetObjectTag() != EObjectTag::Player ) return;
	if ( m_NowMoveState != Enemy::EMoveState::Attack ) return;	// �U����Ԃ���Ȃ���ΏI��.
	if ( m_NowState == Enemy::EEnemyState::Death ) return;	// ���S���Ă�����I��.
	if ( m_NowState == Enemy::EEnemyState::Fright ) return;	// ���ݏ�ԂȂ�I��.

	// �����ƃv���C���[�̓����蔻��.
	if ( coll::IsSphereToSphere( m_pExplosion->GetCollision<CSphere>(), pActor->GetCollision<CSphere>() ) == false ) return;

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

// �G���G�Ɣ����̓����蔻��.
void CSpecialEnemy_1::ExplosionEnemyCollsion( CActor * pActor )
{
	// �I�u�W�F�N�g�̃^�O���G���G����Ȃ���ΏI��.
	if ( pActor->GetObjectTag() != EObjectTag::Enemy ) return;
	if ( m_NowMoveState != Enemy::EMoveState::Attack ) return;	// �U����Ԃ���Ȃ���ΏI��.
	if ( m_NowState == Enemy::EEnemyState::Death ) return;	// ���S���Ă�����I��.
	if ( m_NowState == Enemy::EEnemyState::Fright ) return;	// ���ݏ�ԂȂ�I��.

	// �����ƎG���G�̓����蔻��.
	if ( coll::IsSphereToSphere( m_pExplosion->GetCollision<CSphere>(), pActor->GetCollision<CSphere>() ) == false ) return;

	// �G���G������.
	pActor->SetDelete( [&]( bool& isDead )
	{
		isDead = true;
	});
}

// ����ȓG2�Ɣ����̓����蔻��.
void CSpecialEnemy_1::ExplosionSEnemy2Collsion( CActor * pActor )
{
	// �I�u�W�F�N�g�̃^�O������ȓG2����Ȃ���ΏI��.
	if ( pActor->GetObjectTag() != EObjectTag::SpecialEnemy_2 ) return;
	if ( m_NowMoveState != Enemy::EMoveState::Attack ) return;	// �U����Ԃ���Ȃ���ΏI��.
	if ( m_NowState == Enemy::EEnemyState::Death ) return;	// ���S���Ă�����I��.
	if ( m_NowState == Enemy::EEnemyState::Fright ) return;	// ���ݏ�ԂȂ�I��.

	// �����Ɠ���ȓG2�̓����蔻��.
	if ( coll::IsSphereToSphere( m_pExplosion->GetCollision<CSphere>(), pActor->GetCollision<CSphere>() ) == false ) return;

	// ����ȓG2������.
	pActor->SetDelete( [&]( bool& isDead )
	{
		isDead = true;
	});
}

// �f�o�b�N�X�V�֐�.
void CSpecialEnemy_1::DebugUpdate()
{
#ifdef _DEBUG
	CDebugText::PushText("SpecialEnemy_1", "---------------------------------------------");
	CDebugText::PushText("SpecialEnemy_1", "--------------- SpecialEnemy_1 --------------");
	CDebugText::PushText("SpecialEnemy_1", "---------------------------------------------");
	CDebugText::PushText("SpecialEnemy_1", "Pos : ", m_Tranceform.Position.x, ", ", m_Tranceform.Position.y, ", ", m_Tranceform.Position.z);
	CDebugText::PushText("SpecialEnemy_1", "Rot : ", m_Tranceform.Rotation.x, ", ", m_Tranceform.Rotation.y, ", ", m_Tranceform.Rotation.z);
	CDebugText::PushText("SpecialEnemy_1", "'T' IsDelete = false, 'G' IsDelete = true");
	CDebugText::PushText("SpecialEnemy_1", "IsDelete : ", m_IsDelete == true ? "true" : "false");
	CDebugText::PushText("SpecialEnemy_1", "m_IsExplosion : ", m_pExplosion->GetIsExplosion() == true ? "true" : "false");
#endif
}
