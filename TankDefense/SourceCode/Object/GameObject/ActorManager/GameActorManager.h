#ifndef CHARACTER_MANAGER_H
#define CHARACTER_MANAGER_H

#include "..\..\..\Global.h"
#include "..\GameObjectList.h"

#include <vector>

class CGameObject;		// �Q�[���I�u�W�F�N�g�N���X.
class CPlayer;			// �v���C���[�N���X.
class CEnemy;			// �G���G�N���X.
class CSpecialEnemy_1;	// ����ȓG1�N���X.
class CSpecialEnemy_2;	// ����ȓG2�N���X.

class CGameActorManager
{
public:
	CGameActorManager();
	~CGameActorManager();

	// �������֐�.
	bool Init();
	// �X�V�֐�.
	void Update();
	// �`��֐�.
	void Render();
	// �X�v���C�g�̕`��.
	void SpriteRender();
	// �I�u�W�F�N�g���W���X�g�̎擾.
	inline std::vector<std::pair<EObjectTag, std::pair<D3DXVECTOR3, float>>>	GetObjPositionList() const { return m_ObjPositionList; }

private:
	// ���W���X�g�̐ݒ�.
	void SetPositionList(CGameObject* pObj);

private:
	float								m_DeltaTime;
	std::shared_ptr<CPlayer>			m_pPlayer;			// �v���C���[.
	std::shared_ptr<CEnemy>				m_pEnemy;			// �G���G.
	std::shared_ptr<CSpecialEnemy_1>	m_pSpecialEnemy_1;	// ����ȓG1(��������G).
	std::shared_ptr<CSpecialEnemy_2>	m_pSpecialEnemy_2;	// ����ȓG2(�S������G).

	std::vector<std::pair<EObjectTag, std::pair<D3DXVECTOR3, float>>>	m_ObjPositionList;	// �e�I�u�W�F�N�g�̍��W���X�g.
	int		m_ObjPosListCount;		// �I�u�W�F�N�g�̍��W���X�g�̐�.
	bool	m_IsOllAnimationStop;	// �S���̃A�j���[�V��������~������.
};

#endif	// #ifndef CHARACTER_MANAGER_H.