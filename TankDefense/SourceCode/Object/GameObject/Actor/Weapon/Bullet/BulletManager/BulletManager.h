#pragma once
#include "..\..\..\Actor.h"
#include "..\..\..\Weapon\ShotStatus.h"

class CBullet;

/************************************************
*	�e�}�l�[�W���[.
**/
class CBulletManager final
	: public CActor
{
public:
	CBulletManager();
	~CBulletManager();

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

	// ����̌����ɔ�Ԓe�̔��ˊ֐�.
	void NormalShot( std::string StaticMeshName, EObjectTag ObjTag, SStatus& Status, D3DXVECTOR3 Pos, D3DXVECTOR3 Rot, D3DXVECTOR3 MoveVec );

	// �ǔ�����e�̔��ˊ֐�.
	void HomingShot( std::string StaticMeshName, EObjectTag ObjTag, SStatus& Status, D3DXVECTOR3 StartPos, D3DXVECTOR3 EndPos, D3DXVECTOR3 Rot );

private:
	// �f�o�b�N�̍X�V�֐�.
	void DebugUpdate();

private:
	std::vector<std::unique_ptr<CBullet>>	m_pBullet;			// �e.

};
