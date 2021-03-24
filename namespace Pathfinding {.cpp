namespace Pathfinding {
	struct Node {
		Node(const Vec2i& gridPos, bool obstacle = false, const Vec2f& worldPos = { 0.0f,0.0f })
			:
			_grid_pos(gridPos),
			_world_pos(worldPos),
			_obstacle(obstacle)
		{}
		bool operator>(Node& rhs) {
			return rhs.FCost() < FCost();
		}
		int operator-(const Node& rhs) const {
			int distance_x = abs(_grid_pos.x - rhs._grid_pos.x);
			int distance_y = abs(_grid_pos.y - rhs._grid_pos.y);
			if (distance_x > distance_y) {
				return distance_y * 14 + (distance_x - distance_y) * 10;
			}
			return distance_x * 14 + (distance_y - distance_x) * 10;
		}

		int		_g_cost = 0, _h_cost = 0;
		Vec2i	_grid_pos{ 0,0 };
		Vec2f	_world_pos{ 0.0f,0.0f };
		bool	_obstacle{ false };
		Node* _parent{ nullptr };
		bool	_closed{ false };
		bool	_open{ false };

		int FCost() { return _g_cost + _h_cost; }
	};
	struct Grid {
		Grid(int width, int height, const std::vector<bool> grid)
			:
			_width(width),
			_height(height)
		{
			for (size_t y = 0; y < height; ++y) {
				for (size_t x = 0; x < width; ++x)
					_grid.emplace_back(Vec2i(x, y), grid[y * width + x]); {
				}
			}
		}
		size_t _width{ 0 };
		size_t _height{ 0 };
		std::vector<Node> _grid;
		Node& Get(const Vec2i& pos) {
			return _grid[pos.y * _width + pos.x];
		}
		void GetNeighbours(Node*& node, std::vector<Node*>& neighbours) {
			neighbours.clear();
			if (node->_grid_pos.x - 1 >= 0) {
				neighbours.push_back(&Get({ node->_grid_pos.x - 1, node->_grid_pos.y }));
			}
			if (node->_grid_pos.x + 1 < _width) {
				neighbours.push_back(&Get({ node->_grid_pos.x + 1, node->_grid_pos.y }));
			}
			if (node->_grid_pos.y - 1 >= 0) {
				neighbours.push_back(&Get({ node->_grid_pos.x, node->_grid_pos.y - 1 }));
			}
			if (node->_grid_pos.y + 1 < _height) {
				neighbours.push_back(&Get({ node->_grid_pos.x, node->_grid_pos.y + 1 }));
			}
		}
	};
}

struct Sys_Pathfinding_v2 : public System {
	void UpdateComponent() override {

	}
	std::vector<Pathfinding::Node*> _nodes_to_reset;		// rmb to reserve, PESSIMISM! or something like that
	std::vector<Pathfinding::Node*> _neighbours;			// rmb to reserve

	bool SolveAStar(const Vec2i& start, const Vec2i& end, Pathfinding::Grid& grid, std::vector<Vec2i>& path) {
		// custom comparator
		auto cmp = [](Pathfinding::Node*& node1, Pathfinding::Node*& node2) {return *node1 > *node2; };
		// create min heap
		std::priority_queue<Pathfinding::Node*, std::vector<Pathfinding::Node*>, decltype(cmp)> min_heap(cmp);

		// create start and end temp nodes
		Pathfinding::Node* start_node = &grid.Get(start);
		Pathfinding::Node* end_node = &grid.Get(end);

		// add start node to the open set
		min_heap.push(start_node);
		start_node->_open = true;
		_nodes_to_reset.push_back(start_node);

		// loop
		while (min_heap.size() > 0) {
			Pathfinding::Node* current_node = min_heap.top();

			// erase current node from open set and add to closed set
			min_heap.pop();
			current_node->_open = false;
			current_node->_closed = true;

			// if current == end, reached
			if (current_node == end_node) {
				RetracePath(start_node, end_node, path);
				return true;
			}

			// find/update fcost of neighbours and add them to open set
			grid.GetNeighbours(current_node, _neighbours);
			for (auto& n : _neighbours) {
				// if obstacle or closed skip
				if (n->_obstacle || n->_closed) {
					continue;
				}
				// if new g cost < g cost (need updating) || or if not in open, calculate f cost, add to open
				int new_g_cost = current_node->_g_cost + (*current_node - *n);
				// check
				bool in_open = n->_open;
				if (new_g_cost < n->_g_cost || !in_open) {
					// set fcost
					n->_g_cost = new_g_cost;
					n->_h_cost = (*n - *end_node);
					// set parent node
					n->_parent = current_node;
					if (!in_open) {
						min_heap.push(n);
						n->_open = true;
					}
					_nodes_to_reset.push_back(n);
				}
			}
		}
		return false;
	}
	void RetracePath(const Pathfinding::Node* start, const Pathfinding::Node* end, std::vector<Vec2i>& path) {
		path.clear();
		Pathfinding::Node const* current = end;
		while (start != current) {
			path.push_back(current->_grid_pos);
			current = current->_parent;
		}
		std::reverse(path.begin(), path.end());
		for (auto& n : _nodes_to_reset) {
			ResetNode(n);
		}
		_nodes_to_reset.clear();
	}
	void ResetNode(Pathfinding::Node*& node) {
		node->_g_cost = 0;
		node->_h_cost = 0;
		node->_closed = false;
		node->_open = false;
	}
};