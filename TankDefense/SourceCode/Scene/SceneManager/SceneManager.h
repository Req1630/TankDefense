/**
* @file SceneManager.h.
* @brief �V�[���Ǘ��N���X.
* @author ���c���.
*/
#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include "..\SceneBase\SceneBase.h"
#include <thread>
#include <mutex>

class CSpriteRender;
class CLoadRender;
class CRenderingTexterManager;

// �V�[���̎��.
enum class enSceneNo : unsigned char;
using ESceneNo	= enSceneNo;
using EScene	= enSceneNo;

/**********************************
*	�V�[���Ǘ��N���X.
*/
class CSceneManager
{	
public:
	CSceneManager();
	~CSceneManager();

	// ������.
	void Init( const bool& isResourceLoadEnd );

	// �X�V�֐�.
	void Update( const float& deltaTime, const bool& isResourceLoadEnd );

	// ���̃V�[���ɕύX.
	void ChangeNextScene();

	// �G�f�B�g�V�[���ɕύX.
	void ChangeEditScene();

private:
	// �ǂݍ��ݎ��̕`��.
	void LoadRender( const float& deltaTime, const bool& isLoad );
	// �V�[���̍X�V.
	void SceneUpdate( const float& deltaTime );
	// �X���b�h�̉��.
	void ThreadRelease();
private:
	HWND										m_hWnd;					// �E�B���h�E�n���h��.
	std::shared_ptr<CSceneBase>					m_pScene;				// �V�[���N���X.
	std::unique_ptr<CLoadRender>				m_pLoadRender;			// �ǂݍ��݂̕`��.
	std::unique_ptr<CRenderingTexterManager>	m_pRenderingTexManager;	// �����_�����O�e�N�X�`���N���X.
	std::function<void()>						m_RenderFunction;		// �����_�����O�֐�.
	EScene										m_NowScene;				// ���݂̃V�[��.
	EScene										m_NextScene;			// ���̃V�[��.
	bool										m_IsThreadLoadEnd;		// �X���b�h���[�h���I��������.
	bool										m_IsLoadEnd;			// ���[�h���I��������.
	bool										m_IsOldLoadEnd;			// ���[�h���I��������.
	bool										m_OnceStep;				// ��񂾂�����.
	std::thread	m_Thread;			// �X���b�h.
	std::mutex	m_Mutex;			
	bool		m_isThreadJoined;
};

#endif // #ifndef SCENE_MANAGER_H.