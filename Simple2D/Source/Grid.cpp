#include "Grid.h"
#include "LevelGeneration.h"

Grid& Grid::GetInstance(void)
{
	static Grid grid;
	return grid;
}

std::vector<Grid::Node> Grid::GetNodeGrid(void)
{
	return m_grid;
}

void Grid::AddNodeByPosition(DirectX::XMUINT2 _position, bool _notWall)
{
	Node temp;
	temp.m_position = _position;
	temp.m_givenCost = 0;
	temp.m_finalCost = 0;
	temp.m_parent = nullptr;
	temp.m_NotWall = _notWall;
	temp.m_onList = false;
	temp.m_directions = 0;

	m_grid.push_back(temp);
}

void Grid::SetWidthAndHeight(int _w, int _h)
{
	m_gridWidth = _w;
	m_gridHeight = _h;
}

Grid::Node* Grid::GetNodeByPosition(DirectX::XMUINT2 _position)
{
	return &m_grid[_position.y * m_gridWidth + _position.x];
}

void Grid::ClearNodes(void)
{
	for (int k = 0; k < m_grid.size(); ++k)
	{
		m_grid[k].m_finalCost = 0;
		m_grid[k].m_givenCost = 0;
		m_grid[k].m_onList = false;
		m_grid[k].m_parent = nullptr;
	}
}

void Grid::OnGridFinish(void)
{
	for (int k = 0; k < m_grid.size(); ++k)
	{
		Grid::Node& node = m_grid[k];

		node.m_directions = 0;

		DirectX::XMUINT2 pos = node.m_position;

		if (pos.x > 0)
		{
			--pos.x;

			if (GetNodeByPosition(pos)->m_NotWall)
			{
				node.m_directions |= NodeDirections::Left;
			}
			++pos.x;
		}


		if (pos.x < (m_gridWidth - 1))
		{
			++pos.x;

			if (GetNodeByPosition(pos)->m_NotWall)
			{
				node.m_directions |= NodeDirections::Right;
			}
			--pos.x;
		}


		if (pos.y > 0)
		{
			--pos.y;
			if (GetNodeByPosition(pos)->m_NotWall)
			{
				node.m_directions |= NodeDirections::Down;

				if (node.m_directions & NodeDirections::Right)
				{
					++pos.x;
					if (GetNodeByPosition(pos)->m_NotWall)
					{
						node.m_directions |= NodeDirections::DownRight;
					}
					--pos.x;

				}

				if (node.m_directions & NodeDirections::Left)
				{
					--pos.x;
					if (GetNodeByPosition(pos)->m_NotWall)
					{
						node.m_directions |= NodeDirections::DownLeft;
					}
					++pos.x;

				}
			}
			++pos.y;
		}

		if (pos.y < (m_gridHeight - 1))
		{
			++pos.y;
			if (GetNodeByPosition(pos)->m_NotWall)
			{
				node.m_directions |= NodeDirections::Up;

				if (node.m_directions & NodeDirections::Right)
				{
					++pos.x;
					if (GetNodeByPosition(pos)->m_NotWall)
					{
						node.m_directions |= NodeDirections::UpRight;
					}
					--pos.x;

				}

				if (node.m_directions & NodeDirections::Left)
				{
					--pos.x;
					if (GetNodeByPosition(pos)->m_NotWall)
					{
						node.m_directions |= NodeDirections::UpLeft;
					}
					++pos.x;

				}
			}
			--pos.y;
		}

	}
}

DirectX::XMUINT2 Grid::WorldtoGrid(DirectX::SimpleMath::Vector2 _position)
{
	LevelGeneration& level = LevelGeneration::GetInstance();
	DirectX::SimpleMath::Vector2 size = level.GetSize();


	DirectX::XMUINT2 gridPosition = { 0,0 };

	int x = static_cast<int>(roundf(_position.x));
	int y = static_cast<int>(roundf(_position.y));

	gridPosition.x = x / static_cast<int>(size.x);
	gridPosition.y = y / static_cast<int>(size.y);

	return gridPosition;
}

DirectX::SimpleMath::Vector2 Grid::GridtoWorld(DirectX::XMUINT2 _gridPosition)
{
	LevelGeneration& level = LevelGeneration::GetInstance();
	DirectX::SimpleMath::Vector2 size = level.GetSize();

	DirectX::SimpleMath::Vector2 position = { 25, 25 };

	position.x += _gridPosition.x * size.x;
	position.y += _gridPosition.y * size.y;

	return position;
}
