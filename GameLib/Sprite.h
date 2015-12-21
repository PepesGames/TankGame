
#pragma once


#include "Vector3.h"

class Matrix3x3;

class Sprite
{
protected:
	Sprite(){};

public:
	Sprite(const char *szFilename);	// constructor
	virtual ~Sprite();				// destructor

	float	GetWidth()	{ return m_width; }
	float	GetHeight()	{ return m_height; }

	unsigned int GetHandle()	{ return m_handle; }

	void LoadTexture(const char *szFilename);

	void Draw(Matrix3x3 a_worldTrans);

	void DrawImage(float a_x, float a_y, float a_w, float a_h);
	void DrawChar(const char a_char, float a_x, float a_y, float a_width, float a_height, float a_scale);
	void DrawString(const char* a_text,  float a_x, float a_y, float a_width, float a_height, float a_scale);

	void FreeTexture();


private:
	bool m_loaded;
	float m_width;
	float m_height;
	unsigned int m_handle;
	Vector3 points[4]; 

};

//#endif