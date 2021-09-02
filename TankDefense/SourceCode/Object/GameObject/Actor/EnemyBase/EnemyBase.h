#ifndef ENEMYBASE_H
#define ENEMYBASE_H

#include "..\Actor.h"
#include "..\..\..\..\Common\Mesh\RayMesh\RayMesh.h"
#include <time.h>
#include <random>
#include <vector>

class CDX9SkinMesh;

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

class CEnemyBase : public CActor
{
protected:
	inline static const float TOLERANCE_RADIAN			= static_cast<float>(D3DXToRadian(10.0));	// ��]�̋��e�͈�.
	inline static const float ATTACK_LENGTH				= 10.0f;	// �U�������钷��.
	inline static const float EXPLOSION_ATTACK_LENGTH	= 5.0f;		// �����U�������钷��.
	inline static const float RESTRAINT_ATTACK_LENGTH	= 5.0f;		// �S���U�������钷��.

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
			, MoveSpeed		( 0.05f )
		{}
	} typedef SParameter;

public:
	CEnemyBase();
	virtual ~CEnemyBase();

	// ������W�̐ݒ�.
	virtual void SetTargetPos( CActor& actor );

	// �X�|�[���̐ݒ�.
	virtual bool Spawn( const D3DXVECTOR3& spawnPos ) = 0;

protected:
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
	// ����.
	virtual void Fright();
	// ���S.
	virtual void Death();

	// �����_���ɍ��W�����֐�.
	void Rand_Pos();
	// �����_���ړ��֐�.
	void RandVectorMove( const float& moveSpeed );

	// �����ŎG���G�������֐�.
	virtual void SetDelete( const std::function<void( bool& )>& ) override;

protected:
	CDX9SkinMesh*		m_pSkinMesh;

	D3DXVECTOR3			m_TargetPosition;			// �v���C���[�̍��W.
	D3DXVECTOR3			m_TargetRotation;			// �ڕW�̉�]���.
	D3DXVECTOR3			m_BeforeMoveingPosition;	// �ړ��O�̍��W.
	D3DXVECTOR3			m_MoveVec3;					// �ړ��x�N�g��.
	D3DXVECTOR3			m_RandPos;					// �����_�����W.
	D3DXVECTOR3			m_OldRandPos;				// �O��̃����_�����W.
	D3DXVECTOR3			m_RandRot;					// �����_����].
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
	bool				m_IsRand;					// �����_���ړ����Ă��邩�ǂ���.
};
#endif // !ENEMYBASE_H