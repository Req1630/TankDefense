#ifndef ENEMY_H
#define ENEMY_H

#include "..\EnemyBase.h"

class CEnemyBullet;

class CEnemy : public CEnemyBase
{
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
	// �����蔻��̏�����.
	virtual void InitCollision() override;
	// �����蔻��̍��W��A���a�Ȃǂ̍X�V.
	//	Update�֐��̍Ō�ɌĂ�.
	virtual void UpdateCollision() override;
	// �X�|�[��.
	virtual bool Spawn( const D3DXVECTOR3& spawnPos ) override;
	// ������W�̐ݒ�.
	virtual void SetTargetPos( CActor& actor ) override;
	// �G�l�~�[���W�̐ݒ�.
	void SetEnemyPos(CActor& actor);

private:
	// �X�|�[����.
	virtual void Spawning() override;
	// �ړ�.
	virtual void Move() override;
	// ����.
	virtual void Fright() override;
	// ���S.
	virtual void Death() override;

	// �U���֐�.
	void Attack();
	// �v���C���[�Ƃ̓����蔻��.
	void PlayerCollsion( CActor* pActor );
	// ����ȓG1�Ƃ̓����蔻��.
	void SpecialEnemy_1Collsion( CActor* pActor );
	// ����ȓG2�Ƃ̓����蔻��.
	void SpecialEnemy_2Collsion( CActor* pActor );
	// �v���C���[�ƎG���G�̒e�̓����蔻��.
	void EnemyBulletCollsion( CActor* pActor );

	// ���G�͈�.
	void SearchRange();

	// �f�o�b�N�X�V�֐�.
	void DebugUpdate();

private:
	CEnemyBase*		m_pEnemyBase;		// �G�x�[�X�N���X.
	std::shared_ptr<CEnemyBullet>	m_pEnemyBullet;	// �G���G�̒e�N���X.

	bool			m_HasAimPlayer;		// �v���C���[��_�����ǂ���.
	bool			m_OldHasAimPlayer;	// �O��v���C���[��_���Ă�����.

};

#endif // !ENEMY_H
