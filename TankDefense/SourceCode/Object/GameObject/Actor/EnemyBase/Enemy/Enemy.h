#ifndef ENEMY_H
#define ENEMY_H

#include "..\EnemyBase.h"

class CEnemy : public CEnemyBase
{
	const char* MODEL_NAME = "b_s";

public:
	CEnemy();
	virtual ~CEnemy();

	// �������֐�.
	virtual bool Init() override;
	// �X�V�֐�.
	virtual void Update( const float& deltaTime ) override;
	// �`��֐�.
	virtual void Render() override;
	// �����蔻��֐�.
	virtual void Collision( CActor* pActor ) override;
	// �X�|�[��.
	virtual bool Spawn( const D3DXVECTOR3& spawnPos ) override;
	// ������W�̐ݒ�.
	virtual void SetTargetPos( CActor& actor ) override;

private:
	// �v���C���[���W�̐ݒ�.
	void SetPlayerPos( CActor& actor );
	// �X�|�[����.
	virtual void Spawning() override;
	// �ړ�.
	virtual void Move() override;
	// ����.
	virtual void Fright() override;
	// ���S.
	virtual void Death() override;

	// �ړ��֐�.
	virtual void VectorMove( const float& moveSpeed ) override;
	// �U���֐�.
	void Attack();
	// �v���C���[�Ƃ̓����蔻��.
	void PlayerCollison( CActor* pActor );

	// �v���C���[��_��������.
	void AimPlayerDecision();

	// �����蔻��̐ݒ�.
	bool ColliderSetting();

private:
	CEnemyBase*		m_pEnemyBase;
	D3DXVECTOR3 m_vPlayerPos;	// �v���C���[�̍��W.
	bool	m_HasAimPlayer;		// �v���C���[��_�����ǂ���.
	bool	m_OldHasAimPlayer;	// �O��v���C���[��_���Ă�����.

};

#endif // !ENEMY_H
