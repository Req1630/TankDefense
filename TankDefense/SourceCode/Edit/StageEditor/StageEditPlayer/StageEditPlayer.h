#ifndef STAGE_EDIT_PLAYER_H
#define STAGE_EDIT_PLAYER_H

#include "..\..\..\Object\Object.h"

class CRotLookAtCenter;

/*****************************************
*	�X�e�[�W�G�f�B�^�p�̃v���C���[.
**/
class CStageEditPlayer : public CObject
{
public:
	CStageEditPlayer();
	virtual ~CStageEditPlayer();

	// �������֐�.
	bool Init();
	// �X�V�֐�.
	void Update();
	// �`��֐�.
	void Render();

	// �z�u���̎擾.
	STranceform GetPutTranceform() { return m_PutTranceform; }

	// �u���邩�ǂ���.
	bool IsPut(){ return m_IsPut; }

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
};

#endif	// #ifndef STAGE_EDIT_PLAYER_H.