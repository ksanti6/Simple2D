#pragma once
#include <vector>
#include <directxtk12/SimpleMath.h>

class Grid
{
private:
	explicit Grid() 
	{
		m_gridWidth = 0;
		m_gridHeight = 0;
	}
	Grid(const Grid&) = delete;
	void operator=(const Grid&) = delete;
public:
	struct Node
	{
		Node* m_parent;
		DirectX::XMUINT2 m_position;
		float m_finalCost;
		float m_givenCost;
		bool m_NotWall;
		uint8_t m_directions;
		bool m_onList;
	};

	enum NodeDirections : uint8_t
	{
		Up = 1,
		Right = 1 << 1,
		Down = 1 << 2,
		Left = 1 << 3,
		UpRight = 1 << 4,
		UpLeft = 1 << 5,
		DownRight = 1 << 6,
		DownLeft = 1 << 7
	};

	static Grid& GetInstance(void);

	std::vector<Node> GetNodeGrid(void);

	void AddNodeByPosition(DirectX::XMUINT2 _position, bool _notWall);

	void SetWidthAndHeight(int _w, int _h);

	Node* GetNodeByPosition(DirectX::XMUINT2 _position);

	void ClearNodes(void);

	void OnGridFinish(void);

	DirectX::XMUINT2 WorldtoGrid(DirectX::SimpleMath::Vector2 _position);
	DirectX::SimpleMath::Vector2 GridtoWorld(DirectX::XMUINT2 _gridPosition);

	void Shutdown(void);

private:
	int m_gridWidth;
	int m_gridHeight;

	std::vector<Node> m_grid;
};