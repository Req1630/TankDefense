#include "Boss.h"
#include "..\..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"
#include "..\..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\..\..\Utility\Input\Input.h"

CBoss::CBoss()
	: m_pSkinMesh				( nullptr )
	
	, m_TargetPosition			( 0.0f, 0.0f, 0.0f )
	, m_TargetRotation			( 0.0f, 0.0f, 0.0f )
	, m_BeforeMoveingPosition	( 0.0f, 0.0f, 0.0f )
	, m_MoveVec3				( 0.0f, 0.0f, 0.0f )
	, m_RandPos					( 0.0f, 0.0f, 0.0f )
	, m_OldRandPos				( 1.0f, 0.0f, 0.0f )
	, m_RandRot					( 0.0f, 0.0f, 0.0f )
	, m_NowState				( Boss::EBossState::None )
	, m_NowMoveState			( Boss::EMoveState::None )
	, m_Life					( 0.0f )
	, m_MoveSpeed				( 0.05f )
	, m_DeathCount				( 0.0f )
	, m_FrightCount				( 0 )
	, m_HitCount				( 0 )
	, m_WaitCount				( 0 )
	, m_InvincibleCount			( 0 )
	, m_RandAttack				( 0 )
	, m_AttackWave				( 0 )
	, m_AttackCount				( 0 )
	, m_IsFirght				( false )
	, m_IsExplosion				( false )
	, m_IsDelete				( false )
	, m_IsRandMove				( false )
	, m_IsRandAttack			( false )
	, m_HasAimPlayer			( false )
	, m_OldHasAimPlayer			( false )

{
	m_ObjectTag		= EObjectTag::Boss;
	Init();
}

CBoss::~CBoss()
{
}

// �������֐�.
bool CBoss::Init()
{
	m_pSkinMesh = CMeshResorce::GetSkin( "dragon_kari_s" );
	m_pSkinMesh->SetAnimSpeed( GetDeltaTime<double>() );
	m_pSkinMesh->SetPosition( D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );
	
	Spawn( D3DXVECTOR3( 0.0f, 0.0f ,-20.0f ) );
	m_IsDelete = false;

	InitCollision();	// �����蔻��̏�����.
	
	return true;
}

// �X�V�֐�.
void CBoss::Update( const float& deltaTime )
{
	m_DeltaTime = deltaTime;

	CurrentStateUpdate();	// ���݂̏�ԍX�V.
	//// �X�|�[��.
	//if ( CKeyInput::IsMomentPress( 'R' ) == true ) m_IsDelete = false;
	//// ���S.
	//if ( CKeyInput::IsMomentPress( 'F' ) == true ) m_IsDelete = true;
	//// ����.
	//if ( CKeyInput::IsMomentPress( 'T' ) == true ) m_IsFirght = true;

	DebugUpdate();			// �f�o�b�N�X�V.

	UpdateCollision();	// �����蔻��̍X�V.
}

// �`��֐�.
void CBoss::Render()
{
	// ��ʂ̊O�Ȃ�I��.
	if ( IsDisplayOut() == true ) return;

	m_pSkinMesh->SetPosition( m_Tranceform.Position );
	m_pSkinMesh->SetRotation( m_Tranceform.Rotation );
	m_pSkinMesh->SetScale( m_Tranceform.Scale );
	if ( m_IsDelete == false ) m_pSkinMesh->Render();
}

// �����蔻��֐�.
void CBoss::Collision( CActor* pActor )
{
	if ( pActor == nullptr ) return;
	if ( m_pCollisions == nullptr ) return;
	if ( m_pCollisions->GetCollision<CSphere>() == nullptr ) return;

	PlayerCollsion( pActor );
	EnemyCollsion( pActor );
	SpecialEnemy_1Collsion( pActor );
	SpecialEnemy_2Collsion( pActor );
}

// �����蔻��̏�����.
void CBoss::InitCollision()
{
	m_pCollisions = std::make_unique<CCollisions>();
	m_pCollisions->InitCollision( ECollNo::Sphere );
	m_pCollisions->GetCollision<CSphere>()->SetRadius( 1.5f );
}

// �����蔻��̍��W��A���a�Ȃǂ̍X�V.
//	Update�֐��̍Ō�ɌĂ�.
void CBoss::UpdateCollision()
{
	m_pCollisions->GetCollision<CSphere>()->SetPosition( D3DXVECTOR3( m_Tranceform.Position.x, m_Tranceform.Position.y, m_Tranceform.Position.z ) );
}

// �X�|�[��.
bool CBoss::Spawn( const D3DXVECTOR3& spawnPos )
{
	// ���ɃX�|�[���ς݂Ȃ�I��.
	if ( m_NowState != Boss::EBossState::None ) return true;
	m_Tranceform.Position = spawnPos;			// �X�|�[�����W�̐ݒ�.
	//m_LifePoint = pPARAMETER->LifeMax;		// �̗͂̐ݒ�.
	m_NowState = Boss::EBossState::Spawn;		// ���݂̏�Ԃ��X�|�[���ɕύX.
	return true;
}

// ������W�̐ݒ�.
void CBoss::SetTargetPos( CActor& actor )
{
	SetPlayerPos( actor );
}

// ���݂̏�Ԃ̍X�V�֐�.
void CBoss::CurrentStateUpdate()
{
	switch ( m_NowState )
	{
	case Boss::EBossState::Spawn:
		this->Spawning();
		break;
	case Boss::EBossState::Move:
		this->Move();
		break;
	case Boss::EBossState::Attack:
		this->Attack();
		break;
	case Boss::EBossState::Fright:
		this->Fright();
		break;
	case Boss::EBossState::Death:
		this->Death();
		break;
	default:
		break;
	}
	this->Fright();
}

// �v���C���[�̍��W��ݒ�.
void CBoss::SetPlayerPos( CActor& actor )
{
	//if( m_NowMoveState == Enemy::EMoveState::Move ) return;

	// �v���C���[����Ȃ���ΏI��.
	if ( actor.GetObjectTag() != EObjectTag::Player ) return;
	m_TargetPosition = actor.GetPosition();	// �v���C���[�̍��W���擾.
	// �ړI�̉�]�����擾.
	m_TargetRotation.y = atan2f(
		m_Tranceform.Position.x - m_TargetPosition.x,
		m_Tranceform.Position.z - m_TargetPosition.z );
}

// �ړ��x�N�g���ݒ�֐�.
void CBoss::SetMoveVector( const D3DXVECTOR3& targetPos )
{
	if ( m_NowState == Boss::EBossState::Fright ) return;

	// �ړI�̉�]�����擾.
	m_TargetRotation.y = atan2f(
		m_Tranceform.Position.x - targetPos.x,
		m_Tranceform.Position.z - targetPos.z );

	// �ړ��p�x�N�g�����擾.
	m_MoveVec3.x = sinf( m_TargetRotation.y );
	m_MoveVec3.z = cosf( m_TargetRotation.y );
}

// �ړI�̍��W�։�].
void CBoss::TargetRotation( const float& moveSpeed )
{
	if ( m_NowState == Boss::EBossState::Fright ) return;
	if ( m_NowMoveState != Boss::EMoveState::Rotation ) return;

	// ���g�̃x�N�g����p��.
	D3DXVECTOR3 myVector = { 0.0f, 0.0f ,0.0f };
	myVector.x = sinf( m_Tranceform.Rotation.y );
	myVector.z = cosf( m_Tranceform.Rotation.y );

	// �ړI�̍��W�֌����ĉ�].
	if ( CActor::TargetRotation( m_MoveVec3, 0.05f, TOLERANCE_RADIAN, moveSpeed ) == false ) return;
	m_Tranceform.Rotation.y = m_TargetRotation.y;		// �^�[�Q�b�g�ւ̉�]�擾.
	m_BeforeMoveingPosition = m_Tranceform.Position;	// ���݂̍��W���L��.
	m_NowMoveState = Boss::EMoveState::Move;			// �ړ���Ԃ֑J��.
}

// �����_���ړ��x�N�g���֐�.
void CBoss::SetRandMoveVector( const D3DXVECTOR3 & targetPos )
{
	if ( m_NowState == Boss::EBossState::Fright ) return;

	// �ړI�̉�]�����擾.
	m_RandRot.y = atan2f(
		m_Tranceform.Position.x - targetPos.x,
		m_Tranceform.Position.z - targetPos.z );

	// �ړ��p�x�N�g�����擾.
	m_MoveVec3.x = sinf( m_RandRot.y );
	m_MoveVec3.z = cosf( m_RandRot.y );
}

// �����_���̖ړI�̍��W�։�].
void CBoss::RandTargetRotation( const float& moveSpeed )
{
	if ( m_NowMoveState != Boss::EMoveState::Rotation ) return;

	// ���g�̃x�N�g����p��.
	D3DXVECTOR3 myVector = { 0.0f, 0.0f ,0.0f };
	myVector.x = sinf( m_Tranceform.Rotation.y );
	myVector.z = cosf( m_Tranceform.Rotation.y );

	// �ړI�̍��W�֌����ĉ�].
	if ( CActor::TargetRotation( m_MoveVec3, 0.05f, TOLERANCE_RADIAN, moveSpeed ) == false ) return;
	m_Tranceform.Rotation.y = m_RandRot.y;			// �^�[�Q�b�g�ւ̉�]�擾.
	m_NowMoveState = Boss::EMoveState::Move;		// �ړ���Ԃ֑J��.
}

// �ړ��֐�.
void CBoss::VectorMove( const float& moveSpeed, const float& length )
{
#if 1
	if ( m_NowMoveState != Boss::EMoveState::Move ) return;

	// �ړI�̍��W�֌����ĉ�].
	if ( CActor::TargetRotation( m_MoveVec3, 0.05f, TOLERANCE_RADIAN, moveSpeed ) == false ) return;
	m_Tranceform.Rotation.y = m_TargetRotation.y;		// �^�[�Q�b�g�ւ̉�]�擾.

	// �v���C���[�̍��W�ƓG�̍��W���r.
	// �G����苗������O�ɐi�߂����Ȃ�����.
	if ( D3DXVec3Length( &D3DXVECTOR3( m_TargetPosition - m_Tranceform.Position ) ) >= length ) {
		// �x�N�g�����g�p���Ĉړ�.
		m_Tranceform.Position.x -= m_MoveVec3.x * moveSpeed;
		m_Tranceform.Position.z -= m_MoveVec3.z * moveSpeed;
	}

	// �v���C���[�̍��W�ƓG�̍��W���r.
	if ( D3DXVec3Length( &D3DXVECTOR3( m_TargetPosition - m_Tranceform.Position ) ) >= length ) return;
	m_NowMoveState = Boss::EMoveState::Attack;

#else
	if (m_NowMoveState != Enemy::EMoveState::Move) return;

	// �x�N�g�����g�p���Ĉړ�.
	m_Tranceform.Position.x -= m_MoveVec3.x * moveSpeed;
	m_Tranceform.Position.z -= m_MoveVec3.z * moveSpeed;

	// �ēx�ړI�̍��W��T������r.
	// ��]���ɋL���������W�ƌ��݂̍��W�̋��������ȏ�Ȃ�.
	if (D3DXVec3Length(&D3DXVECTOR3(m_BeforeMoveingPosition - m_Tranceform.Position)) >= 10.0f)
		m_NowMoveState = Enemy::EMoveState::Rotation;	// ��]��Ԃֈړ�.

	if (D3DXVec3Length(&D3DXVECTOR3(m_TargetPosition - m_Tranceform.Position)) >= 10.0f) return;
	// ���݂̍��W�ƖړI�̍��W�̋��������ȏ�Ȃ�.
	m_NowMoveState = Enemy::EMoveState::Wait;	// �ҋ@��Ԃ֑J��.
#endif
}

// �ҋ@�֐�.
void CBoss::WaitMove()
{
	if (m_NowMoveState != Boss::EMoveState::Wait) return;
	m_WaitCount++;	// �ҋ@�J�E���g���Z.
	if (m_WaitCount < 3.0*FPS) return;
	m_NowMoveState = Boss::EMoveState::Rotation;	// �ړ���Ԃ���]����.
	m_WaitCount = 0;	// �ҋ@�J�E���g�̏�����.
}

// �X�|�[����.
void CBoss::Spawning()
{
	m_NowState = Boss::EBossState::Move;
	m_NowMoveState = Boss::EMoveState::Rotation;
}

// �ړ�.
void CBoss::Move()
{
	SearchRange();	// �v���C���[��_�����G����.
	// �v���C���[��ǐ�.
	if ( m_HasAimPlayer == true ) {
		TargetRotation( m_MoveSpeed );				// ��].
		VectorMove( m_MoveSpeed, ATTACK_LENGTH );	// �ړ�.
		Attack();									// �U��.
		WaitMove();									// �ҋ@.
	}
	// �v���C���[����������.
	else {
		if ( m_IsRandMove == false ) {
			Rand_Pos();		// �����_���ɍ��W�����߂�.
		}
		else {
			RandTargetRotation( m_MoveSpeed );	// ��].
			RandVectorMove( m_MoveSpeed );		// �ړ�.
			WaitMove();							// �ҋ@.
		}
	}
}

// �U��.
void CBoss::Attack()
{
	if ( m_NowMoveState != Boss::EMoveState::Attack ) return;
	if ( m_IsRandAttack != true ) Rand_Attack();
	m_IsRandAttack = 2;
	switch ( m_RandAttack )
	{
	case 1:		// ���݂��U��.
		if ( m_AttackWave == 0 ) {
			if ( m_Tranceform.Position.y <= 4.0f ) {
				m_Tranceform.Position.y += 0.1f;

				if ( m_TargetPosition != m_Tranceform.Position ) {
					// �x�N�g�����g�p���Ĉړ�.
					m_Tranceform.Position.x -= m_MoveVec3.x * 0.1f;
					m_Tranceform.Position.z -= m_MoveVec3.z * 0.1f;
				}
			}
			else {
				m_AttackWave++;
			}
		}

		if ( m_AttackWave == 1 ) {
			if ( m_Tranceform.Position.y >= 0.0f ) {
				m_Tranceform.Position.y -= 0.1f;
			}
			else {
				m_AttackWave++;
			}
		}

		if ( m_AttackWave >= 2 ) {
			m_IsRandAttack = false;
		}

		break;
	case 2:		// �K���ガ�����U��.
		if ( m_AttackWave == 0 ) {
			if ( m_Tranceform.Rotation.y >= -10.0f ) {
				m_Tranceform.Rotation.y -= 0.1f;
			}
			else {
				m_AttackWave++;
			}
		}

		if (m_AttackWave >= 1) {
			m_IsRandAttack = false;
		}

		break;
	case 3:		// �u���X�U��.
		m_IsRandAttack = false;
		break;
	case 4:		// �΋��U��.
		m_IsRandAttack = false;
		break;
	default:
		break;
	}

	// �U�����s������ҋ@��ԂɑJ�ڂ���.
	if ( m_IsRandAttack == true ) return;
	m_AttackCount++;	// �U�����s��������J�E���g����.
	m_NowMoveState = Boss::EMoveState::Wait;
}

// ����.
void CBoss::Fright()
{
	if ( m_IsFirght == false ) return;
	m_FrightCount++;
	const float FRIGHT_SPEED = 0.15f;
	m_Tranceform.Position.x -= sinf( static_cast<float>( D3DX_PI )		* static_cast<float>( m_FrightCount ) ) * FRIGHT_SPEED;
	m_Tranceform.Position.z -= sinf( static_cast<float>( D3DX_PI )*0.5f	* static_cast<float>( m_FrightCount ) ) * FRIGHT_SPEED;


	if ( m_FrightCount <= ( 1*0.5f )*FPS ) return;

	m_IsFirght = false;
	m_FrightCount = 0;
}

// ���S.
void CBoss::Death()
{
	//if (m_NowAnimNo != alien::EAnimNo_Dead) {
	//	m_NowAnimNo = alien::EAnimNo_Dead;
	//	m_AnimSpeed = DEFAULT_ANIM_SPEED;
	//}
	//if (m_AnimFrameList[alien::EAnimNo_Dead].IsNowFrameOver() == false) return;

	//m_AnimSpeed = 0.0;
	//m_DeathCount += pPARAMETER->DeadCountAddValue;
	//m_DeathScale -= pPARAMETER->DeadScaleSubValue;
	//// ���f���̃T�C�Y�̌v�Z.
	//const float scale = m_DeathScale * exp(-m_DeathCount)*sinf(DEATH_SCALE_PI*m_DeathCount);
	//m_vScale = { scale, scale, scale };

	//// �傫�������l�ȏ�Ȃ�.
	//if (m_vScale.x > SCALE_MIN) return;
	//m_vScale = { SCALE_MIN, SCALE_MIN, SCALE_MIN };
	//CSoundManager::PlaySE("AlienDead");
	//m_IsDelete = true;	// ���S�t���O�𗧂Ă�.

}

// �U�����@�������_���Ō��߂�֐�.
void CBoss::Rand_Attack()
{
	//�����𐶐����邽�߂̃R�[�h.
	std::random_device rnd;
	std::mt19937_64 mt( rnd() );

	//�����͈̔͂����߂�R�[�h.
	std::uniform_int_distribution<> rand_Attack( 1, 4 );

	m_RandAttack = rand_Attack( mt );

	m_AttackWave = 0;
	m_IsRandAttack = true;
}

// �����_���ɍ��W�����֐�.
void CBoss::Rand_Pos()
{
	//�����𐶐����邽�߂̃R�[�h.
	std::random_device rnd_x;
	std::random_device rnd_z;
	std::mt19937_64 mt_x( rnd_x() );
	std::mt19937_64 mt_z( rnd_z() );

	//�����͈̔͂����߂�R�[�h.
	std::uniform_int_distribution<> rand_Pos( -30, 30 );

	m_RandPos.x = rand_Pos( mt_x );
	m_RandPos.z = rand_Pos( mt_z );

	m_OldRandPos = m_RandPos;
	m_IsRandMove = true;
}

// �����_���ړ��֐�.
void CBoss::RandVectorMove( const float & moveSpeed )
{
	if ( m_NowMoveState != Boss::EMoveState::Move ) return;

	// �����_�����W�Ƃ̋������v�Z.
	float RandPosLength = D3DXVec3Length( &D3DXVECTOR3( m_RandPos - m_Tranceform.Position ) );

	// �x�N�g�����g�p���Ĉړ�.
	m_Tranceform.Position.x -= m_MoveVec3.x * moveSpeed;
	m_Tranceform.Position.z -= m_MoveVec3.z * moveSpeed;

	if ( RandPosLength <= 0.1f ) {
		m_IsRandMove = false;
		m_NowMoveState = Boss::EMoveState::Wait;	// �ҋ@��Ԃ֑J��.
	}
}

// �v���C���[�Ƃ̓����蔻��.
void CBoss::PlayerCollsion( CActor* pActor )
{
	// �I�u�W�F�N�g�̃^�O���v���C���[����Ȃ���ΏI��.
	if ( pActor->GetObjectTag() != EObjectTag::Player ) return;

	// �{�X�ƃv���C���[�̓����蔻��̎���.
	if ( coll::IsSphereToSphere( m_pCollisions->GetCollision<CSphere>(), pActor->GetCollision<CSphere>() ) == false ) return;

	float Radius = pActor->GetCollision<CSphere>()->GetRadius();	//���a.

	D3DXVECTOR3 Pos[2]{ m_Tranceform.Position, pActor->GetPosition() };

	D3DXVECTOR3 Distance = Pos[0] - Pos[1];// �{�X�ƃv���C���[�̋����ivector�j

	float inDistance = ( Radius )-( D3DXVec3Length( &Distance ) );// �{�X�̎h�����Ă��钷��

	D3DXVECTOR3 vecPushOut;// �����o��vector
	D3DXVec3Normalize( &vecPushOut, &Distance );// �o0 - �o1�̐��K���޸�� 

	// �޸�قɒ����������Ďh�����������݂��������߂�
	Pos[0].x -= vecPushOut.x * inDistance / 8;
	Pos[0].z -= vecPushOut.z * inDistance / 8;
	Pos[1].x -= -vecPushOut.x * inDistance / 8;
	Pos[1].z -= -vecPushOut.z * inDistance / 8;

	//m_Tranceform.Position = Pos[0];
	pActor->SetPosition( Pos[1] );
}

// �G���G�Ƃ̓����蔻��.
void CBoss::EnemyCollsion( CActor * pActor )
{
	// �I�u�W�F�N�g�̃^�O���G���G����Ȃ���ΏI��.
	if ( pActor->GetObjectTag() != EObjectTag::Enemy ) return;

	// �{�X�ƎG���G�̓����蔻��̎���.
	if ( coll::IsSphereToSphere( m_pCollisions->GetCollision<CSphere>(), pActor->GetCollision<CSphere>() ) == false ) return;

	float Radius = pActor->GetCollision<CSphere>()->GetRadius();	//���a.

	D3DXVECTOR3 Pos[2]{ m_Tranceform.Position, pActor->GetPosition() };

	D3DXVECTOR3 Distance = Pos[0] - Pos[1];// �{�X�ƎG���G�̋����ivector�j

	float inDistance = ( Radius )-( D3DXVec3Length( &Distance ) );// �{�X�̎h�����Ă��钷��

	D3DXVECTOR3 vecPushOut;// �����o��vector
	D3DXVec3Normalize( &vecPushOut, &Distance );// �o0 - �o1�̐��K���޸�� 

	// �޸�قɒ����������Ďh�����������݂��������߂�
	Pos[0].x -= vecPushOut.x * inDistance / 8;
	Pos[0].z -= vecPushOut.z * inDistance / 8;
	Pos[1].x -= -vecPushOut.x * inDistance / 8;
	Pos[1].z -= -vecPushOut.z * inDistance / 8;

	//m_Tranceform.Position = Pos[0];
	pActor->SetPosition( Pos[1] );
}

// ����ȓG1�Ƃ̓����蔻��.
void CBoss::SpecialEnemy_1Collsion( CActor * pActor )
{
	// �I�u�W�F�N�g�̃^�O������ȓG1����Ȃ���ΏI��.
	if ( pActor->GetObjectTag() != EObjectTag::SpecialEnemy_1 ) return;

	// �{�X�Ɠ���ȓG1�̓����蔻��̎���.
	if ( coll::IsSphereToSphere( m_pCollisions->GetCollision<CSphere>(), pActor->GetCollision<CSphere>() ) == false ) return;

	float Radius = pActor->GetCollision<CSphere>()->GetRadius();	// ���a.

	D3DXVECTOR3 Pos[2]{ m_Tranceform.Position, pActor->GetPosition() };

	D3DXVECTOR3 Distance = Pos[0] - Pos[1];// �{�X�Ɠ���ȓG1�̋����ivector�j

	float inDistance = ( Radius )-( D3DXVec3Length( &Distance ) );// �{�X�̎h�����Ă��钷��

	D3DXVECTOR3 vecPushOut;// �����o��vector
	D3DXVec3Normalize( &vecPushOut, &Distance );// �o0 - �o1�̐��K���޸�� 

	// �޸�قɒ����������Ďh�����������݂��������߂�
	Pos[0].x -= vecPushOut.x * inDistance / 8;
	Pos[0].z -= vecPushOut.z * inDistance / 8;
	Pos[1].x -= -vecPushOut.x * inDistance / 8;
	Pos[1].z -= -vecPushOut.z * inDistance / 8;

	//m_Tranceform.Position = Pos[0];
	pActor->SetPosition( Pos[1] );
}

// ����ȓG2�Ƃ̓����蔻��.
void CBoss::SpecialEnemy_2Collsion( CActor * pActor )
{
	// �I�u�W�F�N�g�̃^�O������ȓG2����Ȃ���ΏI��.
	if ( pActor->GetObjectTag() != EObjectTag::SpecialEnemy_2 ) return;

	// �{�X�Ɠ���ȓG2�̓����蔻��̎���.
	if ( coll::IsSphereToSphere( m_pCollisions->GetCollision<CSphere>(), pActor->GetCollision<CSphere>() ) == false ) return;

	float Radius = pActor->GetCollision<CSphere>()->GetRadius();	// ���a.

	D3DXVECTOR3 Pos[2]{ m_Tranceform.Position, pActor->GetPosition() };

	D3DXVECTOR3 Distance = Pos[0] - Pos[1];// �{�X�Ɠ���ȓG2�̋����ivector�j

	float inDistance = ( Radius )-( D3DXVec3Length( &Distance ) );// �{�X�̎h�����Ă��钷��

	D3DXVECTOR3 vecPushOut;// �����o��vector
	D3DXVec3Normalize( &vecPushOut, &Distance );// �o0 - �o1�̐��K���޸�� 

	// �޸�قɒ����������Ďh�����������݂��������߂�
	Pos[0].x -= vecPushOut.x * inDistance / 8;
	Pos[0].z -= vecPushOut.z * inDistance / 8;
	Pos[1].x -= -vecPushOut.x * inDistance / 8;
	Pos[1].z -= -vecPushOut.z * inDistance / 8;

	//m_Tranceform.Position = Pos[0];
	pActor->SetPosition( Pos[1] );
}

// ���G�͈�.
void CBoss::SearchRange()
{
	if ( m_NowMoveState == Boss::EMoveState::Attack ) return;

	// �v���C���[�Ƃ̋������v�Z.
	float PlayerLength = D3DXVec3Length( &D3DXVECTOR3( m_TargetPosition - m_Tranceform.Position ) );

	m_OldHasAimPlayer = m_HasAimPlayer;
	// �v���C���[�̑_���͈͂��r.
	if ( PlayerLength <= 30.0f ) {
		// ���G�͈͓��Ƀv���C���[������ꍇ.
		m_HasAimPlayer = true;
		m_IsRandMove = false;
		SetMoveVector( m_TargetPosition );
	}
	else {
		// ���G�͈͊O�̏ꍇ.
		m_HasAimPlayer = false;
		SetRandMoveVector( m_RandPos );
	}
	if ( m_OldHasAimPlayer == m_HasAimPlayer ) return;
	m_NowMoveState = Boss::EMoveState::Wait;
}

// �f�o�b�N�X�V�֐�.
void CBoss::DebugUpdate()
{
#ifdef _DEBUG
	CDebugText::PushText("Boss", "---------------------------------------------");
	CDebugText::PushText("Boss", "-------------------- Boss ------------------");
	CDebugText::PushText("Boss", "---------------------------------------------");
	CDebugText::PushText("Boss", "Pos : ", m_Tranceform.Position.x, ", ", m_Tranceform.Position.y, ", ", m_Tranceform.Position.z);
	CDebugText::PushText("Boss", "Rot : ", m_Tranceform.Rotation.x, ", ", m_Tranceform.Rotation.y, ", ", m_Tranceform.Rotation.z);
	CDebugText::PushText("Boss", "'R' IsDelete = false, 'F' IsDelete = true");
	CDebugText::PushText("Boss", "IsDelete : ", m_IsDelete == true ? "true" : "false");
	CDebugText::PushText("Boss", "m_HasAimPlayer : ", m_HasAimPlayer == true ? "true" : "false");
#endif
}


// �����Ń{�X�������֐�.
void CBoss::SetDelete( const std::function<void( bool& )>& proc )
{
	m_IsDelete = false;
	proc( m_IsDelete );
}
