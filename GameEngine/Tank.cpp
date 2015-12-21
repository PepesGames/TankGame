#include "sdl_openGL.h"
#include "sdl.h"
#include "sdl_image.h"


#include "Tank.h"


Tank::Tank(void) 
{
}

Tank::Tank(const char* bodyFilename, const char* turretFilename, float a_x, float a_y, FACTION a_eFaction) 
{
	this->m_worldTrans.SetTranslation(a_x, a_y);


	m_tank = new Sprite(bodyFilename);
	m_acceleration = 0.0f;
	m_acc.m_X = 0.0f;
	m_acc.m_Y = 0.0f;
	m_direction = 0.0f;
	SetFaction(a_eFaction);


	m_turret = new Turret(turretFilename, a_eFaction);
	m_turret->m_localTrans.SetTranslation(0,0);

	m_radius = 35.0f;

	m_missileBounceAngle = 0.0f;

	Collided = false;

	nextShotWaitTime = 0;

	m_score = 0;
	m_health = 5;

}

Tank::~Tank(void)
{
}

//void Tank::Update(Node* a_node2, float dt)
void Tank::Update(float dt)
{
	m_turret->m_worldTrans = this->m_worldTrans * m_turret->m_localTrans;

	if (nextShotWaitTime > 0)
		nextShotWaitTime--;


}


void Tank::Draw()
{	

	m_tank->Draw(m_worldTrans);
	m_turret->Draw();

}

void Tank::Rotate(float f)
{
	Matrix3x3 t;
	t.Identity();
	t.SetRotation(f);

	m_worldTrans = m_worldTrans * t;

}


void Tank::Translate(float a_vX, float a_vY)
{
	Matrix3x3 t;
	t.Identity();
	t.SetTranslation(a_vX, a_vY);

	m_worldTrans = m_worldTrans * t;

}


void Tank::RotateTurret(float f)
{
	Matrix3x3 t;
	t.Identity();
	t.SetRotation(f);

	m_turret->m_localTrans = m_turret->m_localTrans * t;

}


void Tank::Shoot()
{

	if (nextShotWaitTime == 0)
	{
		m_turret->Shoot();
		nextShotWaitTime = 20;
	}
}

Vector3 Tank::GetPosition()
{
	Vector3 tmpV;
	tmpV.m_X =	m_worldTrans.GetX();
	tmpV.m_Y =	m_worldTrans.GetY();
	return tmpV;
}

float Tank::GetRadius()
{
	return m_radius;
}

bool Tank::TestCollide(Node* a_node2)
{

	// No Collision for Green Tank & Green Missile
	if (this->GetFaction() == FT_TANK_GREEN && a_node2->GetFaction() == FT_MISSILE_GREEN)
		return false;

	// No Collision for Red Tank & Red Missile
	if (this->GetFaction() == FT_TANK_RED && a_node2->GetFaction() == FT_MISSILE_RED)
		return false;

	// Circle-Circle Collision Detection
	if (this->GetPosition().isCollidingCircles(a_node2->GetPosition(), m_radius, a_node2->GetRadius() ))
	{
		// Check if Tank colliding on the Wall
		if (a_node2->GetFaction() == FT_WALL)
		{
			this->Translate(m_acc.m_X * -0.1f, m_acc.m_Y * -0.1f);	//Translate to opposite direction Y by 0.1f
			return false;
		}

		Collided = true;
		m_score += 1;


		if (this->GetFaction() == FT_TANK_GREEN)
		{
			m_tank->LoadTexture("./Images/tank1_explode.png");
			Respawn(900, 400);
		}
		else
		{
			m_tank->LoadTexture("./Images/tank2_explode.png");
			Respawn(125, 400);
		}
		return true;
	}
	else
		return false;
}


void Tank::Respawn(float a_x, float a_y)
{
	this->m_worldTrans.SetTranslation(a_x, a_y);
	m_turret->m_localTrans.SetRotation(0);

	if (this->GetFaction() == FT_TANK_GREEN)
		m_tank->LoadTexture("./Images/tank1_body.png");
	else
		m_tank->LoadTexture("./Images/tank2_body.png");


}

