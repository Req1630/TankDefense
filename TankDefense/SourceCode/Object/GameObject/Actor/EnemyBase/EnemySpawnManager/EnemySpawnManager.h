#ifndef ENEMY_SPAWN_MANAGER_H
#define ENEMY_SPAWN_MANAGER_H

#include "EnemySpawnStruct.h"
#include "..\..\Stage\StageStruct.h"

/************************************
*	�G�X�|�[�����Ǘ�����N���X.
**/
class CEnemySpawnManager
{
public:
	CEnemySpawnManager();
	~CEnemySpawnManager();

	// ������.
	bool Init();

	// �X�|�[�����X�g�̎擾.
	inline std::vector<SSpawnBoxRange> GetActorList( const EStageNo& no ){ return m_SpawnList[no]; }
	// �X�|�[�����X�g�̐ݒ�.
	inline void SetActorList( const EStageNo& no, const std::vector<SSpawnBoxRange>& list ){ m_SpawnList[no] = list; }

	// �X�|�[�����X�g�̏�������.
	bool WritingActorLst( const EStageNo& no, const std::vector<SSpawnBoxRange>& list );

private:
	// �X�e�[�W���X�g�̓ǂݍ���.
	bool InitStageList();

	// �X�e�[�W�̓ǂݍ���.
	bool StageLoad();

private:
	spawn_list_map								m_SpawnList;	// �X�|�[�����X�g.
	std::unordered_map<EStageNo, std::string>	m_StageList;	// �X�e�[�W���X�g.
	EStageNo									m_NowStageNo;	// ���݂̃X�e�[�W�ԍ�.

};

#endif	// #ifndef ENEMY_SPAWN_MANAGER_H.