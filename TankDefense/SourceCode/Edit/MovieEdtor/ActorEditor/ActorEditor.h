#ifndef ACTOR_EDITOR_H
#define ACTOR_EDITOR_H

#include "..\..\..\Object\GameObject\Actor\MovieActor\MovieActor.h"
#include "..\..\..\Object\GameObject\Actor\EnemyBase\Enemy\Enemy.h"

#include <vector>
#include <memory>

class CEnemyBase;

/*********************************
*	�A�N�^�[�G�f�B�^�N���X.
**/
class CActorEditor
{
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

	// �X�V����.
	void Update();
	// ImGui�`��.
	bool ImGuiRender();
	// ���f���`��.
	void ModelRender();

private:
	// �P�̃��f���̕`��.
	void SingleModelRender( const EActorNo& no, const int& index, const bool& isWire );
	// �ǉ��A�N�^�[�m�[�h�̕\��.
	void PushActorNodeDraw();

	// ���X�g�̒ǉ�.
	void PushList( const EActorNo& no );
	// ���X�g�̃T�C�Y���擾.
	int GetListSize( const EActorNo& no );
	// �A�N�^�[�ԍ��𕶎��ɕϊ�.
	std::string ActorNoToString( const EActorNo& no );

private:
	std::vector<std::unique_ptr<CMovieActor<CEnemy>>> m_pEnemyList;
	std::vector<SActorEditState>	m_ActorEditStateList;
	int								m_NowSelectIndex;
	bool							m_IsPushNodeOpen;
};

#endif	// #ifndef ACTOR_EDITOR_H.
