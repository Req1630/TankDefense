#ifndef ENEMYBULLET_H
#define ENEMYBULLET_H

#include "..\Bullet.h"

class CEnemyBullet
	: public CBullet
{
public:
	CEnemyBullet();
	~CEnemyBullet();

	// �������֐�.
	virtual bool Init();
	// �X�V�֐�.
	virtual void Update( const float& deltaTime );
	// �`��֐�.
	virtual void Render();

	// �����蔻��֐�.
	virtual void Collision( CActor* pActor );

	// �����蔻��̏�����.
	virtual void InitCollision();
	// �����蔻��̍��W��A���a�Ȃǂ̍X�V.
	//	Update�֐��̍Ō�ɌĂ�.
	virtual void UpdateCollision();

	// �e�̕\���ݒ�.
	void SetBulletScale();
	// �e�̈ʒu�ݒ�.
	void SetBulletPos( D3DXVECTOR3 Pos, D3DXVECTOR3 MoveVec );
	void SetTargetPos( CActor& actor );
	// �e�̃t���O�擾.
	bool GetShotflg() { return m_ShotFlag; }
	void Setshotflg( bool shotflg ) { m_ShotFlag = shotflg; }

private:
	D3DXVECTOR3		m_TargetPosition;	// �U���Ώۂ̍��W.

	bool			m_ShotFlag;			// �e�����˂��ꂽ���ǂ���.
};
#endif	// #ifndef ENEMYBULLET_H.