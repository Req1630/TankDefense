/**
* @file		Bullet.h.
* @brief	�e�N���X.
* @author	��c�F�P.
*/
#ifndef BULLET_H
#define BULLET_H

#include "..\Actor.h"
#include "..\Weapon\Weapon.h"

class CDX9StaticMesh;

namespace{
	const float DISP_TIME = 10.0f;	// �e�̐�������.
}

/************************************************
*	�e�N���X.
**/
class CBullet
	: public CActor
{
public:
	// �e�̎��.
	enum enType
	{
		Normal,			// ����̕����ɔ�Ԓe.
		Homing,			// �ǔ�����e.
		Penetrating,	// �ђʂ���e.
	} typedef EType;

public:
	CBullet();
	~CBullet();

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

	// ����̌����ɔ�Ԓe�̔���.
	void NormalShot( D3DXVECTOR3 Pos, D3DXVECTOR3 MoveVec );

	// �ǔ�����e�̔���.
	void HomingShot( D3DXVECTOR3 StartPos, D3DXVECTOR3 EndPos );

	// �\�������擾.
	bool GetDisp() const { return m_Disp; }

	// �X�e�[�^�X��ݒ�.
	void SetStatus( CWeapon::SStatus status ) { m_Status = status; }

private:
	// �x�W�F�Ȑ��֐�.
	D3DXVECTOR3	Evaluate( std::vector<D3DXVECTOR3> ControlPoints, float t );

	// �o�[���X�^�C�����֐�.
	float Bernstein( int n, int i, float t );

	// �񍀌W�����v�Z����.
	float Binomial( int n, int k );

	// �K����v�Z����.
	float Factorial( int a );

protected:
	CDX9StaticMesh*				m_pStaticMesh;		// �e�̃��f��.

	CWeapon::SStatus			m_Status;			// �X�e�[�^�X.
	EType						m_Type;				// �e�̃^�C�v.
	D3DXVECTOR3					m_MoveVec;			// �ړ��x�N�g��.

	std::vector<D3DXVECTOR3>	m_ControlPoints;	// �x�W�F�Ȑ��̍��W.
	float						m_Distance;			// �x�W�F�Ȑ��Ŏg�p���鋗��(t).

	float						m_DispTimeCnt;		// �\�����ԃJ�E���g.
	bool						m_Disp;				// �\�����.

};

#endif // #ifndef BULLET_H.