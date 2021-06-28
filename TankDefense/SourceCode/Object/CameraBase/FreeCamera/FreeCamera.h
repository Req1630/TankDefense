#ifndef FREE_CAMERA_H
#define FREE_CAMERA_H

#include "..\CameraBase.h"
#include <memory>

/****************************************
* �t���[�J����.
**/
class CFreeCamera : public CCameraBase
{
public:
	CFreeCamera();
	virtual ~CFreeCamera();

	// �X�V�֐�.
	virtual void Update( const float& deltaTime ) override;

private:
	// �}�E�X�̍X�V.
	void MouseUpdate();

private:
	D3DXVECTOR2	m_Radian;		// ���W�A��.
	float		m_DeltaTime;	// �f���^�^�C��.
};

#endif	// #ifndef EDIT_CAMERA_H.