/**
* @file		Weapon.h.
* @brief	����x�[�X�N���X.
* @author	��c�F�P.
*/
#ifndef WEAPON_H
#define WEAPON_H

#include "..\Actor.h"

class CDX9StaticMesh;
class CBullet;

/************************************************
*	����x�[�X�N���X.
**/
class CWeapon
	: public CActor
{
public:
	// ����̎��.
	enum enType
	{
		DefaultWepon,
		MachineGun,
		Missile,
		Beam
	} typedef EType;

	// �X�e�[�^�X.
	struct stStatus
	{
		int	  BulletsNum;	// �e��.
		float Attack;		// �U����.
		float Speed;		// ���x.
		float Interval;		// �Ԋu(�b).

		stStatus()
			: BulletsNum	( 0		)
			, Attack		( 0.0f	)
			, Speed			( 0.0f	)
			, Interval		( 0.0f	)
		{}
	} typedef SStatus;

public:
	CWeapon();
	virtual ~CWeapon();

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

	// �e�̔���.
	void Shot( D3DXVECTOR3 MoveVec );

	// �ړ�.
	void Move( D3DXVECTOR3 Pos, D3DXVECTOR3 Rot );

protected:
	CDX9StaticMesh*							m_pStaticMesh;	// ���탂�f��.

	std::vector<std::unique_ptr<CBullet>>	m_pBullet;		// �e.
	EType									m_Type;			// ����̃^�C�v.
	int										m_ListSize;		// �z��̃T�C�Y.

	SStatus									m_Status;		// �X�e�[�^�X.
	float									m_ShotCnt;		// �e�Ԋu�p�J�E���g.

};

#endif	// #ifndef WEAPON_H.