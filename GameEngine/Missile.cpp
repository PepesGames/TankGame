#include "sdl_openGL.h"
#include "sdl.h"
#include "sdl_image.h"
#include "Settings.h"

#include "Missile.h"
#include "Sprite.h"

Missile::Missile(void) {}
Missile::~Missile(void) {}

Missile::Missile(Matrix3x3 m, const char* szFilename, Vector3 velocity, FACTION a_eFaction) 
{
	m_worldTrans = m;
	m_missile = new Sprite(szFilename);
	m_velocity = velocity;
	m_acceleration = 3.0f;
	m_radius = 10.0f;
	m_collideCount = 0;
	m_timer = 50;
	SetFaction(a_eFaction);
}

void Missile::Update(float dt)
{	
	if (m_timer > 0)
		m_timer = m_timer - 1;

	if (m_collideCount == 0)
	{
		m_velocity.m_Y -= (m_acceleration * dt);

		this->Translate(m_velocity.GetX(), m_velocity.GetY());
	}
	else
	{

		this->Translate(m_velocity.GetX() * 1.0f, m_velocity.GetY() * 1.0f);

	}

	//Update PrevPos with its current position
	this->prevpos.m_X = this->GetPosition().GetX();
	this->prevpos.m_Y = this->GetPosition().GetY();

}

void Missile::Draw()
{

	m_missile->Draw(m_worldTrans);
}



void Missile::Translate(float a_vX, float a_vY)
{
	Matrix3x3 t;
	t.Identity();
	t.SetTranslation(a_vX, a_vY);

	m_worldTrans = m_worldTrans * t;
}

void Missile::Rotate(float f)
{
	Matrix3x3 t;
	t.Identity();
	t.SetRotation(f);

	m_worldTrans = m_worldTrans * t;
}

Vector3 Missile::GetPosition()
{
	//return Vector3();
	Vector3 tmpV;
	tmpV.m_X =	m_worldTrans.GetX();
	tmpV.m_Y =	m_worldTrans.GetY();
	return tmpV;
}

float Missile::GetWidth() 
{
	return m_missile->GetWidth(); 
}

float Missile::GetHeight()
{ 
	return m_missile->GetHeight(); 
}

bool Missile::TestCollide(Node* a_node2)
{
	if (this->GetPosition().isCollidingCircles(a_node2->GetPosition(), m_radius, a_node2->GetRadius() ))
	{
		// Missile collide on the Wall for 1st time, so it will bounce back
		if (a_node2->GetFaction() == FT_WALL && m_collideCount == 0)
		{

			if (m_worldTrans.GetRotation(1) == 0)
			{
				// Robound 180 degree for top side wall
				this->Rotate(180);
			}
			else if (this->prevpos.m_X < a_node2->GetPosition().GetX() &&
				this->prevpos.m_Y >= a_node2->GetPosition().GetY() - a_node2->GetRadius() &&
				this->prevpos.m_Y <= a_node2->GetPosition().GetY() + a_node2->GetRadius())
			{
				// Working OK for right side wall
				float t_Rotation = m_worldTrans.GetRotation(1);
				this->Rotate(360 - t_Rotation * -2.0f);
			}
			else if (this->prevpos.m_X > a_node2->GetPosition().GetX() &&
				this->prevpos.m_Y >= a_node2->GetPosition().GetY() - a_node2->GetRadius() &&
				this->prevpos.m_Y <= a_node2->GetPosition().GetY() + a_node2->GetRadius())
			{
				// Working OK for left side wall
				float t_Rotation = m_worldTrans.GetRotation(1);
				this->Rotate(360 - t_Rotation * 2.0f);
			}
			else if (this->prevpos.m_Y > a_node2->GetPosition().GetY())
			{
				// Working OK for upper wall
				float t_Rotation = m_worldTrans.GetRotation(2);
				this->Rotate(180 - t_Rotation * 2.0f);
			}
			else
			{
				// Working OK for lower wall
				float t_Rotation = m_worldTrans.GetRotation(2);
				this->Rotate(180 + t_Rotation * 2.0f);
			}

			m_collideCount = m_collideCount + 1;	//Increment count of Collision
			return false;
		}  
		// Return FALSE if: Green Missile collide on the Green Tank  OR  Red Missile collide on the Red Tank
		else if (a_node2->GetFaction() == FT_TANK_GREEN && m_eFaction == FT_MISSILE_GREEN || 
			a_node2->GetFaction() == FT_TANK_RED && m_eFaction == FT_MISSILE_RED)
			return false;
		else
			return true;	//Otherwise, return TRUE for other conditions
	}
	else
		return false;
}