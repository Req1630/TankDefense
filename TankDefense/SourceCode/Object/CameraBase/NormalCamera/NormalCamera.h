#ifndef NORMAL_CAMERA_H
#define NORMAL_CAMERA_H

#include "..\CameraBase.h"

/****************************************
* ノーマルカメラ.
**/
class CNormalCamera : public CCameraBase
{
public:
	CNormalCamera();
	virtual ~CNormalCamera();

	// 更新関数.
	virtual void Update( const float& deltaTime ) override;

private:
};

#endif	// #ifndef NORMAL_CAMERA_H.