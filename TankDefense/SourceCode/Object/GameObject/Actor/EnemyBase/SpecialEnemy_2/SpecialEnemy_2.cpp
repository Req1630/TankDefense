#include "SpecialEnemy_2.h"
#include "..\..\..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"
#include "..\..\..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\..\..\..\Utility\Input\Input.h"
#include "..\..\Restraint\Restraint.h"

CSpecialEnemy_2::CSpecialEnemy_2()
	: m_pEnemyBase		()
	, m_pRestraint		( nullptr )
	, m_pSprite			( nullptr )
	, m_ConfuCnt		( 0 )
	, m_IsConfu			( false )

{
	m_pRestraint	= std::make_shared<CRestraint>();
	m_ObjectTag		= EObjectTag::SpecialEnemy_2;
	Init();
}

CSpecialEnemy_2::~CSpecialEnemy_2()
{
}

// 初期化関数.
bool CSpecialEnemy_2::Init()
{
	if ( m_pRestraint->Init() == false ) return false;	// 拘束の初期化.

	m_pSkinMesh = CMeshResorce::GetSkin( "a_s" );
	m_pSkinMesh->SetAnimSpeed( GetDeltaTime<double>() );
	m_pSkinMesh->SetPosition( D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );

	m_pSprite = CSpriteResource::GetSprite( "hukidasi1" );
	
	Spawn( D3DXVECTOR3( -5.0f, -2.5f, -20.0f ) );
	m_IsConfu = false;
	m_IsDelete = false;

	InitCollision();	// 当たり判定の初期化.
	
	return true;
}

// 更新関数.
void CSpecialEnemy_2::Update( const float& deltaTime )
{
	m_DeltaTime = deltaTime;

	CurrentStateUpdate();	// 現在の状態更新.

	// スポーン.
	if ( CKeyInput::IsMomentPress( 'Y' ) == true ) m_IsDelete = false;
	// 死亡.
	if ( CKeyInput::IsMomentPress( 'H' ) == true ) m_IsDelete = true;
	// 実験で拘束攻撃を消す.
	if ( CKeyInput::IsMomentPress( VK_LSHIFT ) == true ) m_pRestraint->SetIsRestraint( false );

	DebugUpdate();			// デバック更新.

	UpdateCollision();	// 当たり判定の更新.
}

// 描画関数.
void CSpecialEnemy_2::Render()
{
	// 画面の外なら終了.
	if ( IsDisplayOut() == true ) return;

	m_pSkinMesh->SetTranceform( m_Tranceform );

	if ( m_IsDelete == false ) m_pSkinMesh->Render();
	m_pRestraint->Render();

	
}

// Sprite3D描画関数.
void CSpecialEnemy_2::Sprite3DRender()
{
	// 混乱画像の3D描画.
	if ( m_IsConfu == true ){
		SSpriteRenderState ConfuSprite = m_pSprite->GetRenderState();
		ConfuSprite.Tranceform.Position = { m_Tranceform.Position.x - 2.5f, m_Tranceform.Position.y + 12.0f, m_Tranceform.Position.z };
		ConfuSprite.Tranceform.Scale = { 50.0f, 50.0f, 50.0f };
		ConfuSprite.Color = { 1.0f, 1.0f, 1.0f, 1.0f };
		m_pSprite->Render3D( false, &ConfuSprite );
		//m_pSprite->Render();
	}
}

// 当たり判定関数.
void CSpecialEnemy_2::Collision( CActor* pActor )
{
	if ( pActor == nullptr ) return;
	if ( m_pCollisions == nullptr ) return;
	if ( m_pCollisions->GetCollision<CSphere>() == nullptr ) return;
	if ( m_pRestraint->GetCollision<CSphere>() == nullptr ) return;

	PlayerCollsion( pActor );
	SpecialEnemy1Collsion( pActor );
	RestraintCollsion( pActor );
}

// 当たり判定の初期化.
void CSpecialEnemy_2::InitCollision()
{
	m_pCollisions = std::make_unique<CCollisions>();
	m_pCollisions->InitCollision( ECollNo::Sphere );
	m_pCollisions->GetCollision<CSphere>()->SetRadius( 1.5f );
}

// 当たり判定の座標や、半径などの更新.
//	Update関数の最後に呼ぶ.
void CSpecialEnemy_2::UpdateCollision()
{
	m_pCollisions->GetCollision<CSphere>()->SetPosition( D3DXVECTOR3( m_Tranceform.Position.x, m_Tranceform.Position.y, m_Tranceform.Position.z ) );
}

// スポーン.
bool CSpecialEnemy_2::Spawn( const D3DXVECTOR3& spawnPos )
{
	// 既にスポーン済みなら終了.
	if ( m_NowState != Enemy::EEnemyState::None ) return true;
	m_Tranceform.Position = spawnPos;			// スポーン座標の設定.
	//m_LifePoint = pPARAMETER->LifeMax;		// 体力の設定.
	m_NowState = Enemy::EEnemyState::Spawn;		// 現在の状態をスポーンに変更.
	return true;
}

// 相手座標の設定.
void CSpecialEnemy_2::SetTargetPos( CActor& actor )
{
	// プレイヤーの座標を取得.
	CEnemyBase::SetTargetPos( actor );
}

// スポーン中.
void CSpecialEnemy_2::Spawning()
{
	CEnemyBase::Spawning();
}

// 移動.
void CSpecialEnemy_2::Move()
{
	// プレイヤーを追跡.
	if ( m_IsConfu == false ) {
		SetMoveVector( m_TargetPosition );								// 移動ベクトルの設定.
		TargetRotation( m_MoveSpeed );									// 回転.
		CEnemyBase::VectorMove( m_MoveSpeed, RESTRAINT_ATTACK_LENGTH );	// 移動.
		Attack();														// 攻撃.
		CEnemyBase::WaitMove();											// 待機.
	}
	// 混乱状態.
	else {
		SetRandMoveVector( m_RandPos );
		if ( m_IsRandMove == false ) {
			Rand_Pos();
		}
		else {
			RandTargetRotation( m_MoveSpeed );
			RandVectorMove( m_MoveSpeed );
			CEnemyBase::WaitMove();		// 待機.
		}

		// 混乱状態から何秒経ったか.
		m_ConfuCnt++;								// 混乱カウント加算.
		if ( m_ConfuCnt < 10.0*FPS ) return;		// 10秒経ったら解除する.
		m_IsConfu = false;							// 混乱状態を解除する.
		m_IsRandMove = false;							// ランダム座標を変える.
		m_NowMoveState = Enemy::EMoveState::Wait;	// 待機状態にする.
		m_ConfuCnt = 0;								// 混乱カウントの初期化.
	}
}

// 怯み.
void CSpecialEnemy_2::Fright()
{
	CEnemyBase::Fright();
}

// 死亡.
void CSpecialEnemy_2::Death()
{
	CEnemyBase::Death();
}

// 攻撃関数.
void CSpecialEnemy_2::Attack()
{
	if ( m_NowMoveState != Enemy::EMoveState::Attack ) return;

	if ( m_pRestraint->GetIsRestraint() == false ) m_pRestraint->SetIsRestraint( true );
	m_pRestraint->Update( m_DeltaTime );
	m_pRestraint->SetRestraintPos( m_Tranceform.Position, m_MoveVec3 );

	// 実験で拘束攻撃を消す.
	if ( CKeyInput::IsMomentPress( VK_LSHIFT ) == true ) m_pRestraint->SetIsRestraint( false );

	// 拘束攻撃でプレイヤーは操作不能になる.
	// 拘束中はスティックのレバガチャをしないと抜けられない.
	// テスト段階は代わりに左シフトを連打で使う.
	// 拘束が解除されたら特殊な敵2は混乱状態になる.

	// 拘束を解除されたら待機状態に遷移する.
	if ( m_pRestraint->GetIsRestraint() == true ) return;
	m_IsConfu = true;	// 混乱状態にする.
	m_NowMoveState = Enemy::EMoveState::Wait;
}

// プレイヤーとの当たり判定.
void CSpecialEnemy_2::PlayerCollsion( CActor* pActor )
{
	// オブジェクトのタグがプレイヤーじゃなければ終了.
	if ( pActor->GetObjectTag() != EObjectTag::Player ) return;

	// 特殊な敵2とプレイヤーの当たり判定の実験.
	if ( coll::IsSphereToSphere( m_pCollisions->GetCollision<CSphere>(), pActor->GetCollision<CSphere>() ) == false ) return;

	float Radius = pActor->GetCollision<CSphere>()->GetRadius();	//半径.

	D3DXVECTOR3 Pos[2]{ m_Tranceform.Position, pActor->GetPosition() };

	D3DXVECTOR3 Distance = Pos[0] - Pos[1];// 特殊な敵2とプレイヤーの距離（vector）

	float inDistance = ( Radius )-( D3DXVec3Length( &Distance ) );// 特殊な敵2の刺さっている長さ

	D3DXVECTOR3 vecPushOut;// 押し出すvector
	D3DXVec3Normalize( &vecPushOut, &Distance );// Ｐ0 - Ｐ1の正規化ﾍﾞｸﾄﾙ 

	// ﾍﾞｸﾄﾙに長さをかけて刺さった分お互いを押し戻す
	Pos[0].x -= vecPushOut.x * inDistance / 8;
	Pos[0].z -= vecPushOut.z * inDistance / 8;
	Pos[1].x -= -vecPushOut.x * inDistance / 8;
	Pos[1].z -= -vecPushOut.z * inDistance / 8;

	//m_Tranceform.Position = Pos[0];
	pActor->SetPosition( Pos[1] );
}

// 特殊な敵1との当たり判定.
void CSpecialEnemy_2::SpecialEnemy1Collsion( CActor * pActor )
{
	// オブジェクトのタグが特殊な敵1じゃなければ終了.
	if ( pActor->GetObjectTag() != EObjectTag::SpecialEnemy_1 ) return;

	// 特殊な敵2と特殊な敵1の当たり判定の実験.
	if ( coll::IsSphereToSphere( m_pCollisions->GetCollision<CSphere>(), pActor->GetCollision<CSphere>() ) == false ) return;

	float Radius = pActor->GetCollision<CSphere>()->GetRadius();	//半径.

	D3DXVECTOR3 Pos[2]{ m_Tranceform.Position, pActor->GetPosition() };

	D3DXVECTOR3 Distance = Pos[0] - Pos[1];// 特殊な敵2と特殊な敵1の距離（vector）

	float inDistance = ( Radius )-( D3DXVec3Length( &Distance ) );// 特殊な敵2の刺さっている長さ

	D3DXVECTOR3 vecPushOut;// 押し出すvector
	D3DXVec3Normalize( &vecPushOut, &Distance );// Ｐ0 - Ｐ1の正規化ﾍﾞｸﾄﾙ 

	// ﾍﾞｸﾄﾙに長さをかけて刺さった分お互いを押し戻す
	Pos[0].x -= vecPushOut.x * inDistance / 8;
	Pos[0].z -= vecPushOut.z * inDistance / 8;
	Pos[1].x -= -vecPushOut.x * inDistance / 8;
	Pos[1].z -= -vecPushOut.z * inDistance / 8;

	//m_Tranceform.Position = Pos[0];
	pActor->SetPosition( Pos[1] );
}

// 拘束攻撃の当たり判定.
void CSpecialEnemy_2::RestraintCollsion( CActor * pActor )
{
	// 拘束解除されたときに通す.
	pActor->SetIsRestraint( [&]( bool& isRestraint )
	{
		isRestraint = false;
	});

	// オブジェクトのタグがプレイヤーじゃなければ終了.
	if ( pActor->GetObjectTag() != EObjectTag::Player ) return;
	if ( m_NowMoveState != Enemy::EMoveState::Attack ) return;	// 攻撃状態じゃなければ終了.
	if ( m_NowState == Enemy::EEnemyState::Death ) return;	// 死亡していたら終了.
	if ( m_NowState == Enemy::EEnemyState::Fright ) return;	// 怯み状態なら終了.

	// 拘束攻撃と雑魚敵の当たり判定.
	if ( coll::IsSphereToSphere( m_pRestraint->GetCollision<CSphere>(), pActor->GetCollision<CSphere>() ) == false ) return;

	// プレイヤーは操作不能状態になる.
	pActor->SetIsRestraint( [&]( bool& isRestraint )
	{
		isRestraint = true;
	});
}

// デバック更新関数.
void CSpecialEnemy_2::DebugUpdate()
{
#ifdef _DEBUG
	CDebugText::PushText("SpecialEnemy_2", "---------------------------------------------");
	CDebugText::PushText("SpecialEnemy_2", "--------------- SpecialEnemy_2 --------------");
	CDebugText::PushText("SpecialEnemy_2", "---------------------------------------------");
	CDebugText::PushText("SpecialEnemy_2", "Pos : ", m_Tranceform.Position.x, ", ", m_Tranceform.Position.y, ", ", m_Tranceform.Position.z);
	CDebugText::PushText("SpecialEnemy_2", "Rot : ", m_Tranceform.Rotation.x, ", ", m_Tranceform.Rotation.y, ", ", m_Tranceform.Rotation.z);
	CDebugText::PushText("SpecialEnemy_2", "'Y' IsDelete = false, 'H' IsDelete = true");
	CDebugText::PushText("SpecialEnemy_2", "IsDelete : ", m_IsDelete == true ? "true" : "false");
	CDebugText::PushText("SpecialEnemy_2", "m_IsConfu : ", m_IsConfu == true ? "true" : "false");
	CDebugText::PushText("SpecialEnemy_2", "m_IsRestraint : ", m_pRestraint->GetIsRestraint() == true ? "true" : "false");
	// こりじょんの位置も出す.
#endif
}
