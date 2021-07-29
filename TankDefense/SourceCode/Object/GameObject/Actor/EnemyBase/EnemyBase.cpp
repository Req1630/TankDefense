#include "EnemyBase.h"
#include "..\..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"
#include "..\..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\Player\Player.h"

CEnemyBase::CEnemyBase()
	: m_pSkinMesh				( nullptr )
	, m_pPlayer					( nullptr )
	
	, m_TargetPosition			( 0.0f, 0.0f, 0.0f )
	, m_TargetRotation			( 0.0f, 0.0f, 0.0f )
	, m_BeforeMoveingPosition	( 0.0f, 0.0f, 0.0f )
	, m_pos						( 0.0f, 0.0f, 0.0f )
	, m_Rot						( 0.0f, 0.0f, 0.0f )
	, m_MoveVec3				( 0.0f, 0.0f, 0.0f )
	, m_NowState				( Enemy::EEnemyState::None )
	, m_NowMoveState			( Enemy::EMoveState::None )
	, m_Life					( 0.0f )
	, m_MoveSpeed				( 1.0f )
	, m_DeathCount				( 0.0f )
	, m_FrightCount				( 0 )
	, m_HitCount				( 0 )
	, m_WaitCount				( 0 )
	, m_InvincibleCount			( 0 )
	, m_IsFirght				( false )
	, m_IsExplosion				( false )
	, m_IsDelete				( false )
{
	m_pPlayer = std::make_shared<CPlayer>();
}

CEnemyBase::~CEnemyBase()
{
}

// �������֐�.
bool CEnemyBase::Init()
{
	return false;
}

// �X�V�֐�.
void CEnemyBase::Update( const float & deltaTime )
{
	
}

// �`��֐�.
void CEnemyBase::Render()
{
	m_pSkinMesh->Render();
}

// ������W�̐ݒ�.
void CEnemyBase::SetTargetPos( CActor & actor )
{
	SetPlayerPos( actor );
}

// �x�N�g���̎擾.
void CEnemyBase::SetCVector( const D3DXVECTOR3 & vec )
{
	if (m_NowState == Enemy::EEnemyState::Spawn) return;
	if (m_NowState == Enemy::EEnemyState::Death) return;
}

// �����蔻��֐�.
void CEnemyBase::Collision( CActor * pActor )
{
}

// �����蔻��̏�����.
void CEnemyBase::InitCollision()
{
}

// �����蔻��̍��W��A���a�Ȃǂ̍X�V.
//	Update�֐��̍Ō�ɌĂ�.
void CEnemyBase::UpdateCollision()
{
}

// ���݂̏�Ԃ̍X�V�֐�.
void CEnemyBase::CurrentStateUpdate()
{
	switch ( m_NowState )
	{
	case Enemy::EEnemyState::Spawn:
		this->Spawning();
		break;
	case Enemy::EEnemyState::Move:
		this->Move();
		break;
	case Enemy::EEnemyState::Fright:
		this->Fright();
		break;
	case Enemy::EEnemyState::Death:
		this->Death();
		break;
	default:
		break;
	}
}

// �v���C���[�̍��W��ݒ�.
void CEnemyBase::SetPlayerPos( CActor & actor )
{
	if( m_NowMoveState == Enemy::EMoveState::Move ) return;

	// �v���C���[����Ȃ���ΏI��.
	if (actor.GetObjectTag() != EObjectTag::Player) return;
	m_TargetPosition = actor.GetPosition();	// �v���C���[�̍��W���擾.
	// �ړI�̉�]�����擾.
	m_TargetRotation.y = atan2f(
		m_Tranceform.Position.x - m_Tranceform.Position.x,
		m_Tranceform.Position.z - m_Tranceform.Position.z);

	// �v���C���[�ƈ��̋����u�������W��ݒ�.
	m_TargetPosition.x += sinf(m_TargetRotation.y);
	m_TargetPosition.z += cosf(m_TargetRotation.y);
}

// �ړ��x�N�g���ݒ�֐�.
void CEnemyBase::SetMoveVector( const D3DXVECTOR3 & targetPos )
{
	if (m_NowState == Enemy::EEnemyState::Fright) return;

	// �ړI�̉�]�����擾.
	m_TargetRotation.y = atan2f(
		m_Tranceform.Position.x - targetPos.x,
		m_Tranceform.Position.z - targetPos.z);

	D3DXVECTOR3 m_MoveVector;
	// �ړ��p�x�N�g�����擾.
	m_MoveVector.x = sinf(m_TargetRotation.y);
	m_MoveVector.z = cosf(m_TargetRotation.y);
}

// �ړI�̍��W�։�].
void CEnemyBase::TargetRotation()
{
	if (m_NowMoveState != Enemy::EMoveState::Rotation) return;

	// ���g�̃x�N�g����p��.
	D3DXVECTOR3 myVector = { 0.0f, 0.0f ,0.0f };
	myVector.x = sinf(m_Tranceform.Rotation.y);
	myVector.z = cosf(m_Tranceform.Rotation.y);

	// �ړI�̍��W�֌����ĉ�].
	if (CActor::TargetRotation( m_MoveVec3, 1.0f, TOLERANCE_RADIAN) == false ) return;
	m_Tranceform.Rotation.y = m_TargetRotation.y;					// �^�[�Q�b�g�ւ̉�]�擾.
	m_BeforeMoveingPosition = m_Tranceform.Position;	// ���݂̍��W���L��.
	m_NowMoveState = Enemy::EMoveState::Move;			// �ړ���Ԃ֑J��.
}

// �ړ��֐�.
void CEnemyBase::VectorMove( const float & moveSpeed )
{
	if (m_NowMoveState != Enemy::EMoveState::Move) return;

	// �x�N�g�����g�p���Ĉړ�.
	m_Tranceform.Position.x -= m_MoveVec3.x * moveSpeed;
	m_Tranceform.Position.z -= m_MoveVec3.z * moveSpeed;

	// �ēx�ړI�̍��W��T������r.
	// ��]���ɋL���������W�ƌ��݂̍��W�̋��������ȏ�Ȃ�.
	if (D3DXVec3Length(&D3DXVECTOR3(m_BeforeMoveingPosition - m_Tranceform.Position)) >= 2.0f)
		m_NowMoveState = Enemy::EMoveState::Rotation;	// ��]��Ԃֈړ�.

	if (D3DXVec3Length(&D3DXVECTOR3(m_TargetPosition - m_Tranceform.Position)) >= 1.0f) return;
	// ���݂̍��W�ƖړI�̍��W�̋��������ȏ�Ȃ�.
	m_NowMoveState = Enemy::EMoveState::Wait;	// �ҋ@��Ԃ֑J��.
}

// �ҋ@�֐�.
void CEnemyBase::WaitMove()
{
	if (m_NowMoveState != Enemy::EMoveState::Wait) return;
	m_WaitCount++;	// �ҋ@�J�E���g���Z.
	if (m_WaitCount < 3*FPS) return;
	m_NowMoveState = Enemy::EMoveState::Rotation;	// �ړ���Ԃ���]����.
	m_WaitCount = 0;	// �ҋ@�J�E���g�̏�����.
}

// �X�|�[����.
void CEnemyBase::Spawning()
{
	m_NowState = Enemy::EEnemyState::Move;
}

// �ړ�.
void CEnemyBase::Move()
{
	TargetRotation();			// ��].
	CEnemyBase::VectorMove(m_MoveSpeed);	// �ړ�.
	CEnemyBase::WaitMove();			// �ҋ@.

	//if (*m_pIsAlienOtherAbduct == false) return;
	//if (m_NowState == Enemy::EAlienState::Abduct) return;
	//SetAnimation(alien::EAnimNo_Move, m_pAC);
	//m_NowState = Enemy::EAlienState::Escape;
	m_NowMoveState = Enemy::EMoveState::Rotation;	// �ړ���Ԃ���]����.
}

// ����.
void CEnemyBase::Fright()
{
	if (m_IsFirght == false) return;
	m_FrightCount++;
	const float FRIGHT_SPEED = 0.15f;
	m_Tranceform.Position.x -= sinf(static_cast<float>(D3DX_PI)		* static_cast<float>(m_FrightCount)) * FRIGHT_SPEED;
	m_Tranceform.Position.z -= sinf(static_cast<float>(D3DX_PI)*0.5f	* static_cast<float>(m_FrightCount)) * FRIGHT_SPEED;


	if (m_FrightCount <= (1*0.5f)*FPS) return;

	m_IsFirght = false;
	m_FrightCount = 0;
}

// ���S.
void CEnemyBase::Death()
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
