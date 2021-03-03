#include <vector>
#include <algorithm>
#include <queue>
#include <utility>

class Pathfinding 
{
public:

private:
	struct sNode
	{
		//bool Obstacle = false;			// Is the node an obstruction?
		bool Visited = false;			// Have we searched this node before?
		float fGlobalGoal;				// Distance to goal so far
		float fLocalGoal;				// Distance to goal if we took the alternative route
		int x;							// Nodes position in 2D space
		int y;
		std::vector<sNode*> vecNeighbours;	// Connections to neighbours
		sNode* parent;					// Node connecting to this node that offers shortest parent
	};

	sNode* nodes = nullptr;
	int nMapWidth = 16;
	int nMapHeight = 16;

	sNode* nodeStart = nullptr;
	sNode* nodeEnd = nullptr;
};