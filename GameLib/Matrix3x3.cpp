#include "stdafx.h"
#include "Matrix3x3.h"
#include "Vector3.h"
#include <math.h>

#define PI 3.14159265

Matrix3x3::Matrix3x3(void)
{
	Identity();
}


Matrix3x3::~Matrix3x3(void)
{
}

void Matrix3x3::Identity()
{
	m11 = m22 = m33 = 1.0f;
	m12 = m13 = m21 = m23 = m31 = m32 = 0.0f;
}

//------- Matrix translate -------------
// [ 1		0		dx
//	 0		1		dy 
//	0		0		1 ]
//	Note: ** with zero rotation
//-------------------------------------
void  Matrix3x3::SetTranslation(float a_fx, float a_fy)
{
	m11 = 1;
	m12 = 0;
	m13 = a_fx;

	m21 = 0;
	m22 = 1;
	m23 = a_fy;

	m31 = 0;
	m32 = 0;
	m33 = 1;
}


//------- Matrix Get Translation -------------
// [ 1		0		dx
//	 0		1		dy 
//	0		0		1 ]
//-------------------------------------
void Matrix3x3::GetTranslation(float &a_fx, float &a_fy)
{
	a_fx = m13;
	a_fy = m23;
}


//------- Matrix scalar -------------
// [ a_fx		0		0
//	 0			a_fy	0 
//	 0			0		0 ]
//-------------------------------------
void Matrix3x3::Scale(float a_fx, float a_fy)
{
	m11 = a_fx;
	m12 = 0;
	m13 = 0;

	m21 = 0;
	m22 = a_fy;
	m23 = 0;

	m31 = 0;
	m32 = 0;
	m33 = 0;

}

float Matrix3x3::GetRotation(int a_walltype)
{
	//if need to detect collision on vertical wall
	if (a_walltype == 1)
		return acos(m11) * 180 / PI;

	//if need to detect collision on horizontal wall
	if (a_walltype == 2)
		return -1 * asin(m21) * 180 / PI;

	return 0.0f;
}


//------- Matrix Set rotation -------------
// [ cos @		sin @	0
//	 -sin @		cos @	0 
//		0		0		1 ]
//-------------------------------------
void Matrix3x3::SetRotation(float a_fangle)  //fangle in radians
{
	m11 = cos (a_fangle * PI / 180);
	m12 = sin(a_fangle * PI / 180);
	m13 = 0;

	m21 = -sin (a_fangle * PI / 180);
	m22 = cos (a_fangle * PI / 180);
	m23 = 0;

	m31 = 0;
	m32 = 0;
	m33 = 1;	
}



//------- Matrix shear -------------
// [ 1			a_fy		0
//	 a_fx		1			0 
//	 0			0			1 ]
//-------------------------------------
void Matrix3x3::Shear(float a_fx, float a_fy)
{
	m11 = 1;
	m12 = a_fy;
	m13 = 0;

	m21 = a_fx;
	m22 = 1;
	m23 = 0;

	m31 = 0;
	m32 = 0;
	m33 = 1;

}




//------- Matrix multiply Matrix -------------
// 	[m11, m12, m13		[n11, n12, n13
//   m21, m22, m23	 *	n21, n22, n23
//   m31, m32, m33]		n31, n32, n33]
//	  
//---------------------------------------------
Matrix3x3 Matrix3x3::operator*(const Matrix3x3& a_Minput)
{ 
	Matrix3x3 t_Mout;

	t_Mout.m11 = (m11 * a_Minput.m11) + (m12 * a_Minput.m21) + (m13 * a_Minput.m31);//(m13 * a_Vinput.m_Z);
	t_Mout.m12 = (m11 * a_Minput.m12) + (m12 * a_Minput.m22) + (m13 * a_Minput.m32);//(m23 * a_Vinput.m_Z);
	t_Mout.m13 = (m11 * a_Minput.m13) + (m12 * a_Minput.m23) + (m13 * a_Minput.m33);//(m33 * a_Vinput.m_Z);

	t_Mout.m21 = (m21 * a_Minput.m11) + (m22 * a_Minput.m21) + (m23 * a_Minput.m31);//(m13 * a_Vinput.m_Z);
	t_Mout.m22 = (m21 * a_Minput.m12) + (m22 * a_Minput.m22) + (m23 * a_Minput.m32);//(m23 * a_Vinput.m_Z);
	t_Mout.m23 = (m21 * a_Minput.m13) + (m22 * a_Minput.m23) + (m23 * a_Minput.m33); //(m33 * a_Vinput.m_Z);

	t_Mout.m31 = (m31 * a_Minput.m11) + (m32 * a_Minput.m21) + (m33 * a_Minput.m31);//(m13 * a_Vinput.m_Z);
	t_Mout.m32 = (m31 * a_Minput.m12) + (m32 * a_Minput.m22) + (m33 * a_Minput.m32);//(m23 * a_Vinput.m_Z);
	t_Mout.m33 = (m31 * a_Minput.m13) + (m32 * a_Minput.m23) + (m33 * a_Minput.m33);//(m33 * a_Vinput.m_Z);

	return t_Mout;
}


//------- Matrix = Matrix -------------
// 	[m11, m12, m13		[n11, n12, n13
//   m21, m22, m23	 *	n21, n22, n23
//   m31, m32, m33]		n31, n32, n33]
//	  
//---------------------------------------------
const Matrix3x3& Matrix3x3::operator=(const Matrix3x3& a_Minput)
{
	m11 = a_Minput.m11;
	m12 = a_Minput.m12;
	m13 = a_Minput.m13;

	m21 = a_Minput.m21;
	m22 = a_Minput.m22;
	m23 = a_Minput.m23;

	m31 = a_Minput.m31;
	m32 = a_Minput.m32;
	m33 = a_Minput.m33;

	return *this;
}


//------- Matrix multiply Vector3-------------
// [ m11		m12		m13			m_X
//	 m21		m22		m23		x	m_Y
//	 m31		m32		m33 ]		m_Z
//	  
//---------------------------------------------
Vector3 Matrix3x3::operator*(const Vector3& a_Vinput)
{ 
	float t_mX;
	float t_mY;
	float t_mZ;

	t_mX = (m11 * a_Vinput.m_X) + (m12 * a_Vinput.m_Y) + m13;//(m13 * a_Vinput.m_Z);
	t_mY = (m21 * a_Vinput.m_X) + (m22 * a_Vinput.m_Y) + m23;//(m23 * a_Vinput.m_Z);
	t_mZ = (m31 * a_Vinput.m_X) + (m32 * a_Vinput.m_Y) + m33;//(m33 * a_Vinput.m_Z);

	Vector3 t_Vout = Vector3(t_mX, t_mY, t_mZ);

	return t_Vout;
}


	
