#pragma once

#include "Node.h"
#include "Vector3.h"


class GridIndex
{
public:
	GridIndex(int BucketCountX, int BucketCountY);
	~GridIndex(void);

	// Adds a node to the Grid Index. 
	void AddNode(Node* node);

	// Updates the Grid Index, Nodes, processes their collision
	void Update(float dt);

	// Delete the current Node
	void DeleteNode();

	// Draws grid of the grid Index
	void DrawGrid();

private:
	//Below are typedefs that you need to fill out for
	//to use your Container

	//Create a 2D dynamic array containing multi-nodes
	typedef vector<Node*> TNode;
	typedef vector<TNode> TRow;
	typedef vector<TRow> TIndex;

	//This will be the Spatial Index 
	TIndex Index;

	//You will also probably want to keep a completely seperate list of
	//All the nodes separate of the grid index
	TNode AllNodes;

	// Processes the collision within a bucket
	// \param	x	The x coordinate.  
	// \param	y	The y coordinate. 
	// \param	dt	The dt.
	void BucketCollision(int x, int y, float dt);

	//Bucket Settings
	Vector3 BucketSize;
	int BucketCountX;
	int BucketCountY;

};

