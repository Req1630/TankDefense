/**
* @file CameraBase.h.
* @brief カメラ基底クラス.
* @author 福田玲也.
*/
#ifndef CAMERA_BASE_H
#define CAMERA_BASE_H

#include "..\Object.h"

/****************************************
*	カメラベースクラス.
**/
class CCameraBase : public CObject
{
public:
	// カメラ情報.
	struct stCameraState
	{
		D3DXVECTOR3 Position;		// 座標.
		D3DXVECTOR3 LookPosition;	// 視点座標.

//		float	operator[] ( size_t i ) const	{ return (&*Position)[i]; }
//		float&	operator[] ( size_t i )			{ return (&*Position)[i]; }
	} typedef SCameraState;

	// カメラを揺らすための情報.
	struct stShakeState
	{
		float			Amplitube;	// 振れ幅(値が大きければ大きく揺れる).
		float			Frequency;	// 周波数(一秒間に何回揺らすか).
		float			Time;		// 揺らす時間.
		D3DXVECTOR3		BasePos;	// 基準となる座標.
		float			Attenuation;// 減衰値 0.0f ~ 1.0fの値を入れると減衰する.
	} typedef SShakeState;

public:
	CCameraBase();
	virtual ~CCameraBase();

	// 更新関数.
	virtual void Update( const float& deltaTime ) = 0;

	// 指定秒で、指定座標に移動するカメラ動作.
	void AnySecondsMove( const SCameraState& newState, const SCameraState& oldState, const float& sec );
	// 指定秒で、指定座標に移動するカメラ動作.
	void AnySecondsMovePosition( const D3DXVECTOR3& newPos, const D3DXVECTOR3& oldPos, const float& sec );
	// 指定秒で、指定座標に移動するカメラ動作.
	void AnySecondsMoveLookPosition( const D3DXVECTOR3& newPos, const D3DXVECTOR3& oldPos, const float& sec );

	// 座標を縦方向に揺らす.
	//	amplitube	振れ幅(値が大きければ大きく揺れる).
	//	frequency	周波数(一秒間に何回揺らすか).
	//	time		揺らす時間.
	//	basePos		基準となる座標.
	//	attenuation	減衰値 0.0f ~ 1.0fの値を入れると減衰する.
	void ShakePositoinVertical(
		const float& amplitube,
		const float& frequency,
		const float& time,
		const D3DXVECTOR3& basePos,
		const float& attenuation = 1.0f );
	// 座標を縦方向に揺らす.
	void ShakePositoinVertical( const SShakeState& state );

	// 座標を横方向に揺らす.
	//	amplitube	振れ幅(値が大きければ大きく揺れる).
	//	frequency	周波数(一秒間に何回揺らすか).
	//	time		揺らす時間.
	//	basePos		基準となる座標.
	//	attenuation	減衰値 0.0f ~ 1.0fの値を入れると減衰する.
	void ShakePositoinHorizontal(
		const float& amplitube,
		const float& frequency,
		const float& time,
		const D3DXVECTOR3& basePos,
		const float& attenuation = 1.0f );
	// 座標を横方向に揺らす.
	void ShakePositoinHorizontal( const SShakeState& state );

	// 視点座標を縦方向に揺らす.
	//	amplitube	振れ幅(値が大きければ大きく揺れる).
	//	frequency	周波数(一秒間に何回揺らすか).
	//	time		揺らす時間.
	//	basePos		基準となる座標.
	//	attenuation	減衰値 0.0f ~ 1.0fの値を入れると減衰する.
	void ShakeLookPositoinVertical(
		const float& amplitube,
		const float& frequency,
		const float& time,
		const D3DXVECTOR3& basePos,
		const float& attenuation = 1.0f );
	// 視点座標を縦方向に揺らす.
	void ShakeLookPositoinVertical( const SShakeState& state );

	// 視点座標を横方向に揺らす.
	//	amplitube	振れ幅(値が大きければ大きく揺れる).
	//	frequency	周波数(一秒間に何回揺らすか).
	//	time		揺らす時間.
	//	basePos		基準となる座標.
	//	attenuation	減衰値 0.0f ~ 1.0fの値を入れると減衰する.
	void ShakeLookPositoinHorizontal(
		const float& amplitube,
		const float& frequency,
		const float& time,
		const D3DXVECTOR3& basePos,
		const float& attenuation = 1.0f );
	// 視点座標を横方向に揺らす.
	void ShakeLookPositoinHorizontal( const SShakeState& state );

	// ビュー・プロジェクションの更新.
	void UpdateViewProj();

	// 注視座標の設定.
	inline void SetLookPosition( const D3DXVECTOR3& p ) { m_LookPosition = p; }
	// 注視座標の取得.
	inline D3DXVECTOR3 GetLookPosition() const { return m_LookPosition; }

	// ビュー行列取得関数.
	inline D3DXMATRIX GetViewMatrix() const { return m_ViewMatrix; }
	// プロジェクション行列取得関数.
	inline D3DXMATRIX GetProjMatrix() const { return m_ProjMatrix; }
	// ビュー・プロジェクションの合成行列取得関数.
	inline D3DXMATRIX GetViewProjMatrix() const { return m_ViewMatrix * m_ProjMatrix; }

	// 動作状態か.
	inline bool IsActive() const { return m_IsActive; }

protected:
	D3DXMATRIX	m_ViewMatrix;			// ビュー行列.
	D3DXMATRIX	m_ProjMatrix;			// プロジェクション行列.
	D3DXVECTOR3	m_LookPosition;			// カメラ注視座標.
	D3DXVECTOR3	m_ViewVector;			// ビューベクトル.
	float		m_ViewAngle;			// 視野角.
	float		m_Aspect;				// アスペクト比.
	float		m_MaxClippingDistance;	// 最大クリッピング距離.
	float		m_MinClippingDistance;	// 最小クリッピング距離.
	bool		m_IsActive;				// 動作しているか.
};

#endif	// #ifndef CAMERA_BASE_H.