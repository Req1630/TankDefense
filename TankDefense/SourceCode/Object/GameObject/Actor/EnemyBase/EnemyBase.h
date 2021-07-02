#ifndef ENEMYBASE_H
#define ENEMYBASE_H

#include "..\Actor.h"
#include "..\..\..\..\Common\Mesh\RayMesh\RayMesh.h"

class CDX9SkinMesh;
class CPlayer;

namespace Enemy		//��Ԃ̊i�[��.
{
	// �G�̏��.
	enum class enEnemyState : unsigned char
	{
		None,

		Spawn,
		Move,
		Fright,
		Death,

		Max,
	} typedef EEnemyState;

	// �ړ����.
	enum class enMovState : unsigned char
	{
		None,

		Rotation,
		Move,
		Attack,
		Wait,

		Max,
	} typedef EMoveState;
}

class CEnemyBase : public CActor
{
protected:
	inline static const float TOLERANCE_RADIAN = static_cast<float>(D3DXToRadian(10.0));	// ��]�̋��e�͈�.

public:
	// �G�l�~�[�p�����[�^.
	struct stParameter
	{
		float	LifeMax;		// HP.
		float	AttackPower;	// �U����.
		float	MoveSpeed;		// �ړ����x.
		stParameter()
			: LifeMax		( 1.0f )
			, AttackPower	( 0.5f )
			, MoveSpeed		( 0.5f )
		{}
	} typedef SParameter;

public:
	CEnemyBase();
	~CEnemyBase();

	// �������֐�.
	virtual bool Init() override;
	// �X�V�֐�.
	virtual void Update( const float& deltaTime ) override;
	// �`��֐�.
	virtual void Render() override;

	// ������W�̐ݒ�.
	virtual void SetTargetPos( CActor& actor );
	// �x�N�g���̎擾.
	virtual void SetCVector( const D3DXVECTOR3& vec );

	// �X�|�[���̐ݒ�.
	virtual bool Spawn( const D3DXVECTOR3& spawnPos ) = 0;

	// �����蔻��֐�.
	virtual void Collision( CActor* pActor );

	// �����蔻��̏�����.
	virtual void InitCollision();
	// �����蔻��̍��W��A���a�Ȃǂ̍X�V.
	//	Update�֐��̍Ō�ɌĂ�.
	virtual void UpdateCollision();

protected:
	// ���݂̏�Ԃ̍X�V�֐�.
	void CurrentStateUpdate();
	// �v���C���[�̍��W��ݒ�.
	void SetPlayerPos( CActor& actor );
	//// ���W�ݒ�֐�.
	//virtual void SetPosition( const D3DXVECTOR3& vPos ) override;

	// �ړ��x�N�g���ݒ�֐�.
	virtual void SetMoveVector( const D3DXVECTOR3& targetPos );
	// �ړI�̍��W�։�].
	void TargetRotation();
	// �ړ��֐�.
	virtual void VectorMove( const float& moveSpeed );
	// �ҋ@�֐�.
	virtual void WaitMove();

	// �X�|�[����.
	virtual void Spawning();
	// �ړ�.
	virtual void Move();
	// ����.
	virtual void Fright();
	// ���S.
	virtual void Death();

protected:
	CDX9SkinMesh*		m_pSkinMesh;

	std::shared_ptr<CPlayer>	m_pPlayer;
	D3DXVECTOR3			m_TargetPosition;			// ���̎q�̍��W.
	D3DXVECTOR3			m_TargetRotation;			// �ڕW�̉�]���.
	D3DXVECTOR3			m_BeforeMoveingPosition;	// �ړ��O�̍��W.
	D3DXVECTOR3			m_pos;
	D3DXVECTOR3			m_Rot;
	D3DXVECTOR3			m_MoveVec3;
	Enemy::EEnemyState	m_NowState;					// ���݂̏��.
	Enemy::EMoveState	m_NowMoveState;				// ���݂̈ړ����.
	float				m_Life;						// HP.
	float				m_MoveSpeed;				// �ړ����x.
	float				m_DeathCount;				// ���S�J�E���g.
	int					m_FrightCount;				// ���݃J�E���g.
	int					m_HitCount;					// �q�b�g�J�E���g.
	int					m_WaitCount;				// �ҋ@�J�E���g.
	int					m_InvincibleCount;			// ���G�J�E���g.
	bool				m_IsFirght;					// ���ݏ�Ԃ�.
	bool				m_IsExplosion;				// �������邩.
	bool				m_IsDelete;					// �������邩�ǂ���.
};
#endif // !ENEMYBASE_H