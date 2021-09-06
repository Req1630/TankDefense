/**
* @file		Weapon.h.
* @brief	����x�[�X�N���X.
* @author	��c�F�P.
*/
#ifndef WEAPON_H
#define WEAPON_H

#include "..\Actor.h"
#include "ShotStatus.h"

class CDX9SkinMesh;
class CBulletManager;

/************************************************
*	����x�[�X�N���X.
**/
class CWeapon
	: public CActor
{
public:
	// ����̎��.
	enum enType : unsigned char
	{
		DefaultWepon,
		MachineGun,
		Missile,
		Beam
	} typedef EType;

public:
	CWeapon();
	virtual ~CWeapon();

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

	// �e�̔��ˊ֐�.
	virtual bool Shot( D3DXVECTOR3 MoveVec ) final;

	// �ړ��֐�.
	virtual void Move( D3DXVECTOR3 BodyPos, D3DXVECTOR3 Rot ) final;

	// �}�l�[�W���[�̐ݒ�.
	void SetBulletMng( const std::shared_ptr<CBulletManager> pBulletMng ) { m_pBulletMng = pBulletMng; }

protected:
	// �f�o�b�N�̍X�V�֐�.
	void DebugUpdate();

	// �쐬�֐�.
	virtual void Create() = 0;

protected:
	CDX9SkinMesh*					m_pSkinMesh_Body;	// �ԑ̃��f��.
	CDX9SkinMesh*					m_pSkinMesh_Weapon;	// ���탂�f��.
	EType							m_Type;				// ����̃^�C�v.
	SStatus							m_Status;			// �X�e�[�^�X.
	std::string						m_BoneName;			// �{�[�����O.
	std::string						m_BulletName;		// �e�̃��f���̖��O.
	std::string						m_ShotName;			// �e�̔��ˏꏊ�̃{�[���̖��O.

private:
	std::shared_ptr<CBulletManager>	m_pBulletMng;		// �e.
	float							m_ShotCnt;			// �e�Ԋu�p�J�E���g.
};

#endif	// #ifndef WEAPON_H.