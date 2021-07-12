#ifndef STAGE_LOADER_H
#define STAGE_LOADER_H

#include "..\StageStruct.h"

#include <vector>
#include <unordered_map>

/****************************
*	�X�e�[�W�ǂݍ��݃N���X.
**/
class CStageLoader
{
public:
	CStageLoader();
	~CStageLoader();

	// ������.
	bool Init();

	// �A�N�^�[���X�g�̎擾.
	inline std::vector<SActorParam> GetActorList( const EStageNo& no ){ return m_ActorList[no]; }
	// �A�N�^�[���X�g�̐ݒ�.
	inline void SetActorList( const EStageNo& no, const std::vector<SActorParam>& list ){ m_ActorList[no] = list; }

	// �A�N�^�[���X�g�̏�������.
	bool WritingActorLst( const EStageNo& no, const std::vector<SActorParam>& list );

private:
	// �X�e�[�W���X�g�̓ǂݍ���.
	bool InitStageList();

	// �X�e�[�W�̓ǂݍ���.
	bool StageLoad();

private:
	std::unordered_map<EStageNo, std::vector<SActorParam>>	m_ActorList;
	std::unordered_map<EStageNo, std::string>				m_StageList;
};

#endif	// #ifndef STAGE_LOADER_H.