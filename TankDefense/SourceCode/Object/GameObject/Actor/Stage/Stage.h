#ifndef STAGE_H
#define STAGE_H

#include "..\Actor.h"
#include "Stage.h"

/************************************
*	�X�e�[�W�N���X.
**/
class CStage : public CActor
{
public:
	CStage();
	virtual ~CStage();

	// �������֐�.
	virtual bool Init() override;
	// �X�V�֐�.
	virtual void Update( const float& deltaTime ) override;
	// �`��֐�.
	virtual void Render() override;
	// �����蔻��֐�.
	virtual void Collision( CActor* pActor ) override;

	// �����蔻��̏�����.
	virtual void InitCollision() override;
	// �����蔻��̍��W��A���a�Ȃǂ̍X�V.
	//	Update�֐��̍Ō�ɌĂ�.
	virtual void UpdateCollision() override;
};

#endif	// #ifndef STAGE_H.