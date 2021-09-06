#include "BulletManager.h"
#include "..\..\..\..\..\..\Utility\Input\KeyInput\KeyInput.h"
#include "..\Bullet.h"

CBulletManager::CBulletManager()
	: m_pBullet		()
{
	Init();
}

CBulletManager::~CBulletManager()
{
}

// 初期化関数.
bool CBulletManager::Init()
{
	InitCollision();
	return true;
}

// 更新関数.
void CBulletManager::Update( const float & deltaTime )
{
	// 弾の更新.
	for ( auto &b : m_pBullet ) b->Update( deltaTime );

	// 弾の削除.
	const int EndIndex = m_pBullet.size() - 1;
	if ( EndIndex >= 0 ) {	// 要素が0個(EndIndex = -1)の場合ではない時.
		if ( m_pBullet[EndIndex]->GetDisp() == false )
			m_pBullet.pop_back();
	}

	// デバックの更新.
	DebugUpdate();

	UpdateCollision();
}

// 描画関数.
void CBulletManager::Render()
{
	// 弾の描画.
	for ( auto &b : m_pBullet ) b->Render();
}

// 当たり判定関数.
void CBulletManager::Collision( CActor* pActor )
{
	// ドロップアイテムの当たり判定.
	for ( auto &b : m_pBullet ) b->Collision( pActor );
}

// 当たり判定の初期化.
void CBulletManager::InitCollision()
{
}

// 当たり判定の座標や、半径などの更新.
void CBulletManager::UpdateCollision()
{
}

// 特定の向きに飛ぶ弾の発射.
void CBulletManager::NormalShot(
	std::string StaticMeshName,
	EObjectTag	ObjTag,	SStatus&	Status,
	D3DXVECTOR3	Pos,	D3DXVECTOR3 Rot, D3DXVECTOR3 MoveVec )
{
	// 使用していないのがあれば使用する.
	for ( auto &b : m_pBullet ){
		if ( b->GetDisp() == false ){
			// ステータスの設定.
			b->SetStatus( Status );

			// 弾の発射.
			b->NormalShot( StaticMeshName, ObjTag, Pos, Rot, MoveVec );
			return;
		}
	}

	// 新しく作成.
	const int NewIndex = m_pBullet.size();
	m_pBullet.push_back( std::unique_ptr<CBullet>() );
	m_pBullet[NewIndex] = std::make_unique<CBullet>();

	// 弾を撃つ.
	m_pBullet[NewIndex]->SetStatus( Status );
	m_pBullet[NewIndex]->NormalShot( StaticMeshName, ObjTag, Pos, Rot, MoveVec );
}

// 追尾する弾の発射.
void CBulletManager::HomingShot(
	std::string StaticMeshName,
	EObjectTag	ObjTag,		SStatus&	Status,
	D3DXVECTOR3	StartPos,	D3DXVECTOR3	EndPos,
	D3DXVECTOR3 Rot )
{
	// 使用していないのがあれば使用する.
	for ( auto &b : m_pBullet ){
		if ( b->GetDisp() == false ){
			// ステータスの設定.
			b->SetStatus( Status );

			// 弾の発射.
			b->HomingShot( StaticMeshName, ObjTag, StartPos, EndPos, Rot );
			return;
		}
	}

	// 新しく作成.
	const int NewIndex = m_pBullet.size();
	m_pBullet.push_back( std::unique_ptr<CBullet>() );
	m_pBullet[NewIndex] = std::make_unique<CBullet>();

	// 弾を撃つ.
	m_pBullet[NewIndex]->SetStatus( Status );
	m_pBullet[NewIndex]->HomingShot( StaticMeshName, ObjTag, StartPos, EndPos, Rot );
}

// デバックの更新.
void CBulletManager::DebugUpdate()
{
	for ( auto &b : m_pBullet ){
		const size_t		Index	= &b - &m_pBullet[0];
		const D3DXVECTOR3	BPos	= b->GetPosition();
		CDebugText::PushTextF( "Bullet", "Bullet[%3d].Pos  : %f, %f, %f", Index, BPos.x, BPos.y, BPos.z );
		CDebugText::PushTextF( "Bullet", "Bullet[%3d].Disp : %s", Index, b->GetDisp() == true ? "true" : "false" );
	}
}
