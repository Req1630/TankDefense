#include "Bullet.h"
#include <random>
#include "..\..\..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\..\..\Utility\Input\Input.h"

CBullet::CBullet()
	: m_pStaticMesh		( nullptr )

	, m_Type			( EType::Normal )
	, m_MoveVec			()

	, m_ControlPoints	()
	, m_Distance		()

	, m_DispTimeCnt		()
	, m_Disp			()
{
	Create();
	Init();
}

CBullet::~CBullet()
{
}

// 初期化関数.
bool CBullet::Init()
{
	// 初期化.
	m_Type			= EType::Normal;
	m_MoveVec		= { 0.0f, 0.0f, 0.0f };
	m_DispTimeCnt	= 0.0f;
	m_Disp			= false;

	InitCollision();
	return true;
}

// 更新関数.
void CBullet::Update( const float& deltaTime )
{
	if ( m_Disp == true ){
		// 弾のタイプに応じて移動.
		switch ( m_Type ) {
		case EType::Normal:
			m_Tranceform.Position -= m_MoveVec * m_Status.Speed;
			break;
		case EType::Homing:
			m_Distance += m_Status.Speed / 10.0f;
			m_Tranceform.Position = Evaluate( m_ControlPoints, m_Distance );
			break;
		case EType::Penetrating:
			break;
		default:
			break;
		}

		// 生存時間を減らす.
		m_DispTimeCnt -= deltaTime;

		// 生存時間が0以下になったら弾を削除する.
		if ( m_DispTimeCnt <= 0.0f ) m_Disp = false;
	}
}

// 描画関数.
void CBullet::Render()
{
	if ( m_Disp == true ){
		// モデルの移動.
		m_pStaticMesh->SetTranceform( m_Tranceform );
		// 弾の描画.
		m_pStaticMesh->Render();
	}
}

// 当たり判定関数.
void CBullet::Collision( CActor* pActor )
{
}

// 当たり判定の初期化.
void CBullet::InitCollision()
{
	m_pCollisions = std::make_unique<CCollisions>();
	m_pCollisions->InitCollision( ECollNo::Sphere );
	m_pCollisions->GetCollision<CSphere>()->SetRadius( 1.5f );
}

// 当たり判定の座標や、半径などの更新.
void CBullet::UpdateCollision()
{
	m_pCollisions->GetCollision<CSphere>()->SetPosition( m_Tranceform.Position );
}

// 特定の方向に飛ぶ弾の発射.
void CBullet::NormalShot( std::string StaticMeshName, EObjectTag ObjTag, D3DXVECTOR3 Pos, D3DXVECTOR3 Rot, D3DXVECTOR3 MoveVec )
{
	// スタティックメッシュの取得.
	m_pStaticMesh			= CMeshResorce::GetStatic( StaticMeshName );

	m_ObjectTag				= ObjTag;			// タグを設定.

	m_Tranceform.Position	= Pos;				// 座標設定.
	m_Tranceform.Rotation	= Rot;				// 回転の設定.
	m_MoveVec				= MoveVec;			// ベクトル設定.
	m_Type					= EType::Normal;	// 弾タイプの設定.
	m_DispTimeCnt			= DISP_TIME;		// 生存時間の設定.

	m_Disp					= true;				// 表示.
}

// 追尾する弾の発射.
void CBullet::HomingShot( std::string StaticMeshName, EObjectTag ObjTag, D3DXVECTOR3 StartPos, D3DXVECTOR3 EndPos, D3DXVECTOR3 Rot )
{
	// スタティックメッシュの取得.
	m_pStaticMesh			= CMeshResorce::GetStatic( StaticMeshName );

	m_ObjectTag				= ObjTag;			// タグを設定.

	m_Tranceform.Position	= StartPos;			// 座標設定.
	m_Tranceform.Rotation	= Rot;				// 回転の設定.
	m_Type					= EType::Homing;	// 弾タイプの設定.
	m_DispTimeCnt			= DISP_TIME;		// 生存時間の設定.
	
	// 開始座標から終了座標までのベクトルの作成.
	D3DXVECTOR3 LengthVec = StartPos - EndPos;
	D3DXVECTOR3 Vec;
	D3DXVec3Normalize( &Vec, &LengthVec );
	// 長さを取得.
	float Length	= D3DXVec3Length( &LengthVec );
	// ベクトルを移動させるための移動ベクトルの作成.
	D3DXVECTOR3 MoveVec;
	MoveVec.x		=  Vec.z;
	MoveVec.y		=  0.0f;
	MoveVec.z		= -Vec.x;
	// ランダムの設定.
	std::random_device	rnd;
	std::mt19937		mt( rnd() );
	std::uniform_int_distribution<>			Rand_LR			( 0, 2 );
	std::uniform_real_distribution<float>	Rand_Length		( 1.0f, Length );
	std::uniform_real_distribution<float>	Rand_Distance	( 10.0f, 30.0f );
	// 中間座標の設定.
	D3DXVECTOR3 MiddlePos;
	MiddlePos = Vec * Rand_Length( mt );
	// 中間座標の移動.
	if ( Rand_LR( mt ) )
		MiddlePos += MoveVec * Rand_Distance( mt );
	else
		MiddlePos -= MoveVec * Rand_Distance( mt );

	// ベジェ曲線の設定.
	m_ControlPoints[0]		= StartPos;		// ベジェ曲線の開始座標.
	m_ControlPoints[1]		= MiddlePos;	// ベジェ曲線の中間座標.
	m_ControlPoints[2]		= EndPos;		// ベジェ曲線の終了座標.

	m_Disp = true;							// 表示.
}

// 作成関数.
void CBullet::Create()
{
	// ベジェ曲線の設定.
	m_ControlPoints.resize( 3 );
}

// ベジェ曲線関数.
D3DXVECTOR3 CBullet::Evaluate( std::vector<D3DXVECTOR3> ControlPoints, float t )
{
	m_ControlPoints = ControlPoints;

	D3DXVECTOR3 result = { 0.0f, 0.0f, 0.0f };
	int n = ControlPoints.size();
	for ( int i = 0; i < n; i++ )
	{
		result += m_ControlPoints[i] * Bernstein( n - 1, i, t );
	}

	return result;
}

// バーンスタイン基底関数.
float CBullet::Bernstein( int n, int i, float t )
{
	return Binomial( n, i ) * pow( t, i ) * pow( 1 - t, n - i );
}

// 二項係数を計算する.
float CBullet::Binomial( int n, int k )
{
	return Factorial( n ) / ( Factorial( k ) * Factorial( n - k ) );
}

// 階乗を計算する.
float CBullet::Factorial( int a )
{
	float result = 1.0f;
	for ( int i = 2; i <= a; i++ )
	{
		result *= i;
	}

	return result;
}
