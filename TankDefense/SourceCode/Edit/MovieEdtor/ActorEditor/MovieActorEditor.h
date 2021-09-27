#ifndef MOVIE_ACTOR_EDITOR_H
#define MOVIE_ACTOR_EDITOR_H

#include "..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"
#include "..\..\..\Object\GameObject\Actor\MovieActor\MovieActor.h"
#include "..\..\..\Object\GameObject\Actor\MovieActor\MovieActorManager\MovieActorManager.h"
#include "..\..\..\Object\GameObject\Actor\EnemyBase\Enemy\Enemy.h"

#include <vector>
#include <memory>

class CEnemyBase;
class CEditPlayer;	// ステージエディタ用のプレイヤー.

/*********************************
*	アクターエディタクラス.
**/
class CMovieActorEditor
{
	// アクターの選択状態.
	enum enSelectStateFlag : unsigned char
	{
		ESelectStateFlag_None		= 0,

		ESelectStateFlag_StartPos	= 1 << 0,
		ESelectStateFlag_EndPos		= 1 << 1,
		ESelectStateFlag_Rot		= 1 << 2,

		ESelectStateFlag_AllOn		= 0xff,
		ESelectStateFlag_AllOff		= 0,

	} typedef ESelectStateFlag;

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
		SAnimationController	AC;
		SMovieActor				MovieState;
		EActorNo				ActorNo;
		int						ListIndex;

		stActorEditState()
			: AC			()
			, MovieState	()
			, ActorNo		( EActorNo_Enemy )
			, ListIndex		( 1 )
		{}
	} typedef SActorEditState;
public:
	CMovieActorEditor();
	~CMovieActorEditor();

	// 更新処理.
	void Update();
	// ImGui描画.
	bool ImGuiRender();
	// モデル描画.
	void ModelRender();

	// エディタ用プレイヤーの設定.
	void SetEditPlayer( CEditPlayer* pPlayer );

	// アクターリストの取得.
	SMovieActorStateList GetActorStateList();
	// アクターリストの設定.
	void SetActorStateList( const SMovieActorStateList stateList );

private:
	// 全部のアクターの描画.
	void AllActorRender();
	// 現在選択されているアクターの描画.
	void NowSelectActorRender();
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

	// ImGuiでゲームパッド操作を有効にする.
	void OnImGuiGamepad();
	// ImGuiでゲームパッド操作を無効にする.
	void OffImGuiGamepad();

	// アクターリストの作成.
	template<class T> 
	void CreateMovieActorList( 
		const std::vector<SMovieActor>&		stateList,
		std::vector<std::unique_ptr<T>>&	actorList,
		const EActorNo& actorNo )
	{
		const int stateSize		= stateList.size();
		const int createSize	= stateSize - actorList.size();

		if( createSize > 0 ){
			// 作成数が正の整数なら追加していく.
			for( int i = 0; i < createSize; i++ ){
				actorList.emplace_back( std::make_unique<T>() );
			}
		} else if( createSize < 0 ){
			// 作成数が負の整数なら消していく.
			for( int i = 0; i < abs(createSize); i++ ){
				actorList.pop_back();
			}
		}

		for( int i = 0; i < stateSize; i++ ){
			actorList[i]->Init();
			actorList[i]->SetMovieState( stateList[i] );

			// 各値を設定して追加.
			SActorEditState state;
			state.ActorNo	= actorNo;
			state.ListIndex	= GetListSize(actorNo)-1;
			state.MovieState = stateList[i];
			CDX9SkinMesh* pSkinMesh = GetSkinMesh( state.ActorNo, state.ListIndex );
			state.AC.SetAnimController( pSkinMesh->GetAnimationController() );

			m_ActorEditStateList.emplace_back( state );
		}
	}

private:
	CEditPlayer*								m_pEditPlayer;
	std::vector<std::unique_ptr<CMovieEnemy>>	m_pEnemyList;
	std::vector<SActorEditState>				m_ActorEditStateList;
	float										m_DeltaTime;
	float										m_SelectActorRedColor;
	int											m_NowSelectIndex;
	char										m_NowSelectActorState;
	bool										m_IsPushNodeOpen;
	bool										m_IsImGuiGamepad;
};

#endif	// #ifndef MOVIE_ACTOR_EDITOR_H.
