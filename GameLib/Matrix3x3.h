#ifndef VECTOR_H
#define VECTOR_H



class Vector3;

class Matrix3x3
{
private:
	float m11, m12, m13;
	float m21, m22, m23;
	float m31, m32, m33;

public:
	Matrix3x3(void);
	~Matrix3x3(void);


	//------- Matrix identity -------------
	// [ 1		0		0
	//	 0		1		0 
	//	 0		0		1 ]
	//-------------------------------------
	void Identity();


	//------- Matrix translate -------------
	// [ 1		0		a_fx
	//	 0		1		a_fy 
	//	0		0		1 ]
	//-------------------------------------
	void SetTranslation(float a_fx, float a_fy);


	void GetTranslation(float &a_fx, float &a_fy);

	//------- Matrix scale -------------
	// [ a_fx		0		0
	//	 0			a_fy	0 
	//	 0			0		0 ]
	//-------------------------------------
	void Scale(float a_fx, float a_fy);


	//------- Matrix rotation -------------
	// [ cos @		sin @	0
	//	 -sin @		cos @	0 
	//		0		0		1 ]
	//-------------------------------------
	//void MRotation(float a_fangle);  //fangle in radians

	float GetRotation(int a_walltype);

	//void SetMTranslation(float a_fx, float a_fy);

	void SetRotation(float a_fAngle);


	void MTransformVector();



	//------- Matrix shear -------------
	// [ 1			a_fy		0
	//	 a_fx		1			0 
	//	 0			0			1 ]
	//-------------------------------------
	void Shear(float a_fx, float a_fy);


	//------- Overload operator * for Matrix & Matrix -------------
	// 	[m11, m12, m13		[n11, n12, n13
	//   m21, m22, m23	 *	n21, n22, n23
	//   m31, m32, m33]		n31, n32, n33]
	// 
	//--------------------------------------------------
	Matrix3x3 operator*(const Matrix3x3& a_Minput);

	// Overload operator = for Matrix1 = Matrix2
	const Matrix3x3& operator=(const Matrix3x3& a_Minput);


	// Overload operator * for Matrix & Vector3 
	Vector3 operator*(const Vector3& a_Vinput);

	float GetX() { return m13; }
	float GetY() { return m23; }

};


#endif