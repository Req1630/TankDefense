#ifndef SPECIALENEMY_1_H
#define SPECIALENEMY_1_H

#include "..\EnemyBase.h"

class CExplosion;

class CSpecialEnemy_1 : public CEnemyBase
{

public:
	CSpecialEnemy_1();
	virtual ~CSpecialEnemy_1();

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
	// �v���C���[�Ɣ����̓����蔻��.
	void ExplosionCollsion( CActor* pActor );
	// �G���G�Ɣ����̓����蔻��.
	void ExplosionEnemyCollsion( CActor* pActor );
	// ����ȓG2�Ɣ����̓����蔻��.
	void ExplosionSEnemy2Collsion( CActor* pActor );

private:
	CEnemyBase*		m_pEnemyBase;		// �G�x�[�X�N���X.
	std::shared_ptr<CExplosion>		m_pExplosion;	// �����N���X.

};

#endif // !SPECIALENEMY_1_H
