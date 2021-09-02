#include "Actor.h"

CActor::CActor()
	: m_pCollisions	( nullptr )
{
}

CActor::~CActor()
{
}

// 目的の座標へ回転.
bool CActor::TargetRotation( const D3DXVECTOR3& targetVec, const float& rotSpeed, const float& ToleranceRadian, const float& MoveSpeed )
{
	// 自身のベクトルを用意.
	const D3DXVECTOR3 myVector = { sinf( m_Tranceform.Rotation.y ), 0.0f, cosf( m_Tranceform.Rotation.y ) };

	// ベクトルの長さを求める.
	const float myLength = sqrtf( myVector.x*myVector.x + myVector.z*myVector.z );
	const float targetLength = sqrtf( targetVec.x*targetVec.x + targetVec.z*targetVec.z );

	// 内積を求める.
	float dot = myVector.x*targetVec.x + myVector.z*targetVec.z;
	dot = acosf( ( myLength * targetLength ) * dot );

	if ( ( -ToleranceRadian < dot && dot < ToleranceRadian ) ||	// 内積が許容範囲なら.
		( std::isfinite( dot ) ) == false ) {					// 内積の値が計算できない値なら.
		return true;	// 回転終了.
	}
	else {
		// 目的のベクトルと、自分のベクトルの外積を求める.
		const float cross = myVector.x*targetVec.z - myVector.z*targetVec.x;
		// 外積が0.0より少なければ 時計回り : 反時計回り に回転する.
		m_Tranceform.Rotation.y += cross < 0.0f ? rotSpeed : -rotSpeed;

		// ベクトルを使用して移動.
		m_Tranceform.Position.x -= myVector.x * MoveSpeed;
		m_Tranceform.Position.z -= myVector.z * MoveSpeed;

		return false;	// 回転中.
	}
}