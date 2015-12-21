#pragma once

#include "Node.h"
#include "Sprite.h"
#include "GameController.h"

class Sprite;
class GameController;

class Turret : public Node
{
public:
	Turret(void);
	Turret(const char* szFilename, FACTION a_eFaction);
	~Turret(void);
	
	void Update(float dt);

	void Draw();

	Vector3 GetPosition();

	float GetRadius() {	return m_radius;}

	void Shoot();

	//Skip this checking because Turret is part of the Tank
	bool TestCollide(Node* a_node2) { return false; }

	void Destroy()
	{
		delete this;
	}

	float GetWidth() {return m_turret->GetWidth();}
	float GetHeight() {return m_turret->GetHeight();}

private:
	Sprite* m_turret;
	float m_radius;


};

