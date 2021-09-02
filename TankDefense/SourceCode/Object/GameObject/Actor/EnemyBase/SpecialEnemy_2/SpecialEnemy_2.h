#ifndef SPECIALENEMY_2_H
#define SPECIALENEMY_2_H

#include "..\EnemyBase.h"
#include <time.h>
#include <random>
#include <vector>

class CRestraint;

class CSpecialEnemy_2 : public CEnemyBase
{

public:
	CSpecialEnemy_2();
	virtual ~CSpecialEnemy_2();

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
	// ����ȓG1�Ƃ̓����蔻��.
	void SpecialEnemy1Collsion( CActor* pActor );
	// �S���U���̓����蔻��.
	void RestraintCollsion( CActor* pActor );

private:
	CEnemyBase*		m_pEnemyBase;		// �G�x�[�X�N���X.
	std::shared_ptr<CRestraint>		m_pRestraint;	// �S���N���X.
	CSprite*		m_pSprite;			// �X�v���C�g�N���X

	int				m_ConfuCnt;			// �����J�E���g.
	bool			m_IsConfu;			// ������ԂɂȂ��Ă��邩�ǂ���.

};

#endif // !SPECIALENEMY_2_H
