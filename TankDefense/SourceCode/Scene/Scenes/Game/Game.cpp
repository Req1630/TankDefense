#include "..\SceneList.h"
#include "..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"
#include "..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\Utility\Input\Input.h"
#include "..\..\..\Object\Fade\Fade.h"
#include "..\..\..\Object\CameraBase\CameraManager\CameraManager.h"

CGame::CGame( CSceneManager* pSceneManager )
	: CSceneBase			( pSceneManager )
{
}

CGame::~CGame()
{
}

//============================.
//	�Ǎ��֐�.
//============================.
bool CGame::Load()
{
	// �Q�[���V�[���p�̃J������p�ӂ��Ă��Ȃ�����.
	//	�t���[�J������L��������.
	CCameraManager::SetActiveFreeCamera();
	return true;
}

//============================.
//	�X�V�֐�.
//============================.
void CGame::Update()
{
	if( CKeyInput::IsMomentPress('G') == true ){
		SetSceneChange();
	}
}

//============================.
// ���f���`��֐�.
//============================.
void CGame::ModelRender()
{
}

//============================.
// �摜�`��֐�.
//============================.
void CGame::SpriteRender()
{
}

//============================.
// �X�v���C�g3D�`��֐�.
//============================.
void CGame::Sprite3DRender()
{
}

