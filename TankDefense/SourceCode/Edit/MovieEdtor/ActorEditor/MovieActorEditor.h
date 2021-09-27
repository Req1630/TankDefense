#ifndef MOVIE_ACTOR_EDITOR_H
#define MOVIE_ACTOR_EDITOR_H

#include "..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"
#include "..\..\..\Object\GameObject\Actor\MovieActor\MovieActor.h"
#include "..\..\..\Object\GameObject\Actor\MovieActor\MovieActorManager\MovieActorManager.h"
#include "..\..\..\Object\GameObject\Actor\EnemyBase\Enemy\Enemy.h"

#include <vector>
#include <memory>

class CEnemyBase;
class CEditPlayer;	// �X�e�[�W�G�f�B�^�p�̃v���C���[.

/*********************************
*	�A�N�^�[�G�f�B�^�N���X.
**/
class CMovieActorEditor
{
	// �A�N�^�[�̑I�����.
	enum enSelectStateFlag : unsigned char
	{
		ESelectStateFlag_None		= 0,

		ESelectStateFlag_StartPos	= 1 << 0,
		ESelectStateFlag_EndPos		= 1 << 1,
		ESelectStateFlag_Rot		= 1 << 2,

		ESelectStateFlag_AllOn		= 0xff,
		ESelectStateFlag_AllOff		= 0,

	} typedef ESelectStateFlag;

	// �g�p�A�N�^�[�ԍ�.
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

	// �G�f�B�^�p�A�N�^�[���.
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

	// �X�V����.
	void Update();
	// ImGui�`��.
	bool ImGuiRender();
	// ���f���`��.
	void ModelRender();

	// �G�f�B�^�p�v���C���[�̐ݒ�.
	void SetEditPlayer( CEditPlayer* pPlayer );

	// �A�N�^�[���X�g�̎擾.
	SMovieActorStateList GetActorStateList();
	// �A�N�^�[���X�g�̐ݒ�.
	void SetActorStateList( const SMovieActorStateList stateList );

private:
	// �S���̃A�N�^�[�̕`��.
	void AllActorRender();
	// ���ݑI������Ă���A�N�^�[�̕`��.
	void NowSelectActorRender();
	// �A�N�^�[�m�[�h�̕\��.
	void ActorNodeDraw();
	// �ǉ��A�N�^�[�m�[�h�̕\��.
	void PushActorNodeDraw();
	// �A�N�^�[�p�����[�^�̕\��.
	void ActorParameterDraw();
	// ���W�̕\��.
	void PositionDraw( const char* c, const D3DXVECTOR3& pos );

	// ���X�g�̒ǉ�.
	void PushList( const EActorNo& no );
	// ���X�g�̃T�C�Y���擾.
	int GetListSize( const EActorNo& no );
	// �A�N�^�[�ԍ��𕶎��ɕϊ�.
	std::string ActorNoToString( const EActorNo& no );

	// �e���b�V���̎擾.
	CDX9SkinMesh* GetSkinMesh( const EActorNo& no, const int& index );

	// ImGui�ŃQ�[���p�b�h�����L���ɂ���.
	void OnImGuiGamepad();
	// ImGui�ŃQ�[���p�b�h����𖳌��ɂ���.
	void OffImGuiGamepad();

	// �A�N�^�[���X�g�̍쐬.
	template<class T> 
	void CreateMovieActorList( 
		const std::vector<SMovieActor>&		stateList,
		std::vector<std::unique_ptr<T>>&	actorList,
		const EActorNo& actorNo )
	{
		const int stateSize		= stateList.size();
		const int createSize	= stateSize - actorList.size();

		if( createSize > 0 ){
			// �쐬�������̐����Ȃ�ǉ����Ă���.
			for( int i = 0; i < createSize; i++ ){
				actorList.emplace_back( std::make_unique<T>() );
			}
		} else if( createSize < 0 ){
			// �쐬�������̐����Ȃ�����Ă���.
			for( int i = 0; i < abs(createSize); i++ ){
				actorList.pop_back();
			}
		}

		for( int i = 0; i < stateSize; i++ ){
			actorList[i]->Init();
			actorList[i]->SetMovieState( stateList[i] );

			// �e�l��ݒ肵�Ēǉ�.
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
