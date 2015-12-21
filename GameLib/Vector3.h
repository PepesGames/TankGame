#pragma once

#ifndef _VECTOR3
#define _VECTOR3

class Vector3
{

public:
	Vector3(void);
	~Vector3(void);

	Vector3(const Vector3& a_Vinput);
	Vector3(float a_fx, float a_fy, float a_fz); 
	
	//Overload Operators Vector = Vector
	Vector3& operator =(const Vector3& a_Vinput);

	//Get Length, or Get Magnitude
	float GetMagnitude(); 

	// Normalize
	void Normalize(); // Nx = x / length; Ny = y / length

	//Dot Product = Ax * Bx + Ay * By
	float DotProduct(Vector3& v2);

	// Calculate Angle Between 2 Vector
	float Calculate2VectorsAngle(Vector3& v2);

	// Get Perpendicular Clockwise
	Vector3 GetPerpendicularClkw();

	// Get Perpendicular AntiClockwise
	Vector3 GetPerpendicularAClkw();

	//Overload Operators Vector = Vector
	Vector3& operator =(const Vector3& v2) const;

	//Overload Operators Vector + Vector
	Vector3 operator +(const Vector3& v2) const;

	//Overload Operators Vector - Vector
	Vector3 operator -(const Vector3& v2) const; 

	//Overload Operators Vector * Scalar
	Vector3 operator *(const float s2) const;

	//Overload Operators Vector / Vector
	Vector3 operator /(const Vector3& v2) const;

	//Overload Operators Vector / Scalar
	Vector3 operator /(const float s2) const; 

	//Overload Operators Vector * Vector
	float operator *(const Vector3& v2) const;

	//Overload Operators Vector += Vector
	Vector3& operator +=(const Vector3& v2);

	//Overload Operators Vector -= Vector
	Vector3& operator -=(const Vector3& v2);

	//Overload Operators Vector *= Scalar
	Vector3& operator *=(const float s2);

	//Overload Operators Vector /= Scalar
	Vector3& operator /=(const float s2);

	//Overload Operators Vector *= Vector
	float operator *=(const Vector3& v2);


	////Overload Operators Vector / Vector  (NO NEED to implement as it may have diff meanings to diff ppl)
	//Vector3 operator /(const Vector3& v2);

	//Interpolation Ex - Lerp function
	static Vector3 Lerp(const Vector3& v1, const Vector3& v2, float dT);

	//Interpolation Ex - Lerp function 2 (same functionality as above but without v1 input)
	Vector3 Lerp2(const Vector3& v2, float dT);

	//Collision detection using Circle-Circle
	bool isCollidingCircles(Vector3 a_c2Centre, float a_c1Radius, float a_c2Radius);

	//Collision detection using AABB-AABB
	bool isCollidingAABBs(Vector3 a_c2Centre, float a_width, float a_height);



	float GetX();
	float GetY();
	float GetZ();

	float m_X;
	float m_Y;
	float m_Z;


};

#endif