#ifndef CAMERA_EDITOR_H
#define CAMERA_EDITOR_H

#include "..\..\..\Object\CameraBase\MovieCamera\MovieCamera.h"

class CNormalCamera;

/********************************
*	演出用カメラエディタ.
**/
class CCameraEditor
{
public:
	CCameraEditor();
	~CCameraEditor();

	// 更新処理.
	void Update();
	// ImGui描画.
	bool ImGuiRender();
	// モデル描画.
	void ModelRender();

	// 演出カメラ情報の取得.
	inline SMovieCamera GetMovieCameraState() { return m_MovieCamera; }

private:
	// カメラ操作.
	void CameraController();
	// カメラ再生.
	void CameraPlaying();
	// 座標の更新.
	void PositionUpdate();
	// 視点座標の更新.
	void LookPositionUpdate();

	// カメラ移動処理の切り替え.
	void ChangeMoveCamera();

private:
	std::unique_ptr<CNormalCamera>	m_pCamera;
	CCameraBase::SCameraState		m_CameraState;
	SMovieCamera					m_MovieCamera;
	D3DXVECTOR2						m_Radian;			// ラジアン.
	float							m_DeltaTime;		// デルタタイム.
	float							m_PosMoveTime;		// 移動時間.
	float							m_LookPosMoveTime;	// 注視位置移動時間.
	bool							m_IsCameraControll;	// カメラ操作できるか.
	bool							m_IsCameraPlaying;	// カメラ再生中か.
};

#endif	// #ifndef CAMERA_EDITOR_H.