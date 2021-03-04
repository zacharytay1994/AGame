#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include "zEntity.h"
#include "CSHeaderDef.h"
#include "Factory.h"
#include "ResourceManager.h"
using namespace std;


class PathFinding
{
	public:
		PathFinding() 
		{}
		void MapCreate()
		{
			// Create a 2D array of nodes - this is for convenience of rendering and construction
			// and is not required for the algorithm to work - the nodes could be placed anywhere
			// in any space, in multiple dimension
			int MapArea = nMapWidth * nMapHeight;
			nodes = new sNode[MapArea];
			for (int x = 0; x < nMapWidth; x++)
				for (int y = 0; y < nMapHeight; y++)
				{
					nodes[y * nMapWidth + x].x = x; // to give each node its own coordinates
					nodes[y * nMapWidth + x].y = y;
					// set everything to default value 1st
					nodes[y * nMapWidth + x].bObstacle = false;
					nodes[y * nMapWidth + x].parent = nullptr;
					nodes[y * nMapWidth + x].bVisited = false;
				}

			// Create connections - in this case nodes are on a regular grid
			for (int x = 0; x < nMapWidth; x++)
				for (int y = 0; y < nMapHeight; y++)
				{
					if (y > 0)
						nodes[y * nMapWidth + x].vecNeighbours.push_back(&nodes[(y - 1) * nMapWidth + (x + 0)]);
					if (y < nMapHeight - 1)
						nodes[y * nMapWidth + x].vecNeighbours.push_back(&nodes[(y + 1) * nMapWidth + (x + 0)]);
					if (x > 0)
						nodes[y * nMapWidth + x].vecNeighbours.push_back(&nodes[(y + 0) * nMapWidth + (x - 1)]);
					if (x < nMapWidth - 1)
						nodes[y * nMapWidth + x].vecNeighbours.push_back(&nodes[(y + 0) * nMapWidth + (x + 1)]);

				}

			// Manually positio the start and end markers so they are not nullptr
			nodeStart = &nodes[(nMapHeight / 2) * nMapWidth + 1];
			nodeEnd = &nodes[(nMapHeight / 2) * nMapWidth + nMapWidth - 2];
		}

	bool Solve_AStar()
	{
		// Reset Navigation Graph - default all node states
		for (int x = 0; x < nMapWidth; x++)
			for (int y = 0; y < nMapHeight; y++)
			{
				nodes[y * nMapWidth + x].bVisited = false;
				nodes[y * nMapWidth + x].fGlobalGoal = INFINITY;
				nodes[y * nMapWidth + x].fLocalGoal = INFINITY;
				nodes[y * nMapWidth + x].parent = nullptr;	// No parents
			}

		auto distance = [](sNode* a, sNode* b) // For convenience
		{
			return sqrtf((a->x - b->x) * (a->x - b->x) + (a->y - b->y) * (a->y - b->y));
		};

		auto heuristic = [distance](sNode* a, sNode* b) // So we can experiment with heuristic
		{
			return distance(a, b);
		};

		// Setup starting conditions
		sNode* nodeCurrent = nodeStart;
		nodeStart->fLocalGoal = 0.0f;
		nodeStart->fGlobalGoal = heuristic(nodeStart, nodeEnd);

		// Add start node to not tested list - this will ensure it gets tested.
		// As the algorithm progresses, newly discovered nodes get added to this
		// list, and will themselves be tested later
		list<sNode*> listNotTestedNodes;
		listNotTestedNodes.push_back(nodeStart);

		// if the not tested list contains nodes, there may be better paths
		// which have not yet been explored. However, we will also stop 
		// searching when we reach the target - there may well be better
		// paths but this one will do - it wont be the longest.
		while (!listNotTestedNodes.empty() && nodeCurrent != nodeEnd)// Find absolutely shortest path // && nodeCurrent != nodeEnd)
		{
			// Sort Untested nodes by global goal, so lowest is first
			listNotTestedNodes.sort([](const sNode* lhs, const sNode* rhs) { return lhs->fGlobalGoal < rhs->fGlobalGoal; });

			// Front of listNotTestedNodes is potentially the lowest distance node. Our
			// list may also contain nodes that have been visited, so ditch these...
			while (!listNotTestedNodes.empty() && listNotTestedNodes.front()->bVisited)
				listNotTestedNodes.pop_front();

			// ...or abort because there are no valid nodes left to test
			if (listNotTestedNodes.empty())
				break;

			nodeCurrent = listNotTestedNodes.front();
			nodeCurrent->bVisited = true; // We only explore a node once


			// Check each of this node's neighbours...
			for (auto nodeNeighbour : nodeCurrent->vecNeighbours)
			{
				// ... and only if the neighbour is not visited and is 
				// not an obstacle, add it to NotTested List
				if (!nodeNeighbour->bVisited && nodeNeighbour->bObstacle == 0)
					listNotTestedNodes.push_back(nodeNeighbour);

				// Calculate the neighbours potential lowest parent distance
				float fPossiblyLowerGoal = nodeCurrent->fLocalGoal + distance(nodeCurrent, nodeNeighbour);

				// If choosing to path through this node is a lower distance than what 
				// the neighbour currently has set, update the neighbour to use this node
				// as the path source, and set its distance scores as necessary
				if (fPossiblyLowerGoal < nodeNeighbour->fLocalGoal)
				{
					nodeNeighbour->parent = nodeCurrent;
					nodeNeighbour->fLocalGoal = fPossiblyLowerGoal;

					// The best path length to the neighbour being tested has changed, so
					// update the neighbour's score. The heuristic is used to globally bias
					// the path algorithm, so it knows if its getting better or worse. At some
					// point the algo will realise this path is worse and abandon it, and then go
					// and search along the next best path.
					nodeNeighbour->fGlobalGoal = nodeNeighbour->fLocalGoal + heuristic(nodeNeighbour, nodeEnd);
				}
			}
		}

		return true;
	}

	private:

		struct sNode
		{
			bool bObstacle = false;			// Is the node an obstruction?
			bool bVisited = false;			// Have we searched this node before?
			float fGlobalGoal = 0.0f;				// Distance to goal so far
			float fLocalGoal = 0.0f;				// Distance to goal if we took the alternative route
			int x = 0;							// Nodes position in 2D space
			int y = 0;
			vector<sNode*> vecNeighbours;	// Connections to neighbours
			sNode* parent = nullptr;					// Node connecting to this node that offers shortest parent
		};

		sNode* nodes = nullptr;
		int nMapWidth = 16;
		int nMapHeight = 16;

		sNode* nodeStart = nullptr;
		sNode* nodeEnd = nullptr;

};
