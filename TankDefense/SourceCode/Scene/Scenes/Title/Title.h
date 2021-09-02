#ifndef TITLE_H
#define TITLE_H

#include "..\..\SceneBase\SceneBase.h"
#include "..\..\..\Common\Effect\EffectManager.h"
#include "..\..\..\Object\Collider\CollisionManager\CollisionManager.h"
#include "..\..\..\Object\CameraBase\RotLookAtCenter\RotLookAtCenter.h"
#include "..\..\..\Common\Mesh\RayMesh\RayMesh.h"

//class CPlayer;
class CGameActorManager;

/**********************************
*	�^�C�g���V�[���N���X.
*/
class CTitle : public CSceneBase
{
public:
	CTitle( CSceneManager* pSceneManager );
	virtual ~CTitle();

	// �Ǎ��֐�.
	virtual bool Load() override;
	// �X�V�֐�.
	virtual void Update() override;
	// ���f���`��֐�.
	virtual void ModelRender() override;
	// �摜�`��֐�.
	virtual void SpriteRender() override;

private:
	float m_DeltaTime;
	CDX9StaticMesh*	m_pStaticMesh;
	std::unique_ptr<CGameActorManager>	m_pGameObjManager;	// �Q�[���I�u�W�F�N�g�Ǘ��N���X.
};

#endif	// #ifndef TITLE_H.