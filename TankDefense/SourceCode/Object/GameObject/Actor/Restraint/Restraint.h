#ifndef RESTRAINT_H
#define RESTRAINT_H

#include "..\Actor.h"
#include "..\..\..\..\Common\Mesh\RayMesh\RayMesh.h"

class CDX9StaticMesh;

class CRestraint : public CActor
{
public:
	CRestraint();
	virtual ~CRestraint();

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

	// �S���̈ʒu�ݒ�.
	void SetRestraintPos( D3DXVECTOR3 pos, D3DXVECTOR3 movevec );

	// �S���t���O�̎擾.
	bool GetIsRestraint() { return m_IsRestraint; }
	void SetIsRestraint( bool IsRestraint ) { m_IsRestraint = IsRestraint; }

protected:
	CDX9StaticMesh*		m_pStaticMesh;		// �X�^�e�B�b�N���b�V��.

	D3DXVECTOR3			m_Vec3;				// �x�N�g��
	int					m_RestraintCnt;		// �S���J�E���g.
	bool				m_IsRestraint;		// �S���������ǂ���.

};
#endif // !RESTRAINT_H