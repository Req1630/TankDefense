#include "SceneBase.h"

#include "..\SceneManager\SceneManager.h"

// �V�[���̕ύX�̐ݒ�.
//	�V�[����ύX����O�ɌĂ�.
void CSceneBase::SetSceneChange()
{
	m_IsSceneChange = true; CFade::SetFadeIn();
}

// �V�[���̕ύX.
//	�e�V�[���̍X�V�֐��̍Ōキ�炢�ŌĂ�.
void CSceneBase::ChangeScene()
{
	if( m_IsSceneChange == true && CFade::IsFadeEnd() ) m_pSceneManager->ChangeNextScene();
}

// �G�f�B�b�g�V�[���̕ύX�̐ݒ�.
void CSceneBase::SetEditSceneChange()
{
	SetSceneChange();
	m_pSceneManager->ChangeEditScene();
}