#ifndef MOVIE_ACTOR_H
#define MOVIE_ACTOR_H

#include "..\EnemyBase\EnemyBase.h"
#include "..\EnemyBase\Enemy\Enemy.h"
#include "..\Actor.h"

// ムービー(演出用)アクター情報.
struct stMovieActor
{
	D3DXVECTOR3	StartPosition;	// 開始座標.
	D3DXVECTOR3	EndPosition;	// 終了座標.
	D3DXVECTOR3	StartRotation;	// 開始回転値.
	float		AciveStartTime;	// 動作開始時間(秒).

	stMovieActor()
		: StartPosition		( 0.0f, 0.0f, 0.0f )
		, EndPosition		( 0.0f, 0.0f, 0.0f )
		, StartRotation		( 0.0f, 0.0f, 0.0f )
		, AciveStartTime	( 0.0f )
	{}
	stMovieActor( 
		const D3DXVECTOR3&	startPos,
		const D3DXVECTOR3&	endPos,
		const D3DXVECTOR3&	rot,
		const float&		startTime )
		: StartPosition		( startPos )
		, EndPosition		( endPos )
		, StartRotation		( rot )
		, AciveStartTime	( startTime )
	{}

} typedef SMovieActor;

/********************************************
*	ムービー用アクタークラス.
*	 インスタンス作成時に使用したいアクターを設定する.
**/
template<typename T>
class CMovieActor : public T
{
public:
	CMovieActor();
	virtual ~CMovieActor();

	// 初期化関数.
	virtual bool Init() override;
	// 更新関数.
	virtual void Update( const float& deltaTime ) override;
	// 描画関数.
	virtual void Render() override;

	// ムービー情報の設定.
	inline void SetMovieState( const SMovieActor& state ){ m_MovieState = state; }

	// モデル表示.
	void ModelRender( const bool& isWire = false );

	// メッシュのポンタの取得.
	inline CDX9SkinMesh* GetSkinMesh(){ return T::m_pSkinMesh; }

private:
	// 動作できるか確認する.
	void ConfirmAcive();

private:
	SMovieActor	m_MovieState;	// ムービー情報.
	float		m_NowTime;		// 動作開始時間(秒).
	bool		m_IsAcive;		// 動作状態か.
};

#endif	// #ifndef MOVIE_ACTOR_H.