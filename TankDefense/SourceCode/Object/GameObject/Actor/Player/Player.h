/**
* @file		Player.h.
* @brief	�v���C���[�N���X.
* @author	��c�F�P.
*/
#ifndef PLAYER_H
#define PLAYER_H

#include "..\Actor.h"

class CDX9StaticMesh;
class CRotLookAtCenter;
class CWeapon;
class CDefaultWepon;
class CMachineGun;
class CMissile;

/************************************************
*	�v���C���[�N���X.
**/
class CPlayer
	: public CActor
{
public:
	// �X�e�[�^�X.
	struct stStatus
	{
		float Hp;
		float Speed;

		stStatus()
			: Hp	( 0.0f )
			, Speed	( 0.0f )
		{}
	} typedef SStatus;

public:
	CPlayer();
	~CPlayer();

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

private:
	// ����֐�.
	void Controller();
	// �J�����̑���֐�.
	void CameraController();
	// �U���֐�.
	void AttackController();

	// �ړ��֐�.
	void Move();
	// �J�����̍X�V.
	void CameraUpdate();
	
protected:
	CDX9StaticMesh*						m_pStaticMesh;		// �v���C���[�̎ԑ̃��f��.
	std::unique_ptr<CRotLookAtCenter>	m_pLookCamera;		// �J����.
	std::shared_ptr<CWeapon>			m_pWeapon;			// ����.
	std::shared_ptr<CDefaultWepon>		m_pDefaultWepon;	// ��C.
	std::shared_ptr<CMachineGun>		m_pMachineGun;		// �}�V���K��.
	std::shared_ptr<CMissile>			m_pMissile;			// �~�T�C��.

	D3DXVECTOR3							m_CameraRot;		// �J�����̉�].
	D3DXVECTOR3							m_MoveVec;			// �ړ��x�N�g��.

	SStatus								m_Status;			// �X�e�[�^�X.
};

#endif	// #ifndef PLAYER_H.