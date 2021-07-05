#ifndef ACTOR_EDITOR_H
#define ACTOR_EDITOR_H

#include "..\..\..\Object\GameObject\Actor\MovieActor\MovieActor.h"
#include "..\..\..\Object\GameObject\Actor\EnemyBase\Enemy\Enemy.h"

#include <vector>
#include <memory>

class CEnemyBase;
class CEditPlayer;	// ステージエディタ用のプレイヤー.

/*********************************
*	アクターエディタクラス.
**/
class CActorEditor
{
	// 使用アクター番号.
	enum enActorNo : unsigned char
	{
		EActorNo_None,

		EActorNo_Enemy,
		EActorNo_Enemy1,
		EActorNo_Enemy2,
		EActorNo_Boss,

		EActorNo_Max,

		EActorNo_Begin	= EActorNo_Enemy,
		EActorNo_End	= EActorNo_Max,
	} typedef EActorNo;

	// エディタ用アクター情報.
	struct stActorEditState
	{
		SMovieActor	MovieState;
		EActorNo	ActorNo;
		int			ListIndex;

		stActorEditState()
			: MovieState	()
			, ActorNo		( EActorNo_Enemy )
			, ListIndex		( 1 )
		{}
	} typedef SActorEditState;
public:
	CActorEditor();
	~CActorEditor();

	// 更新処理.
	void Update();
	// ImGui描画.
	bool ImGuiRender();
	// モデル描画.
	void ModelRender();

	// エディタ用プレイヤーの設定.
	inline void SetEditPlayer( CEditPlayer* pPlayer ){ m_pEditPlayer = pPlayer; }

private:
	// アクターノードの表示.
	void ActorNodeDraw();
	// 追加アクターノードの表示.
	void PushActorNodeDraw();
	// アクターパラメータの表示.
	void ActorParameterDraw();
	// 座標の表示.
	void PositionDraw( const char* c, const D3DXVECTOR3& pos );

	// リストの追加.
	void PushList( const EActorNo& no );
	// リストのサイズを取得.
	int GetListSize( const EActorNo& no );
	// アクター番号を文字に変換.
	std::string ActorNoToString( const EActorNo& no );

	// 各メッシュの取得.
	CDX9SkinMesh* GetSkinMesh( const EActorNo& no, const int& index );

private:
	CEditPlayer*										m_pEditPlayer;
	std::vector<std::unique_ptr<CMovieActor<CEnemy>>>	m_pEnemyList;
	std::vector<SActorEditState>						m_ActorEditStateList;
	int													m_NowSelectIndex;
	bool												m_IsPushNodeOpen;
	bool												m_IsImGuiGamepad;
};

#endif	// #ifndef ACTOR_EDITOR_H.
