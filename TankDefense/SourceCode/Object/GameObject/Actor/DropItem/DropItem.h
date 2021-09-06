/**
* @file		DropItem.h.
* @brief	�h���b�v�A�C�e���x�[�X�N���X.
* @author	��c�F�P.
*/
#ifndef DROPITEM_H
#define DROPITEM_H

#include "..\Actor.h"

class CDX9StaticMesh;
class CPlayer;
class CWeapon;

/************************************************
*	�h���b�v�A�C�e���N���X.
**/
class CDropItem
	: public CActor
{
public:
	CDropItem();
	~CDropItem();

	// �������֐�.
	virtual bool Init() override;
	// �X�V�֐�.
	virtual void Update( const float& deltaTime ) override;
	// �`��֐�.
	virtual void Render() override;
	// Sprite3D�`��֐�.
	virtual void Sprite3DRender() override;

	// �����蔻��֐�.
	virtual void Collision( CActor* pActor ) override;

	// �����蔻��̏�����.
	virtual void InitCollision() override;
	// �����蔻��̍��W��A���a�Ȃǂ̍X�V.
	//	Update�֐��̍Ō�ɌĂ�.
	virtual void UpdateCollision() override;

	// �h���b�v�֐�.
	virtual void Drop( D3DXVECTOR3 DropPos ) final;

	// �v���C���[��ݒ�.
	void SetPlayer( const std::shared_ptr<CPlayer> pPlayer ) { m_pPlayer = pPlayer; }

	// �\�����擾.
	bool GetDisp() const { return m_Disp; }

protected:
	// �쐬�֐�.
	virtual void Create() = 0;

protected:
	std::shared_ptr<CWeapon>	m_pGetWeapom;		// ����ł��镐��.
	CDX9StaticMesh*				m_pStaticMesh;		// �{�b�N�X�̃��f��.
	CSprite*					m_pBillboardSprite;	// ����̖��O.

private:
	std::shared_ptr<CPlayer>	m_pPlayer;			// �v���C���[.
	bool						m_Disp;				// �\�����.

};
#endif	// #ifndef DROPITEM_H.
