/**
* @file		Player.h.
* @brief	�v���C���[�N���X.
* @author	��c�F�P.
*/
#ifndef PLAYER_H
#define PLAYER_H

#include "..\Actor.h"

class CDX9SkinMesh;
class CRotLookAtCenter;
class CBulletManager;
class CWeapon;
class CDefaultWepon;
class CMachineGun;
class CMissile;
class CBeam;

/************************************************
*	�v���C���[�N���X.
**/
class CPlayer final
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
	CPlayer( const std::shared_ptr<CBulletManager> pBulletMng );
	~CPlayer();

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

	// ����̕ύX�֐�.
	void ChangeWeapon( const std::shared_ptr<CWeapon> pWeapon );

private:
	// �쐬�֐�.
	void Create();

	// ����֐�.
	void Controller();
	// �J�����̑���֐�.
	void CameraController();
	// �U���֐�.
	void AttackController();

	// �ړ��֐�.
	void Move();
	// �J�����̍X�V�֐�.
	void CameraUpdate();
	
	// �f�o�b�N�̍X�V�֐�.
	void DebugUpdate();

protected:
	CDX9SkinMesh*						m_pSkinMesh;		// �v���C���[�̎ԑ̃��f��.
	std::unique_ptr<CRotLookAtCenter>	m_pLookCamera;		// �J����.
	std::shared_ptr<CBulletManager>		m_pBulletMng;		// �e�}�l�[�W���[.
	std::shared_ptr<CWeapon>			m_pWeapon;			// ����.
	std::shared_ptr<CDefaultWepon>		m_pDefaultWepon;	// ��C.

	D3DXVECTOR3							m_CameraRot;		// �J�����̉�].
	D3DXVECTOR3							m_MoveVec;			// �ړ��x�N�g��.
	D3DXVECTOR3							m_DireVec;			// �����x�N�g��.

	D3DXVECTOR3							m_AlignmentPos;		// �Ə��̈ʒu.

	SStatus								m_Status;			// �X�e�[�^�X.
};

#endif	// #ifndef PLAYER_H.