#include "..\SceneList.h"
#include "..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"
#include "..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\Utility\Input\Input.h"
#include "..\..\..\Object\Fade\Fade.h"
#include "..\..\..\Object\Collider\Sphere\Sphere.h"
#include "..\..\..\Object\Collider\Capsule\Capsule.h"
#include "..\..\..\Object\Collider\CollisionRender\CollisionRender.h"
#include "..\..\..\Object\CameraBase\CameraManager\CameraManager.h"
#include "..\..\..\Object\Collider\CollisionManager\CollisionManager.h"
#include "..\..\..\Object\Collider\Box\Box.h"
#include "..\..\..\Object\Collider\Ray\Ray.h"
#include "..\..\..\Utility\XAudio2\SoundManager.h"
#include "..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\..\Object\GameObject\ActorManager\GameActorManager.h"
#include "..\..\..\Object\Movie\Movie.h"

CTitle::CTitle( CSceneManager* pSceneManager )
	: CSceneBase		( pSceneManager )
	, m_pStaticMesh		( nullptr )
	, m_pGameObjManager	( nullptr )
	, m_pMovie			( nullptr )
{
	m_pGameObjManager = std::make_unique<CGameActorManager>();
	m_pMovie = std::make_unique<CMovie>();
}

CTitle::~CTitle()
{
}

//============================.
//	�Ǎ��֐�.
//============================.
bool CTitle::Load()
{
	m_pStaticMesh = CMeshResorce::GetStatic("stage");
	if( m_pGameObjManager->Init() == false ) return false;

	// �Đ����������[�r�[�ԍ����w�肵�ď�����.
	m_pMovie->Init( EMovieNo::EnemySpawn );

	return true;
}

//============================.
//	�X�V�֐�.
//============================.
void CTitle::Update()
{
	m_DeltaTime = GetDeltaTime();
	m_pGameObjManager->Update();

	// �C�ӂ̉ӏ��ōĐ�.
	if( CKeyInput::IsMomentPress('O') ) m_pMovie->Play();

	m_pMovie->Update();


	if( CKeyInput::IsPress('K') ){
		SetEditSceneChange();
	}
}

//============================.
// ���f���`��֐�.
//============================.
void CTitle::ModelRender()
{
	m_pStaticMesh->Render();
	m_pGameObjManager->Render();

	m_pMovie->ModelRender();
}

//============================.
// �摜�`��֐�.
//============================.
void CTitle::SpriteRender()
{
	m_pMovie->SpriteRender();
}

//============================.
// �X�v���C�g3D�`��֐�.
//============================.
void CTitle::Sprite3DRender()
{
	m_pGameObjManager->Sprite3DRender();
}

