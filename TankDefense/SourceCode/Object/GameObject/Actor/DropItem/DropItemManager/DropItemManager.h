/**
* @file		DropItemManager.h.
* @brief	�h���b�v�A�C�e���}�l�[�W���[�N���X.
* @author	��c�F�P.
*/
#ifndef DROPITEMMANAGER_H
#define DROPITEMMANAGER_H

#include "..\..\Actor.h"
#include "..\DropItem.h"

class CPlayer;

namespace DropItem{
	// �h���b�v���X�g.
	enum class enDropItemList : unsigned char {
		MachineGun,
		Missile,
		Beam,

		Max
	} typedef EList;

	// �h���b�v�m��(%).
	struct stProbability
	{
		int MachineGun;
		int Missile;
		int Beam;

		stProbability()
			: stProbability( 33, 33 )
		{}
		stProbability( const int MachProb, const int MissProb, const int BeamProb = 0 )
			: MachineGun	( MachProb )
			, Missile		( MissProb )
			, Beam			( BeamProb )
		{}
	} typedef SProb;
}

/************************************************
*	�h���b�v�A�C�e���}�l�[�W���[.
**/
class CDropItemManager final
	: public CActor
{
private:
	struct stDropItem {
		std::unique_ptr<CDropItem>	pItem;
		DropItem::EList				Tag;

		stDropItem()
			: stDropItem( nullptr, DropItem::EList::MachineGun )
		{}
		stDropItem( std::unique_ptr<CDropItem> pNewItem, const DropItem::EList& NewTag )
			: pItem	( std::move( pNewItem ) )
			, Tag	( NewTag )
		{}

	} typedef SDropItem;

public:
	CDropItemManager();
	CDropItemManager( const std::shared_ptr<CPlayer> pPlayer );
	~CDropItemManager();

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

	// �w�肵������𗎂Ƃ��֐�.
	void Drop( const DropItem::EList& DropItem, const D3DXVECTOR3& DropPos );

	// �����_���ȕ���𗎂Ƃ��֐�.
	void RandomDrop( const D3DXVECTOR3& DropPos, const DropItem::SProb& Prob = DropItem::SProb() );
	
private:
	// �h���b�v�A�C�e���̍쐬�֐�(�t�@�N�g���[���\�b�h).
	std::unique_ptr<CDropItem> CreateDropItem( const DropItem::EList& DropItem );

	// �f�o�b�N�̍X�V�֐�.
	void DebugUpdate();

private:
	std::vector<SDropItem>		m_DropItem;	// �h���b�v�A�C�e��.
	std::shared_ptr<CPlayer>	m_pPlayer;	// �v���C���[.

};
#endif	// #ifndef DROPITEMMANAGER_H.
