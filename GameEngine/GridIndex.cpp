#include "GridIndex.h"
#include "sdl_openGL.h"
#include "Settings.h"


GridIndex::GridIndex(int BucketCountX, int BucketCountY)
{
	Index.resize(BucketCountX);

	for (int i = 0; i < BucketCountX; i++)
		Index[i].resize(BucketCountY);

	BucketSize = Vector3(1024, SCREEN_HEIGHT, 0) / Vector3((float)BucketCountX, (float)BucketCountY, 0.0f);

	this->BucketCountX = BucketCountX;
	this->BucketCountY = BucketCountY;
}


GridIndex::~GridIndex(void)
{
}


// Adds a node to the Grid Index. 
void GridIndex::AddNode(Node* node)
{
	AllNodes.push_back(node);
}


// Updates the Grid Index, Nodes, processes their collision
void GridIndex::Update(float dT)
{

	for (int i=0; i<BucketCountX; i++)
	{
		for (int j=0; j<BucketCountY; j++)
		{
			Index[i][j].clear();
		}
	}

	TNode::iterator it = AllNodes.begin();
	while (it != AllNodes.end())
	{
		Node* nd1 = *it;

		nd1->Update(dT);

		float x = nd1->GetPosition().GetX();
		float y = nd1->GetPosition().GetY();

		if (x >= 0.0f && y >= 0.0f && x <= 1024.0f && y <= 768.0f)
		{

			Vector3 ObjectVert[4] = {
				Vector3(nd1->GetPosition().GetX() - nd1->GetWidth()/2, nd1->GetPosition().GetY() - nd1->GetHeight()/2, 1),
				Vector3(nd1->GetPosition().GetX() + nd1->GetWidth()/2, nd1->GetPosition().GetY() - nd1->GetHeight()/2, 1),
				Vector3(nd1->GetPosition().GetX() + nd1->GetWidth()/2, nd1->GetPosition().GetY() + nd1->GetHeight()/2, 1),
				Vector3(nd1->GetPosition().GetX() - nd1->GetWidth()/2, nd1->GetPosition().GetY() + nd1->GetHeight()/2, 1)
			};

			Vector3 indexs[4] = {};
			int idxCount = 0;
			for(int i=0; i<4; i++)
			{
				Vector3 idx = ObjectVert[i] / BucketSize;
				bool found = false;
				for(int j=0; j<4; j++)
				{
					if(j>=idxCount)
						break;

					if( idx.m_X == indexs[j].m_X  && idx.m_Y == indexs[j].m_Y)
					{
						found = true;
						break;
					}
				}
				if(found==false)
				{
					indexs[idxCount] = idx;
					idxCount++;
				}
			}
			
			for(int i=0; i<idxCount; i++)
			{
				int x = (int)indexs[i].m_X;
				int y = (int)indexs[i].m_Y;
				if(x < BucketCountX && x >= 0 && y < BucketCountY && y >= 0)
					Index[x][y].push_back(nd1);
			}
		}
		it++;

	}

	//Check for collision detection
	for (int i=0; i<BucketCountX; i++)
	{
		for (int j=0; j<BucketCountY; j++)
		{
			BucketCollision(i, j, dT);
			//if bucket collision true, then delete node
		}
	}

}


// Delete the current Node
void GridIndex::DeleteNode()
{
}


// Draws grid of the grid Index
void GridIndex::DrawGrid()
{
	TNode::iterator it = AllNodes.begin();
	while (it != AllNodes.end())
	{
		Node* nd1 = *it;

		nd1->Draw();

		it++;
	}




#ifdef _DEBUG
	glDisable(GL_TEXTURE_2D);

	glColor3f(0,1,0);

	glBegin(GL_LINES);
	{
		for(float x = 0; x < 1024; x += BucketSize.GetX())
		{
			glVertex2f(x, 0);
			glVertex2f(x, SCREEN_HEIGHT);
		}

		for(float y = 0; y < SCREEN_HEIGHT; y += BucketSize.GetY())
		{
			glVertex2f(0, y);
			glVertex2f(1024, y);
		}
	}
	glEnd();

	glColor3f(1,1,1);

	glEnable(GL_TEXTURE_2D);
#endif


}


void GridIndex::BucketCollision(int x, int y, float dt)
{
	TNode tn1 = Index[x][y];
	for (int i=0; i < tn1.size(); i++)
	{	
		if (tn1[i] == NULL)
			continue;

		Node *nd1 = tn1[i];
		for (int j=0; j < tn1.size(); j++)
		{
			if (tn1[j] == NULL)
				continue;

			Node *nd2 = tn1[j];

			if (nd1 == nd2)
				continue;

			if (nd1->TestCollide(nd2))
			{

				// Skip the loop if both nodes are walls
				if (nd1->GetFaction() == FT_WALL && nd2->GetFaction() == FT_WALL)
					continue;


				if ((nd2->GetFaction() == FT_MISSILE_GREEN || nd2->GetFaction() == FT_MISSILE_RED) && nd2->m_timer == 0 ||
					(nd2->GetFaction() == FT_MISSILE_GREEN || nd2->GetFaction() == FT_MISSILE_RED) && (nd1->GetFaction() == FT_TANK_GREEN || nd1->GetFaction() == FT_TANK_RED))
				{
					TNode::iterator it = AllNodes.begin();
					while(it != AllNodes.end())
					{
						if(nd2 == *it)
						{
							AllNodes.erase(it);
							tn1[j] = NULL;

							if (nd2->GetFaction() != FT_MISSILE_GREEN && nd2->GetFaction() != FT_MISSILE_RED)
								delete nd2;

							break;
						}
						it++;
					}

				}

			}

		}
	}
}