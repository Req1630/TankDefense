#ifndef STAGE_EDIT_PLAYER_H
#define STAGE_EDIT_PLAYER_H

#include "..\..\Object\Object.h"

class CRotLookAtCenter;

/*****************************************
* �G�f�B�^�p�̃v���C���[.
**/
class CEditPlayer : public CObject
{
public:
	CEditPlayer();
	virtual ~CEditPlayer();

	// �������֐�.
	bool Init();
	// �X�V�֐�.
	void Update();
	// �`��֐�.
	void Render();

	// �z�u���̎擾.
	inline STranceform GetPutTranceform() { return m_PutTranceform; }

	// �u���邩�ǂ���.
	inline bool IsPut(){ return m_IsPut; }

	// ��]������s�����ǂ���.
	inline void SetIsRotController( const bool& isRotController ){ m_IsRotController = isRotController; }

private:
	// �J��������.
	void CameraController();
	// �ړ�����.
	void MoveController();
	// ��]����.
	void RotationController();
	// �u������.
	void PutController();
	// �z�u���W�̍X�V.
	void PutPositionUpdate();

private:
	std::unique_ptr<CRotLookAtCenter>	m_pLookCamera;
	CDX9SkinMesh*						m_pSkinMesh;
	D3DXVECTOR3							m_MoveVector;
	STranceform							m_PutTranceform;
	float								m_MoveSpeed;
	float								m_CameraMoveSpeed;
	float								m_CameraLength;
	bool								m_IsPut;
	bool								m_IsRotController;
};

#endif	// #ifndef STAGE_EDIT_PLAYER_H.