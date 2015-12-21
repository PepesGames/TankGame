#include "stdafx.h"
#include <math.h>
#include "Vector3.h"

#define PI 3.14159265

Vector3::Vector3(void)
{
}


Vector3::~Vector3(void)
{
}

Vector3::Vector3(const Vector3& a_Vinput)
{
	m_X = a_Vinput.m_X;
	m_Y = a_Vinput.m_Y;
	m_Z = 0;
}

Vector3::Vector3(float a_fx, float a_fy, float a_fz)
{
	m_X = a_fx;
	m_Y = a_fy;
	m_Z = 0;
}


//Overload Operators Vector = Vector
Vector3& Vector3::operator =(const Vector3& a_Vinput)
{
	m_X = a_Vinput.m_X;
	m_Y = a_Vinput.m_Y;
	//m_Z = a_Vinput.m_Z;
	m_Z = 0;
	return *this;
}


//Get Length, or Get Magnitude
float Vector3::GetMagnitude()
{
	return sqrt(m_X*m_X + m_Y*m_Y + m_Z*m_Z);
}

// Normalize
void Vector3::Normalize() // Nx = x / length; Ny = y / length
{
	float tmpX;
	float tmpY;
	float tmpZ;

	tmpX = m_X / GetMagnitude();
	tmpY = m_Y / GetMagnitude();
	tmpZ = m_Z / GetMagnitude();

	m_X = tmpX;
	m_Y = tmpY;
	m_Z = 0;
}

//Dot Product = Ax * Bx + Ay * By
float Vector3::DotProduct(Vector3& v2)
{
	return m_X * v2.m_X + m_Y * v2.m_Y + m_Z * v2.m_Z;
}

// Calculate Angle Between 2 Vector
float Vector3::Calculate2VectorsAngle(Vector3& v2)
{
	Vector3 tmp_v1(m_X, m_Y, m_Z);
	tmp_v1.Normalize();

	Vector3 tmp_v2(v2.m_X, v2.m_Y, v2.m_Z);
	tmp_v2.Normalize();

	float tmpDP = tmp_v1.DotProduct(tmp_v2);

	return (float)(acos (tmpDP) * 180.0 / PI );
}

// Get Perpendicular Clockwise
Vector3 Vector3::GetPerpendicularClkw()
{
	Vector3 tmp_v1(m_Y, -1.0f * m_X, 0);
	return tmp_v1;
}

// Get Perpendicular AntiClockwise
Vector3 Vector3::GetPerpendicularAClkw()
{
	Vector3 tmp_v1(-1.0f * m_Y, m_X, 0);
	return tmp_v1;
}



//Overload Operators Vector + Vector
Vector3 Vector3::operator +(const Vector3& v2) const
{
	Vector3 tmpV; 
	tmpV.m_X = m_X + v2.m_X;
	tmpV.m_Y = m_Y + v2.m_Y;
	tmpV.m_Z = m_Z + v2.m_Z;

	return tmpV;
}

//Overload Operators Vector - Vector
Vector3 Vector3::operator -(const Vector3& v2) const
{
	Vector3 tmpV;
	tmpV.m_X = m_X - v2.m_X;
	tmpV.m_Y = m_Y - v2.m_Y;
	tmpV.m_Z = m_Z - v2.m_Z;
	return tmpV;
}

//Overload Operators Vector * Scalar
Vector3 Vector3::operator *(const float s2) const
{
	Vector3 tmpV;
	tmpV.m_X = m_X * s2;
	tmpV.m_Y = m_Y * s2;
	tmpV.m_Z = m_Z * s2;

	return tmpV;
}


//Overload Operators Vector / Vector
Vector3 Vector3::operator /(const Vector3& v2) const
{
	Vector3 tmpV;
	tmpV.m_X = m_X / v2.m_X;
	tmpV.m_Y = m_Y / v2.m_Y;
	tmpV.m_Z = m_Z / v2.m_Z;

	return tmpV;
}


//Overload Operators Vector / Scalar
Vector3 Vector3::operator /(const float s2) const
{
	Vector3 tmpV;
	tmpV.m_X = m_X / s2;
	tmpV.m_Y = m_Y / s2;
	tmpV.m_Z = m_Z / s2;

	return tmpV;
}

//Overload Operators Vector * Vector  (i.e. Dot Product)
float Vector3::operator *(const Vector3& v2) const
{
	return m_X * v2.m_X + m_Y * v2.m_Y + m_Z * v2.m_Z; 
}

//Overload Operators Vector += Vector
Vector3& Vector3::operator +=(const Vector3& v2)
{
	m_X += v2.m_X;
	m_Y += v2.m_Y;
	m_Z += v2.m_Z;

	return *this;
}

//Overload Operators Vector -= Vector
Vector3& Vector3::operator -=(const Vector3& v2)
{
	m_X -= v2.m_X;
	m_Y -= v2.m_Y;
	m_Z -= v2.m_Z;

	return *this;
}

//Overload Operators Vector *= Scalar
Vector3& Vector3::operator *=(const float s2)
{
	m_X *= s2;
	m_Y *= s2;
	m_Z *= s2;

	return *this;
}

//Overload Operators Vector /= Scalar
Vector3& Vector3::operator /=(const float s2)
{
	m_X /= s2;
	m_Y /= s2;
	m_Z /= s2;

	return *this;
}


//Interpolation Ex - Lerp function
Vector3 Vector3::Lerp(const Vector3& v1, const Vector3& v2, float dT)
{
	Vector3 tmpV;
	tmpV.m_X = v2.m_X + dT * (v1.m_X - v2.m_X);
	tmpV.m_Y = v2.m_Y + dT * (v1.m_Y - v2.m_Y);	
	tmpV.m_Z = v2.m_Z + dT * (v1.m_Z - v2.m_Z);	

	return tmpV;
}


//Interpolation Ex - Lerp function 2 (same functionality as above but without v1 input)
Vector3 Vector3::Lerp2(const Vector3& v2, float dT)
{
	Vector3 tmpV;
	tmpV.m_X = m_X + dT * (v2.m_X - m_X);
	tmpV.m_Y = m_Y + dT * (v2.m_Y - m_Y);	
	tmpV.m_Z = m_Z + dT * (v2.m_Z - m_Z);	

	return tmpV;
}


//Collision detection using Circle-Circle
bool Vector3::isCollidingCircles(Vector3 a_c2Centre, float a_c1Radius, float a_c2Radius)
{
	Vector3 c1Centre;
	c1Centre.m_X = m_X;
	c1Centre.m_Y = m_Y;
	c1Centre.m_Z = m_Z;

	float distance = (c1Centre - a_c2Centre).GetMagnitude();
	if (distance < a_c1Radius + a_c2Radius)
		return true;
	else
		return false;
}


float Vector3::GetX()
{
	return m_X;
}

float Vector3::GetY()
{
	return m_Y;
}

float Vector3::GetZ()
{
	return m_Z;
}