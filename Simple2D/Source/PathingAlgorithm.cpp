#include "PathingAlgorithm.h"

#ifdef min
#undef min
#endif // min

#ifdef max
#undef max
#endif // max



PathingAlgorithm& PathingAlgorithm::GetInstance(void)
{
	static PathingAlgorithm path;
	return path;
}

bool PathingAlgorithm::TheAlgorithm(Request& _request)
{
	Grid& grid = Grid::GetInstance();
	std::vector<Grid::Node> nodeGrid = grid.GetNodeGrid();


	if (_request.isNewRequest)
	{
		//initialie everything and clear open lists

		m_target = _request.m_target;
		grid.ClearNodes();

		//clear priority queue?
	    //push starting node to the open list with cost of ?????

		Grid::Node* startingNode = grid.GetNodeByPosition(_request.m_start);
		startingNode->m_parent = nullptr;

		InsertNode(startingNode);

	}
	
	while (!m_openList.empty())
	{
		Grid::Node* parent = GetNextNode();

		if ((parent->m_position.x == m_target.x) && (parent->m_position.y == m_target.y))
		{
			Grid::Node* n = parent;

			while (true)
			{
				_request.m_finalPath.push_back(n->m_position);

				if (n->m_parent == nullptr)
				{
					break;
				}
				n = n->m_parent;
			}

			std::reverse(_request.m_finalPath.begin(), _request.m_finalPath.end());

			return true;
		}

		uint8_t pDir = parent->m_directions;

		for (uint8_t dir = 1; dir <= Grid::NodeDirections::DownLeft; dir <<= 1)
		{
			if (pDir & dir)
			{
				AddChild(parent, dir);
			}
		}
	}

	return false;
}

Grid::Node* PathingAlgorithm::GetNextNode(void)
{
	Grid::Node* node = m_sortedOpenList.top();

	node->m_onList = true;

	m_sortedOpenList.pop();

	return node;
}

void PathingAlgorithm::AddChild(Grid::Node* _node, uint8_t _direction)
{
	Grid& grid = Grid::GetInstance();

	DirectX::XMUINT2 position = _node->m_position;

	DirectX::XMUINT2 childPosition = position;
	float givenCost = 1;

	switch (_direction)
	{
	case Grid::NodeDirections::Up:
	{
		++childPosition.y;
		break;
	}
	case Grid::NodeDirections::Right:
	{
		++childPosition.x;
		break;
	}
	case Grid::NodeDirections::Down:
	{
		--childPosition.y;
		break;
	}
	case Grid::NodeDirections::Left:
	{
		--childPosition.x;
		break;
	}
	case Grid::NodeDirections::UpRight:
	{
		++childPosition.y;
		++childPosition.x;
		givenCost = 1.4f;
		break;
	}
	case Grid::NodeDirections::UpLeft:
	{
		++childPosition.y;
		--childPosition.x;
		givenCost = 1.4f;
		break;
	}
	case Grid::NodeDirections::DownRight:
	{
		--childPosition.y;
		++childPosition.x;
		givenCost = 1.4f;
		break;
	}
	case Grid::NodeDirections::DownLeft:
	{
		--childPosition.y;
		--childPosition.x;
		givenCost = 1.4f;
		break;
	}
	}

	givenCost += _node->m_givenCost;
	Grid::Node* child = grid.GetNodeByPosition(childPosition);

	if (child == _node->m_parent)
	{
		return;
	}

	float cost = CalculateCost(child);

	cost += givenCost;

	if (!child->m_onList || cost < child->m_finalCost)
	{
		child->m_givenCost = givenCost;
		child->m_finalCost = cost;
		child->m_parent = _node;

		InsertNode(child);
	}

}

float PathingAlgorithm::CalculateCost(Grid::Node* _node)
{
	//use octile huerstric

	int xDiff = abs(static_cast<int>(m_target.x) - static_cast<int>(_node->m_position.x));
	int yDiff = abs(static_cast<int>(m_target.y) - static_cast<int>(_node->m_position.y));

	int min = std::min(xDiff, yDiff);
	int max = std::max(xDiff, yDiff);




	return min * 1.41421356f + max - min;
}

void PathingAlgorithm::InsertNode(Grid::Node* _node)
{
	_node->m_onList = true;
	m_sortedOpenList.push(_node);
}





bool PathingAlgorithm::Compare::operator()(const Grid::Node* _nodeA, const Grid::Node* _nodeB) const
{
	return (_nodeA->m_finalCost > _nodeB->m_finalCost);
}
