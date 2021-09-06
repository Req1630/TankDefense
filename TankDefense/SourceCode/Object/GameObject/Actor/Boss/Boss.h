#ifndef BOSS_H
#define BOSS_H

#include "..\Actor.h"
#include "..\..\..\..\Common\Mesh\RayMesh\RayMesh.h"
#include <time.h>
#include <random>
#include <vector>

class CDX9SkinMesh;

namespace Boss		//��Ԃ̊i�[��.
{
	// �G�̏��.
	enum class enBossState : unsigned char
	{
		None,

		Spawn,
		Move,
		Attack,
		Fright,
		Death,

		Max,
	} typedef EBossState;

	// �ړ����.
	enum class enMoveState : unsigned char
	{
		None,

		Rotation,
		Move,
		Attack,
		Wait,

		Max,
	} typedef EMoveState;
}

class CBoss : public CActor
{
protected:
	inline static const float TOLERANCE_RADIAN			= static_cast<float>(D3DXToRadian(10.0));	// ��]�̋��e�͈�.
	inline static const float ATTACK_LENGTH				= 10.0f;	// �U�������钷��.
	inline static const float EXPLOSION_ATTACK_LENGTH	= 5.0f;		// �����U�������钷��.
	inline static const float RESTRAINT_ATTACK_LENGTH	= 5.0f;		// �S���U�������钷��.

public:
	// �{�X�p�����[�^.
	struct stParameter
	{
		float	LifeMax;		// HP.
		float	AttackPower;	// �U����.
		float	MoveSpeed;		// �ړ����x.
		stParameter()
			: LifeMax		( 1.0f )
			, AttackPower	( 0.5f )
			, MoveSpeed		( 0.05f )
		{}
	} typedef SParameter;

public:
	CBoss();
	virtual ~CBoss();

	// �������֐�.
	virtual bool Init() override;
	// �X�V�֐�.
	virtual void Update( const float& deltaTime ) override;
	// �`��֐�.
	virtual void Render() override;
	// �����蔻��֐�.
	virtual void Collision( CActor* pActor ) override;
	// �����蔻��̏�����.
	virtual void InitCollision() override;
	// �����蔻��̍��W��A���a�Ȃǂ̍X�V.
	//	Update�֐��̍Ō�ɌĂ�.
	virtual void UpdateCollision() override;

	// �X�|�[���̐ݒ�.
	virtual bool Spawn( const D3DXVECTOR3& spawnPos );

	// ������W�̐ݒ�.
	virtual void SetTargetPos( CActor& actor );

private:
	// ���݂̏�Ԃ̍X�V�֐�.
	void CurrentStateUpdate();
	// �v���C���[�̍��W��ݒ�.
	void SetPlayerPos( CActor& actor );

	// �ړ��x�N�g���ݒ�֐�.
	virtual void SetMoveVector( const D3DXVECTOR3& targetPos );
	// �ړI�̍��W�։�].
	void TargetRotation( const float& moveSpeed );
	// �����_���ړ��x�N�g���֐�.
	void SetRandMoveVector( const D3DXVECTOR3& targetPos );
	// �����_���̖ړI�̍��W�։�].
	void RandTargetRotation( const float& moveSpeed );
	// �ړ��֐�.
	virtual void VectorMove( const float& moveSpeed, const float& length );
	// �ҋ@�֐�.
	virtual void WaitMove();

	// �X�|�[����.
	virtual void Spawning();
	// �ړ�.
	virtual void Move();
	// �U��.
	virtual void Attack();
	// ����.
	virtual void Fright();
	// ���S.
	virtual void Death();

	// �U�����@�������_���Ō��߂�֐�.
	void Rand_Attack();

	// �����_���ɍ��W�����֐�.
	void Rand_Pos();
	// �����_���ړ��֐�.
	void RandVectorMove( const float& moveSpeed );

	// �v���C���[�Ƃ̓����蔻��.
	void PlayerCollsion( CActor* pActor );
	// �G���G�Ƃ̓����蔻��.
	void EnemyCollsion( CActor* pActor );
	// ����ȓG1�Ƃ̓����蔻��.
	void SpecialEnemy_1Collsion( CActor* pActor );
	// ����ȓG2�Ƃ̓����蔻��.
	void SpecialEnemy_2Collsion( CActor* pActor );

	// ���G�͈�.
	void SearchRange();

	// �f�o�b�N�X�V�֐�.
	void DebugUpdate();

	// �����Ń{�X�������֐�.
	virtual void SetDelete( const std::function<void( bool& )>& ) override;

private:
	CDX9SkinMesh*		m_pSkinMesh;

	D3DXVECTOR3			m_TargetPosition;			// �v���C���[�̍��W.
	D3DXVECTOR3			m_TargetRotation;			// �ڕW�̉�]���.
	D3DXVECTOR3			m_BeforeMoveingPosition;	// �ړ��O�̍��W.
	D3DXVECTOR3			m_MoveVec3;					// �ړ��x�N�g��.
	D3DXVECTOR3			m_RandPos;					// �����_�����W.
	D3DXVECTOR3			m_OldRandPos;				// �O��̃����_�����W.
	D3DXVECTOR3			m_RandRot;					// �����_����].
	Boss::EBossState	m_NowState;					// ���݂̏��.
	Boss::EMoveState	m_NowMoveState;				// ���݂̈ړ����.
	float				m_Life;						// HP.
	float				m_MoveSpeed;				// �ړ����x.
	float				m_DeathCount;				// ���S�J�E���g.
	int					m_FrightCount;				// ���݃J�E���g.
	int					m_HitCount;					// �q�b�g�J�E���g.
	int					m_WaitCount;				// �ҋ@�J�E���g.
	int					m_InvincibleCount;			// ���G�J�E���g.
	int					m_RandAttack;				// �U�����@�������_���Ō��߂�.
	int					m_AttackWave;				// �U���̗���.
	int					m_AttackCount;				// �U��������s������.
	bool				m_IsFirght;					// ���ݏ�Ԃ�.
	bool				m_IsExplosion;				// �������邩.
	bool				m_IsDelete;					// �������邩�ǂ���.
	bool				m_IsRandMove;				// �����_���ړ����Ă��邩�ǂ���.
	bool				m_IsRandAttack;				// �U�����@�������_���Ō��߂Ă��邩�ǂ���.
	bool				m_HasAimPlayer;				// �v���C���[��_�����ǂ���.
	bool				m_OldHasAimPlayer;			// �O��v���C���[��_���Ă�����.

};
#endif // !BOSS_H