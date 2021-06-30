#include "Math.h"

//-------------------------------------------------------------.
// degree�@���@radian�ϊ�,
//-------------------------------------------------------------.
float Math::ToRadian( const float& deg )
{
	return deg * (M_PI / 180.0f);
}

//-------------------------------------------------------------.
// radian�@���@degree�ϊ�,
//-------------------------------------------------------------.
float Math::ToDegree( const float& red )
{
	return red * (180.0f / M_PI);
}

//-------------------------------------------------------------.
// v�̃x�N�g���ɑ΂��Ē����̃x�N�g����Ԃ�.
//-------------------------------------------------------------.
D3DXVECTOR3 Math::Orthogonal( const D3DXVECTOR3& v )
{
	const float x = fabsf( v.x );
	const float y = fabsf( v.y );
	const float z = fabsf( v.z );

	D3DXVECTOR3 vOther = x < y ? (x < z ? X_AXIS : Z_AXIS) : (y < z ? Y_AXIS : Z_AXIS);

	D3DXVECTOR3 vResult;
	D3DXVec3Cross( &vResult, &v, &vOther );

	return vResult;
}

//-------------------------------------------------------------.
// u�̕�����v�̕����Ɠ����ɂ��邽�߂�D3DXQUATERNION��Ԃ�.
//-------------------------------------------------------------.
D3DXQUATERNION Math::GetRotationBetween( const D3DXVECTOR3& _u, const D3DXVECTOR3& _v )
{
	D3DXQUATERNION q;

	D3DXVECTOR3 u = _u, v = _v;
	// �x�N�g���𐳋K��.
	D3DXVec3Normalize( &u, &u );
	D3DXVec3Normalize( &v, &v );

	/*
	* We have to check for when u == -v, as u + v
	* in this case will be (0, 0, 0), which cannot be normalized.
	*
	* We account for floating point inaccuracies by taking the dot product (-1.0 when u == -v)
	* and checking if it is close enough to -1.0f

	* ʰ��޸�فiu + v�j�𐳋K�����Čv�Z���Ă��邽��
	* �������^�t�̏ꍇ�� (u == -v) -> u + v == (0, 0, 0)
	* (0, 0, 0) �͐��K���ł��Ȃ����߁A������l�����Ȃ��Ƃ����Ȃ��B
	*
	* �^�t�̏ꍇ�́Au�ɑ΂��Ē����̎���180�x��]�ōςނ̂ŁA�v�Z������قǓ���Ȃ�.
	* �������A���������_���ΐ��x���l�����Ȃ��Ƃ����Ȃ��̂ŁADOT���Z�Ő^�t���ǂ����̔��f������.
	*
	* u == -v �̎��A�h�b�g���Z��-1.0�ɂȂ�.
	*/
	const float UoV = D3DXVec3Dot( &u, &v );
	if( UoV <= -1.0f + FLT_EPSILON ){ // FLT_EPSILON�ŕ��������_���ΐ��x���l��.
		// �G�b�W�P�[�X.

		// �����̎���180�x��].
		D3DXVECTOR3 vOrthogonal = Orthogonal( u );
		D3DXVec3Normalize( &vOrthogonal, &vOrthogonal );

		q.x = vOrthogonal.x;
		q.y = vOrthogonal.y;
		q.z = vOrthogonal.z;
		q.w = 0.0f;
	} else {
		// �ʏ��

		D3DXVECTOR3 vHalf;
		D3DXVec3Normalize( &vHalf, &(u + v) );

		D3DXVECTOR3 vCrossUHalf;
		D3DXVec3Cross( &vCrossUHalf, &u, &vHalf );

		q.x = vCrossUHalf.x;
		q.y = vCrossUHalf.y;
		q.z = vCrossUHalf.z;
		q.w = D3DXVec3Dot( &u, &vHalf );
	}

	return q;
}
