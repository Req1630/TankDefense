#ifndef ACTOR_EDITOR_H
#define ACTOR_EDITOR_H

#include "..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"
#include "..\..\..\Object\GameObject\Actor\MovieActor\MovieActor.h"
#include "..\..\..\Object\GameObject\Actor\EnemyBase\Enemy\Enemy.h"

#include <vector>
#include <memory>

class CEnemyBase;
class CEditPlayer;	// �X�e�[�W�G�f�B�^�p�̃v���C���[.

/*********************************
*	�A�N�^�[�G�f�B�^�N���X.
**/
class CActorEditor
{
	// �A�N�^�[�̑I�����.
	enum enSelectStateFlag : unsigned char
	{
		ESelectStateFlag_None		= 0,

		ESelectStateFlag_StartPos	= 1 << 0,
		ESelectStateFlag_EndPos		= 1 << 1,
		ESelectStateFlag_Rot		= 1 << 2,

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
	CActorEditor();
	~CActorEditor();

	// �X�V����.
	void Update();
	// ImGui�`��.
	bool ImGuiRender();
	// ���f���`��.
	void ModelRender();

	// �G�f�B�^�p�v���C���[�̐ݒ�.
	inline void SetEditPlayer( CEditPlayer* pPlayer ){ m_pEditPlayer = pPlayer; }

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

private:
	CEditPlayer*										m_pEditPlayer;
	std::vector<std::unique_ptr<CMovieActor<CEnemy>>>	m_pEnemyList;
	std::vector<SActorEditState>						m_ActorEditStateList;
	float												m_DeltaTime;
	float												m_SelectActorRedColor;
	int													m_NowSelectIndex;
	char												m_NowSelectActorState;
	bool												m_IsPushNodeOpen;
	bool												m_IsImGuiGamepad;
};

#endif	// #ifndef ACTOR_EDITOR_H.
