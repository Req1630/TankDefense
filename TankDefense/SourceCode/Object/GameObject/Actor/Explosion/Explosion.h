#ifndef EXPLOSION_H
#define EXPLOSION_H

#include "..\Actor.h"
#include "..\..\..\..\Common\Mesh\RayMesh\RayMesh.h"

class CDX9StaticMesh;

class CExplosion : public CActor
{
public:
	CExplosion();
	virtual ~CExplosion();

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

	// �����̈ʒu�ݒ�.
	void SetExplosionPos( D3DXVECTOR3 pos );

	// �����t���O�̎擾.
	bool GetIsExplosion() { return m_IsExplosion; }
	void SetIsExplosion( bool IsExplosion ) { m_IsExplosion = IsExplosion; }

protected:
	CDX9StaticMesh*		m_pStaticMesh;		// �X�^�e�B�b�N���b�V��.

	int					m_ExplosionCnt;		// �����J�E���g.
	bool				m_IsExplosion;		// �����������ǂ���.

};
#endif // !EXPLOSION_H