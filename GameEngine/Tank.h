//#ifndef TANK_H
//#define TANK_H
#pragma once

#include "Sprite.h"
#include "Node.h"
#include "Turret.h"


class Sprite;
class Turret;


class Tank : public Node
{
public:
	float m_acceleration;
	Vector3 m_acc;
	float m_direction;


	Tank(void);
	Tank(const char* bodyFilename, const char* turretFilename, float a_x, float a_y, FACTION a_eFaction);
	~Tank(void);

	void Update(float dt);

	void Draw();

	void Rotate(float f); 
	void Translate(float a_velX, float a_velY);

	void RotateTurret(float f);

	void Shoot();

	Vector3 GetPosition();

	float GetRadius();

	bool TestCollide(Node* a_node2);

	void Respawn(float a_x, float a_y);

	void Destroy()
	{
		delete this;
	}

	bool IsCircle() { return true; }


	bool IsAABB()	{ return false; }


	float GetWidth() {return m_tank->GetWidth();}
	float GetHeight() {return m_tank->GetHeight();}
	int m_score;
	int m_health;
	string m_szProfileName;

private:
	Sprite* m_tank;
	Turret* m_turret;
	float m_radius;
	float m_missileBounceAngle;
	bool Collided;

	int nextShotWaitTime;


};

//#endif
