#include "Actor.h"

CActor::CActor()
	: m_pCollisions	( nullptr )
{
}

CActor::~CActor()
{
}

// �ړI�̍��W�։�].
bool CActor::TargetRotation( const D3DXVECTOR3& targetVec, const float& rotSpeed, const float& ToleranceRadian, const float& MoveSpeed )
{
	// ���g�̃x�N�g����p��.
	const D3DXVECTOR3 myVector = { sinf( m_Tranceform.Rotation.y ), 0.0f, cosf( m_Tranceform.Rotation.y ) };

	// �x�N�g���̒��������߂�.
	const float myLength = sqrtf( myVector.x*myVector.x + myVector.z*myVector.z );
	const float targetLength = sqrtf( targetVec.x*targetVec.x + targetVec.z*targetVec.z );

	// ���ς����߂�.
	float dot = myVector.x*targetVec.x + myVector.z*targetVec.z;
	dot = acosf( ( myLength * targetLength ) * dot );

	if ( ( -ToleranceRadian < dot && dot < ToleranceRadian ) ||	// ���ς����e�͈͂Ȃ�.
		( std::isfinite( dot ) ) == false ) {					// ���ς̒l���v�Z�ł��Ȃ��l�Ȃ�.
		return true;	// ��]�I��.
	}
	else {
		// �ړI�̃x�N�g���ƁA�����̃x�N�g���̊O�ς����߂�.
		const float cross = myVector.x*targetVec.z - myVector.z*targetVec.x;
		// �O�ς�0.0��菭�Ȃ���� ���v��� : �����v��� �ɉ�]����.
		m_Tranceform.Rotation.y += cross < 0.0f ? rotSpeed : -rotSpeed;

		// �x�N�g�����g�p���Ĉړ�.
		m_Tranceform.Position.x -= myVector.x * MoveSpeed;
		m_Tranceform.Position.z -= myVector.z * MoveSpeed;

		return false;	// ��]��.
	}
}