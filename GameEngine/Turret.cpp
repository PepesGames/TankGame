#include "Turret.h"


Turret::Turret(void)
{
}


Turret::~Turret(void)
{
}


Turret::Turret(const char* szFilename, FACTION a_eFaction) 
{
	m_turret = new Sprite(szFilename);
	m_radius = 1.0f;
	SetFaction(a_eFaction);
}


void Turret::Update(float dt)
{
}

void Turret::Draw()
{	
	m_turret->Draw(m_worldTrans);
}


Vector3 Turret::GetPosition()
{
	Vector3 tmpV;
	tmpV.m_X =	m_worldTrans.GetX();
	tmpV.m_Y =	m_worldTrans.GetY();
	return tmpV;
}


void Turret::Shoot()
{
	//Missile initial velocity
	Vector3 missileVelocity;
	missileVelocity.m_X = 0.0f;
	missileVelocity.m_Y = -5.0f;
	missileVelocity.m_Z = 0.0f;

	Matrix3x3 m;
	m.Identity();
	m.SetTranslation(0, -35);

	Matrix3x3 missileWorld = this->m_worldTrans * m;

	if (m_eFaction == FT_TANK_GREEN)
		GameController::createMissile(missileWorld, missileVelocity, FT_MISSILE_GREEN);
	else
		GameController::createMissile(missileWorld, missileVelocity, FT_MISSILE_RED);
}
