#include "sdl_openGL.h"
#include "sdl.h"
#include "sdl_image.h"


#include "Wall.h"
#include "Sprite.h"


Wall::Wall(void)
{
}

Wall::Wall(const char* szFilename, float a_x, float a_y)
{
	this->m_worldTrans.SetTranslation(a_x, a_y);

	m_wall = new Sprite(szFilename);
	m_radius = 35.0f;
	Collided = false;
}

Wall::~Wall(void)
{

}

//virtual void Update(Node* a_node2, float dt);
void Wall::Update(float dt)
{

}

void Wall::Draw()
{

	m_wall->Draw(m_worldTrans);

}

Vector3 Wall::GetPosition()
{
	Vector3 tmpV;
	tmpV.m_X =	m_worldTrans.GetX();
	tmpV.m_Y =	m_worldTrans.GetY();
	return tmpV;
}

float Wall::GetRadius()
{
	return m_radius;
}

bool Wall::TestCollide(Node* a_node2)
{

	if (this->GetPosition().isCollidingCircles(a_node2->GetPosition(), m_radius, a_node2->GetRadius() ))
		return true;
	else
		return false;

}





