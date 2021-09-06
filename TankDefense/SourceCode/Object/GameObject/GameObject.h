/**
* @file GameObject.h.
* @brief �Q�[�����̃I�u�W�F�N�g���N���X.
* @author ���c���.
*/
#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "..\Object.h"
#include "GameObjectList.h"

/*************************************
*	�Q�[���I�u�W�F�N�g.
*	�@�Q�[�����I�u�W�F�N�g�AUI�Ȃǂ̌p��������N���X.
**/
class CGameObject : public CObject
{
	// �E�B���h�E�O�̒����l.
	static constexpr float WND_OUT_ADJ_SIZE = 300.0f;
public:
	CGameObject();
	virtual ~CGameObject();

	// �������֐�.
	virtual bool Init() = 0;
	// �X�V�֐�.
	virtual void Update( const float& deltaTime ) = 0;
	// �`��֐�.
	virtual void Render() = 0;
	// Sprite3D�`��֐�.
	virtual void Sprite3DRender(){}

	// �Q�[���I�u�W�F�N�g�^�O�̎擾.
	inline EObjectTag GetObjectTag(){ return m_ObjectTag; }

	// ��ʂ̊O�ɏo�Ă��邩.
	bool IsDisplayOut( const float& adjSize = WND_OUT_ADJ_SIZE );

protected:
	// �Q�[���I�u�W�F�N�g�^�O�̐ݒ�.
	inline void SetObjectTag( const EObjectTag& tag ){ m_ObjectTag = tag; }

protected:
	float		m_DeltaTime;	// �f���^�^�C��(�K�v�Ȃ�A�b�v�f�[�g�֐��̎n�߂Őݒ肷��).
	EObjectTag	m_ObjectTag;	// �I�u�W�F�N�g�^�O.
};

#endif	// #ifndef GAME_OBJECT_H.
