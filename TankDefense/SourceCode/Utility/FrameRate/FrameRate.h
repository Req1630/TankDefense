/**
* @file FrameRate.h.
* @brief ゲームループのフレームレートを計算するクラス.
* @author 福田玲也.
*/
#ifndef FRAME_RATE_H
#define FRAME_RATE_H

// 警告についてのｺｰﾄﾞ分析を無効にする.4005:再定義.
#pragma warning(disable:4005)

#include <Windows.h>

#include <D3DX10.h>	//「D3DX～」の定義使用時に必要.

// ライブラリ読み込み.
#pragma comment( lib, "winmm.lib" )
#pragma comment( lib, "d3dx10.lib" )	//「D3DX～」の定義使用時に必要.

/********************************************
*	フレームレートの計測クラス.
**/
class CFrameRate
{
public:
	CFrameRate();
	CFrameRate( const double& fps = 60.0 );
	~CFrameRate();

	// 待機関数.
	//	true の場合そのフレームを終了する.
	bool Wait();

	// FPSの取得.
	inline double GetFPS() const { return m_FPS; }
	// デルタタイムの取得.
	static inline double		GetDeltaTime(){ return m_FrameTime; }
	// 経過時間の取得.
	//	ステージロードからの経過時間(t/20,t,t*2,t*3).
	static inline D3DXVECTOR4	GetElapsedTime(){ return m_Time; }
	// 経過時間の初期化.
	//	シーンのロード終了時などに呼ぶ.
	static inline void ClearElapsedTime(){ m_Time = { 0.0f, 0.0f, 0.0f, 0.0f }; }

private:
	const double	FRAME_RATE;		// フレームレート.
	const double	MIN_FRAME_TIME;	// 最小フレームタイム.

private:
	static double		m_FrameTime;	// フレームタイム(デルタタイム).
	static D3DXVECTOR4	m_Time;			// ステージロードからの経過時間(t/20,t,t*2,t*3).
	double				m_FPS;			// 現在のFPS.
	LARGE_INTEGER		m_StartTime;	// 開始時間.
	LARGE_INTEGER		m_NowTime;		// 現在の時間.
	LARGE_INTEGER		m_FreqTime;		// 起動した時間.
};

// デルタタイムの取得.
template<class T = float>
static inline T GetDeltaTime()
{
	return static_cast<T>(CFrameRate::GetDeltaTime());
}

// 経過時間の取得.
//	ステージロードからの経過時間(t/20,t,t*2,t*3).
template<class T = D3DXVECTOR4>
static inline T GetElapsedTime()
{
	return static_cast<T>(CFrameRate::GetElapsedTime());
}

#endif	// #ifndef FRAME_RATE_H.