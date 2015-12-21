#pragma once
#ifndef _WALL
#define _WALL

#include "Sprite.h"
#include "Node.h"

class Sprite;

class Wall :
	public Node
{
public:
	Wall(void);
	Wall(const char* szFilename, float a_x, float a_y);
	~Wall(void);

	void Update(float dt);

	void Draw(); 

	Vector3 GetPosition();

	float GetRadius();

	bool TestCollide(Node* a_node2);

	void Destroy()
	{
		delete this;
	}
	float GetWidth() {return m_wall->GetWidth();}
	float GetHeight() {return m_wall->GetHeight();}

private:
	Sprite* m_wall;
	float m_radius;
	bool Collided;

};

#endif