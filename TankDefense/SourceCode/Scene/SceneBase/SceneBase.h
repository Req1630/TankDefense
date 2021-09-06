/**
* @file SceneBase.h.
* @brief �V�[�����N���X.
* @author ���c���.
*/
#ifndef SCENE_BASE_H
#define SCENE_BASE_H

#include <Windows.h>
#include <string>
#include <stack>
#include <memory>

#include "..\..\Global.h"
#include "..\..\Object\Fade\Fade.h"

class CSceneManager;

/***********************************************
*	�V�[�����N���X.
*		�e�V�[���͂��̃N���X���p�����Ďg�p����.
**/
class CSceneBase
{
public:
	CSceneBase( CSceneManager* pSceneManager )
		: m_pSceneManager	( pSceneManager )
		, m_IsSceneChange	( false )
	{}

	virtual ~CSceneBase(){};
	
	// �Ǎ��֐�.
	virtual bool Load() = 0;
	// �X�V�֐�.
	virtual void Update() = 0;
	// ���f���`��֐�.
	virtual void ModelRender() = 0;
	// �摜�`��֐�.
	virtual void SpriteRender() = 0;
	// �X�v���C�g3D�`��֐�.
	virtual void Sprite3DRender() = 0;

	// �V�[���̕ύX.
	void ChangeScene();

protected:
	// �V�[���̕ύX�̐ݒ�.
	//	�V�[����ύX����O�ɌĂ�.
	void SetSceneChange();

	// �G�f�B�b�g�V�[���̕ύX�̐ݒ�.
	void SetEditSceneChange();


protected:
	CSceneManager*	m_pSceneManager;	// �V�[�����.
	bool			m_IsSceneChange;	// �V�[����ύX���邩.

private:
	// �R�s�[�E���[�u�R���X�g���N�^, ������Z�q�̍폜.
	CSceneBase( const CSceneBase & )				= delete;
	CSceneBase& operator = ( const CSceneBase & )	= delete;
	CSceneBase( CSceneBase && )						= delete;
	CSceneBase& operator = ( CSceneBase && )		= delete;
};

#endif // #ifndef SCENE_BASE_H.