#include "Enemy.h"
#include "..\..\..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"
#include "..\..\..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\Player\Player.h"

CEnemy::CEnemy()
	:m_pEnemyBase		()
	, m_vPlayerPos		( 0.0f, 0.0f, 0.0f )
	, m_HasAimPlayer	( false )
	, m_OldHasAimPlayer	( false )

{
	Init();
}

CEnemy::~CEnemy()
{
}

// �������֐�.
bool CEnemy::Init()
{
	m_pSkinMesh = CMeshResorce::GetSkin( "b_s" );
	m_pSkinMesh->SetAnimSpeed( GetDeltaTime<double>() );
	
	Spawn( D3DXVECTOR3( 0.0f, 0.0f ,0.0f ) );
	m_IsDelete = false;

	return true;
}

// �X�V�֐�.
void CEnemy::Update( const float& deltaTime )
{
	SetTargetPos( *m_pPlayer.get() );
	CurrentStateUpdate();
	// �X�|�[��.
	if (GetAsyncKeyState('R') & 0x0001)	m_IsDelete = false;

	// ���S.
	if (GetAsyncKeyState('D') & 0x0001)	m_IsDelete = true;
}

// �`��֐�.
void CEnemy::Render()
{
	// ��ʂ̊O�Ȃ�I��.
	if( IsDisplayOut() == true ) return;

	if( m_IsDelete == false ) m_pSkinMesh->Render();
}

// �����蔻��֐�.
void CEnemy::Collision( CActor * pActor )
{
	if( pActor == nullptr ) return;
	if( m_pCollisions == nullptr ) return;
	if( m_pCollisions->GetCollision<CSphere>() == nullptr ) return;
}

// �X�|�[��.
bool CEnemy::Spawn( const D3DXVECTOR3 & spawnPos )
{
	// ���ɃX�|�[���ς݂Ȃ�I��.
	if (m_NowState != Enemy::EEnemyState::None) return true;
	//m_vScale = { SCALE_MIN, SCALE_MIN, SCALE_MIN };
	m_Tranceform.Position = spawnPos;			// �X�|�[�����W�̐ݒ�.
	//m_LifePoint = pPARAMETER->LifeMax;		// �̗͂̐ݒ�.
	m_NowState = Enemy::EEnemyState::Spawn;		// ���݂̏�Ԃ��X�|�[���ɕύX.
	//m_AnimSpeed = 0.0;						// �A�j���[�V�������x���~�߂�.
	//m_pEffects[alien::EEffectNo_Spawn]->Play(m_vPosition);
	//m_pEffects[alien::EEffectNo_Spawn]->SetSpeed(0.5f);
	return true;
}

// ������W�̐ݒ�.
void CEnemy::SetTargetPos( CActor& actor )
{
	// �v���C���[�̍��W���擾.
	CEnemyBase::SetTargetPos( actor );
	// �v���C���[�̍��W���擾.
	CEnemy::SetPlayerPos( actor );
}

// �v���C���[���W�̐ݒ�.
void CEnemy::SetPlayerPos( CActor& actor )
{
	// �v���C���[����Ȃ���ΏI��.
	if (actor.GetObjectTag() != EObjectTag::Player) return;
	m_vPlayerPos = actor.GetPosition();	// �v���C���[�̍��W���擾.
}

// �X�|�[����.
void CEnemy::Spawning()
{
	CEnemyBase::Spawning();
}

// �ړ�.
void CEnemy::Move()
{
	AimPlayerDecision();	// �v���C���[��_��������.
	TargetRotation();		// ��].
	// �v���C���[��_���Ă��邩.
	if (m_HasAimPlayer == true) {
		CEnemy::VectorMove(m_MoveSpeed);	// �ړ�.
		Attack();					// �U��.
		CEnemyBase::WaitMove();		// �ҋ@.
	}
	else {
		CEnemyBase::VectorMove(m_MoveSpeed);		// �ړ�.
		CEnemyBase::WaitMove();		// �ҋ@.
	}
	if (m_NowMoveState == Enemy::EMoveState::Move) {
		//SetAnimation(alien::EAnimNo_Move, m_pAC);
	}

	//if (*m_pIsAlienOtherAbduct == false) return;
	//if (m_NowState == alien::EAlienState::Abduct) return;
	//m_pEffects[alien::EEffectNo_Attack]->Stop();
	//SetAnimation(alien::EAnimNo_Move, m_pAC);
	//m_NowState = alien::EAlienState::Escape;	// �������Ԃ֑J��.
	m_NowMoveState = Enemy::EMoveState::Rotation;	// �ړ���Ԃ���]�֑J�ڂ���.
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

// �ړ��֐�.
void CEnemy::VectorMove( const float & moveSpeed )
{
	if (m_NowMoveState != Enemy::EMoveState::Move) return;

	// �x�N�g�����g�p���Ĉړ�.
	m_Tranceform.Position.x -= m_MoveVec3.x * moveSpeed;
	m_Tranceform.Position.z -= m_MoveVec3.z * moveSpeed;

	// �v���C���[�̍��W�ƉF���l�̍��W���r.
	if (D3DXVec3Length(&D3DXVECTOR3(m_vPlayerPos - m_Tranceform.Position)) >= 1.0f) return;
	//if (m_IsBarrierHit == true) return;
	m_NowMoveState = Enemy::EMoveState::Attack;
	//m_RotAccValue = pPARAMETER->AttackRotInitPower;
	//m_pEffects[alien::EEffectNo_Attack]->Play(m_vPosition);
}

// �U���֐�.
void CEnemy::Attack()
{
}

// �v���C���[�Ƃ̓����蔻��.
void CEnemy::PlayerCollison( CActor * pActor )
{
}

// �v���C���[��_��������.
void CEnemy::AimPlayerDecision()
{
	if (m_NowMoveState == Enemy::EMoveState::Attack) return;

	// �v���C���[�Ƃ̋������v�Z.
	float playerLenght = D3DXVec3Length(&D3DXVECTOR3(m_vPlayerPos - m_Tranceform.Position));

	m_OldHasAimPlayer = m_HasAimPlayer;
	// �v���C���[�̑_���͈͂��r.
	if (playerLenght <= 3.0f) {
		// �v���C���[�̂ق����߂��̂Ńv���C���[��_��.
		m_HasAimPlayer = true;
		SetMoveVector(m_vPlayerPos);
	}
	//else {
	//	// ���̎q�̂ق���_��.
	//	m_HasAimPlayer = false;
	//	SetMoveVector(m_TargetPosition);
	//}
	if (m_OldHasAimPlayer == m_HasAimPlayer) return;
	m_NowMoveState = Enemy::EMoveState::Rotation;
}
