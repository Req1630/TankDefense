#ifndef CAMERA_EDITOR_H
#define CAMERA_EDITOR_H

#include "..\..\..\Object\CameraBase\MovieCamera\MovieCamera.h"

class CMovieCamera;

/********************************
*	演出用カメラエディタ.
**/
class CCameraEditor
{
	enum enShakePosFlags : unsigned char
	{
		EShakePosFlag_None		= 0,

		EShakePosFlag_PosVer		= 1 << 0,	// 座標の縦方向.
		EShakePosFlag_PosHor		= 1 << 1,	// 座標の横方向.
		EShakePosFlag_LookPosVer	= 1 << 2,	// 視点座標の縦方向.
		EShakePosFlag_LookPosHor	= 1 << 3,	// 視点ア票の横方向.

		EShakePosFlag_max,
	} typedef EShakePosFlag;
	using shake_pos_flag = unsigned int;
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
	inline SMovieMoveCamera GetMovieCameraState() { return m_MovieMoveCamera; }

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

	// 移動カメラの表示.
	void MoveCameraDraw();
	// 揺れカメラの表示.
	void ShakeCameraDraw();

private:
	std::unique_ptr<CMovieCamera>	m_pCamera;
	CCameraBase::SCameraState		m_CameraState;
	SMovieMoveCamera				m_MovieMoveCamera;
	SMovieShakeCamera				m_MovieShakeCamera;
	D3DXVECTOR2						m_Radian;			// ラジアン.
	shake_pos_flag					m_ShakePosFlag;		// 揺れ座標のフラグ.
	float							m_DeltaTime;		// デルタタイム.
	bool							m_IsCameraControll;	// カメラ操作できるか.
	bool							m_IsCameraPlaying;	// カメラ再生中か.
};

#endif	// #ifndef CAMERA_EDITOR_H.