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
#include "..\..\..\Object\GameObject\Actor\Player\CPlayer.h"

CTitle::CTitle( CSceneManager* pSceneManager )
	: CSceneBase	( pSceneManager )
	, m_pSkinMesh	( nullptr )
	, m_pStaticMesh	( nullptr )
	, m_pPlayer		( nullptr )
{
	m_pPlayer = new CPlayer();
}

CTitle::~CTitle()
{
	SAFE_DELETE( m_pPlayer );
}

//============================.
//	読込関数.
//============================.
bool CTitle::Load()
{
	m_pSkinMesh = CMeshResorce::GetSkin("kaito_s");
	m_pStaticMesh = CMeshResorce::GetStatic("stage");
	m_Sprite = CSpriteResource::GetSprite("pokemon");
	m_effect1.SetEffect("uvtest");
	m_effect2.SetEffect("uvtest");
	return true;
}

//============================.
//	更新関数.
//============================.
void CTitle::Update()
{
	m_DeltaTime = GetDeltaTime();
	m_pSkinMesh->SetAnimSpeed( GetDeltaTime<double>() );

	static D3DXVECTOR3 pos = { 0.0f, 0.0f, 0.0f };
	static D3DXVECTOR3 pos2 = { 0.0f, 0.0f, 1.0f };
	static float rot = 0.0f, rot_y = 0.0f;

	if( CKeyInput::IsPress('I') == true ) pos.x += 0.01f;
	if( CKeyInput::IsPress('K') == true ) pos.x -= 0.01f;
	if( CKeyInput::IsPress('J') == true ) pos.y += 0.01f;
	if( CKeyInput::IsPress('L') == true ) pos.y -= 0.01f;
	if( CKeyInput::IsPress('U') == true ) pos.z += 0.01f;
	if( CKeyInput::IsPress('O') == true ) pos.z -= 0.01f;

	if( CKeyInput::IsPress('I') == true ) pos2.x += 0.01f;
	if( CKeyInput::IsPress('K') == true ) pos2.x -= 0.01f;
	if( CKeyInput::IsPress('J') == true ) pos2.y += 0.01f;
	if( CKeyInput::IsPress('L') == true ) pos2.y -= 0.01f;
	if( CKeyInput::IsPress('U') == true ) pos2.z += 0.01f;
	if( CKeyInput::IsPress('O') == true ){
		m_effect2.Stop();
		pos2.z -= 0.01f;
	}
	
	if( CKeyInput::IsPress('H') == true ){
		m_effect2.Play( {  0.0f, 0.0f, 0.0f } );
		rot += 0.01f;
	}
	if( CKeyInput::IsPress('N') == true ){
		rot_y += 0.01f;
		m_effect1.Play( {  0.0f, 10.0f, 0.0f  } );
	}

	static CSphere sphere, s1, s2;
	static CRay ray;
	D3DXVECTOR3 vec;
	D3DXVec3Normalize( &vec, &D3DXVECTOR3( cosf(rot), sinf(rot_y), sinf(rot) ) );
	ray.SetVector( vec );
	ray.SetStartPos( pos );
	ray.SetEndPos( pos2 );
	sphere.SetPosition( { 0.0f, 1.0f, 0.0f } );

	D3DXVECTOR3 p, p2;
	s1.SetRadius( 0.1f );
	s2.SetRadius( 0.1f );

	bool isHit = coll::IsRayToSphere( &ray, &sphere, &p, &p2 );
	s1.SetPosition( p );
	s2.SetPosition( p2 );
	CCollisionRender::PushRay( &ray );
	CCollisionRender::PushSphere( &s1 );
	CCollisionRender::PushSphere( &s2 );
	CCollisionRender::PushSphere( &sphere );

	CDebugText::PushText( "vec X : ", m_effect2.GetTotalInstanceCount() );
	CDebugText::PushText( "vec Y : ", m_effect1.GetTotalInstanceCount() );
	CDebugText::PushText( "vec Z : ", vec.z );
	//if( CKeyInput::IsMomentPress('G') == true ){
	//	SetSceneChange();
	//}
	//if( CKeyInput::IsMomentPress('E') == true ){
	//	SetSceneChange();
	//	m_pSceneManager->ChangeEditScene();
	//}

	m_pPlayer->Update( GetDeltaTime<float>() );
}

//============================.
// モデル描画関数.
//============================.
void CTitle::ModelRender()
{
	m_pStaticMesh->Render();
	m_pSkinMesh->Render();

	CDX9StaticMesh* pStaticMesh = CMeshResorce::GetStatic("normTest");

	pStaticMesh->SetPosition( { -2.0f, 3.0f, -10.0f } );
	pStaticMesh->Render();

	//SSpriteRenderState s = m_Sprite->GetRenderState();
	//s.Tranceform.Scale = { 3.0f, 3.0f, 3.0f };
	//s.Color = { 1.0f, 1.0f, 1.0f, 1.0f };
	//m_Sprite->Render3D( false, &s );

	m_effect2.Render();
	m_effect1.Render();

	m_pPlayer->Render();
}

//============================.
// 画像描画関数.
//============================.
void CTitle::SpriteRender()
{
#if 0
	D3DXVECTOR2 vec = D3DXVECTOR2(1.0f,0.0f);
	static float angle = 0.0f;

	for( int i = 0; i < 4; i++ ){
		// 行列回転.
		float fmat[3][3];
		fmat[0][0] = cosf(angle);
		fmat[0][1] = sinf(angle);
		fmat[0][2] = 0.0f;

		fmat[1][0] = -sinf(angle);
		fmat[1][1] = cosf(angle);
		fmat[1][2] = 0.0f;

		fmat[2][0] = 0.0f;
		fmat[2][1] = 0.0f;
		fmat[2][2] = 1.0f;
		
		D3DXVECTOR2 fvec2;

		// TransformCoord.
		float _x = ( vec.x * fmat[0][0] ) + ( vec.y * fmat[1][0] ) + fmat[2][0];
		float _y = ( vec.x * fmat[0][1] ) + ( vec.y * fmat[1][1] ) + fmat[2][1];
		
		float w = ((vec.x * fmat[0][2] ) + (vec.y * fmat[1][2] ) + fmat[2][2]);
		if(w) w	= 1.0f / w;

		fvec2.x = _x * w;
		fvec2.y = _y * w;

		SSpriteRenderState rs;
		rs.Tranceform.Position = 
			D3DXVECTOR3( 100.0f, 100.0f, 0.0f) + 
			D3DXVECTOR3( fvec2.x, fvec2.y, 0.0f ) * 100.0f;
		rs.Tranceform.Scale = D3DXVECTOR3( 0.1f, 0.1f, 0.0f );
		m_Sprite->Render( &rs );
		angle += D3DX_PI*0.5f;
	}
	angle += 0.01f;
#endif
}

