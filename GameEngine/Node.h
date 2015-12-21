#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <vector>

#include "Vector3.h"
#include "Matrix3x3.h"


using namespace std;

enum FACTION
{
	FT_TANK_GREEN,
	FT_TANK_RED,
	FT_MISSILE_GREEN,
	FT_MISSILE_RED,
	FT_WALL,
};

class Node
{
public:
	Node(void);	
	~Node(void);

	virtual Vector3 GetPosition()=0;

	virtual float GetRadius()=0;

	virtual bool TestCollide(Node* a_node2)=0;

	virtual void Destroy()=0;

	virtual void Update(float dt)=0;

	virtual void Draw()=0;

	virtual float GetWidth()=0;
	virtual float GetHeight()=0;


//protected:
	Matrix3x3 m_worldTrans;
	Matrix3x3 m_localTrans;

	int m_collideCount;
	int m_timer;

	FACTION GetFaction()
	{
		return m_eFaction;
	}

	void SetFaction(FACTION a_eFaction)
	{
		m_eFaction = a_eFaction;
	}

protected:

	FACTION m_eFaction;



};

#endif