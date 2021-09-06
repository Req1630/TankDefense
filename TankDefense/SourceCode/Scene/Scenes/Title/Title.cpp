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

CTitle::CTitle( CSceneManager* pSceneManager )
	: CSceneBase		( pSceneManager )
	, m_pStaticMesh		( nullptr )
	, m_pGameObjManager	( nullptr )
{
	m_pGameObjManager = std::make_unique<CGameActorManager>();
}

CTitle::~CTitle()
{
}

//============================.
//	読込関数.
//============================.
bool CTitle::Load()
{
	m_pStaticMesh = CMeshResorce::GetStatic("stage");
	if( m_pGameObjManager->Init() == false ) return false;

	return true;
}

//============================.
//	更新関数.
//============================.
void CTitle::Update()
{
	m_DeltaTime = GetDeltaTime();
	m_pGameObjManager->Update();
	if( CKeyInput::IsPress('K') ){
		SetEditSceneChange();
	}
}

//============================.
// モデル描画関数.
//============================.
void CTitle::ModelRender()
{
	m_pStaticMesh->Render();
	m_pGameObjManager->Render();
}

//============================.
// 画像描画関数.
//============================.
void CTitle::SpriteRender()
{
}

//============================.
// スプライト3D描画関数.
//============================.
void CTitle::Sprite3DRender()
{
}

