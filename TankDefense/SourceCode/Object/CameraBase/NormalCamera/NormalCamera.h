#ifndef NORMAL_CAMERA_H
#define NORMAL_CAMERA_H

#include "..\CameraBase.h"

/****************************************
* �m�[�}���J����.
**/
class CNormalCamera : public CCameraBase
{
public:
	CNormalCamera();
	virtual ~CNormalCamera();

	// �X�V�֐�.
	virtual void Update( const float& deltaTime ) override;

private:
};

#endif	// #ifndef NORMAL_CAMERA_H.