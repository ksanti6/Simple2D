/**********************************************************************************************************************
*
* Author : Kiara Santiago
* File   : PathingAlgorithm.cpp
* Purpose: the implementation of the A Star algorithm
*
**********************************************************************************************************************/
#include "PathingAlgorithm.h"

//undefining the min and max macros so i can use std:: min and max 
#ifdef min
#undef min
#endif // min

#ifdef max
#undef max
#endif // max


/************************************************
*
* getting an instance of the algorithm
*
************************************************/
PathingAlgorithm& PathingAlgorithm::GetInstance(void)
{
	static PathingAlgorithm path;
	return path;
}

/************************************************
*
* init - ensuring we have a fresh priority queue
*
************************************************/
void PathingAlgorithm::Init(void)
{
	m_sortedOpenList = std::priority_queue<Grid::Node*, std::vector<Grid::Node*>, Compare>(Compare(), m_openList);
}

/************************************************
*
* A Star Algorithm, returns true if the algorithm
* is complete
*
************************************************/
bool PathingAlgorithm::TheAlgorithm(Request& _request)
{
	Grid& grid = Grid::GetInstance();
	std::vector<Grid::Node> nodeGrid = grid.GetNodeGrid();


	if (_request.isNewRequest)
	{
		//for the new request make sure to clear all data of last request
		//and get ready for new request
		_request.isNewRequest = false;
		m_target = _request.m_target;
		grid.ClearNodes();
		m_openList.clear();
		m_sortedOpenList = std::priority_queue<Grid::Node*, std::vector<Grid::Node*>, Compare>(Compare(), m_openList);

	    //get the starting node for new request and add it to open list
		Grid::Node* startingNode = grid.GetNodeByPosition(_request.m_start);
		startingNode->m_parent = nullptr;

		InsertNode(startingNode);

	}
	
	//while there are still nodes to process on open list
	while (!m_sortedOpenList.empty())
	{
		Grid::Node* parent = GetNextNode();

		//is this node the target / end goal?
		if ((parent->m_position.x == m_target.x) && (parent->m_position.y == m_target.y))
		{
			//yes
			//make the completed path and stop the algorithm

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

		//no
		//find the children of current node and add them to the open list

		uint8_t pDir = parent->m_directions;

		//check in all 8 directions of node for children
		for (uint16_t dir = 1; dir <= Grid::NodeDirections::DownLeft; dir <<= 1)
		{
			if (pDir & dir)
			{
				AddChild(parent, static_cast<uint8_t>(dir));
			}
		}
	}
	//no more nodes to process and algorithm did not finish
	return false;
}

/************************************************
*
* get the next node from the open list to 
* process
*
************************************************/
Grid::Node* PathingAlgorithm::GetNextNode(void)
{
	Grid::Node* node = m_sortedOpenList.top();

	node->m_onList = true;

	m_sortedOpenList.pop();

	return node;
}

/************************************************
*
* find the child in the specific direction of the
* node and see if we can add it to the open list
*
************************************************/
void PathingAlgorithm::AddChild(Grid::Node* _node, uint8_t _direction)
{
	Grid& grid = Grid::GetInstance();

	//position of the node we want to find children for
	DirectX::XMUINT2 position = _node->m_position;

	DirectX::XMUINT2 childPosition = position;
	float givenCost = 1;

	//adjust childs position based on direction
	//diagonal child get higher cost
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

	//get the child node and calculate its cost
	givenCost += _node->m_givenCost;
	Grid::Node* child = grid.GetNodeByPosition(childPosition);

	if (child == _node->m_parent)
	{
		//the nodes parent cannot be a child 
		return;
	}

	float cost = CalculateCost(child);

	cost += givenCost;

	//we want the child if its not on the list OR
	//if its cost is cheaper than its other self on the list
	if (!child->m_onList || cost < child->m_finalCost)
	{
		child->m_givenCost = givenCost;
		child->m_finalCost = cost;
		child->m_parent = _node;

		InsertNode(child);
	}
}

/************************************************
*
* we calculate the cost of the node based on the
* octile heuristic
*
************************************************/
float PathingAlgorithm::CalculateCost(Grid::Node* _node)
{
	//this uses the diagonal distance
	int xDiff = abs(static_cast<int>(m_target.x) - static_cast<int>(_node->m_position.x));
	int yDiff = abs(static_cast<int>(m_target.y) - static_cast<int>(_node->m_position.y));

	int min = std::min(xDiff, yDiff);
	int max = std::max(xDiff, yDiff);

	//1.41421356f is the magic number of the day!
	//its the sqrt of 2
	return min * 1.41421356f + max - min;
}

/************************************************
*
* add the given node to the open list
*
************************************************/
void PathingAlgorithm::InsertNode(Grid::Node* _node)
{
	_node->m_onList = true;
	m_sortedOpenList.push(_node);
}

/************************************************
*
* shutdown - clear all the lists
*
************************************************/
void PathingAlgorithm::Shutdown(void)
{
	m_openList.clear();
	m_sortedOpenList = std::priority_queue<Grid::Node*, std::vector<Grid::Node*>, Compare>(Compare(), m_openList);
	m_target = { 0,0 };

}

/************************************************
*
* custom compare function for priority queue
* compares the nodes final costs
*
************************************************/
bool PathingAlgorithm::Compare::operator()(const Grid::Node* _nodeA, const Grid::Node* _nodeB) const
{
	return (_nodeA->m_finalCost > _nodeB->m_finalCost);
}