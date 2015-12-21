#pragma once
#ifndef _MISSILE
#define _MISSILE

#include "Node.h"

class Sprite;

class Missile : public Node
{
public:
	float m_acceleration;


	Missile(void);
	Missile(Matrix3x3 m, const char* szFilename, Vector3 a_velocity, FACTION a_eFaction);
	~Missile(void);

	void Update(float dt);
	void Draw();

	void Rotate(float f);

	void Translate(float a_vX, float a_vY);

	Vector3 GetPosition();

	float GetWidth();
	float GetHeight();
	float GetRadius() {	return m_radius; }

	bool TestCollide(Node* a_node2);


	void Destroy()
	{
		delete this;
	}



private:
	Sprite* m_missile;
	float m_radius;

	Vector3 m_velocity;

	Vector3 prevpos;

};

#endif