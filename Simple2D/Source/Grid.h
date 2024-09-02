#pragma once
/**********************************************************************************************************************
*
* Author : Kiara Santiago
* File   : Grid.h
* Purpose: holds the sington grid -> this is data for A Star Algorithm
*
**********************************************************************************************************************/
#include <vector>                    //std vector
#include <directxtk12/SimpleMath.h>  //vector2, xmuint2

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
	//node data, for A Star Algorithm
	struct Node
	{
		Node* m_parent;              //parent to this node
		DirectX::XMUINT2 m_position; //position in BLOCKS for this node
		float m_finalCost;           //final cost of this node
		float m_givenCost;           //given cost of this node
		bool m_NotWall;              //true = not a wall, false = is a wall
		uint8_t m_directions;        //directions that are walkable from this node
		bool m_onList;               //is this current on the open list?
	};

	//the 8 directions
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

	DirectX::XMUINT2 GetNearbyNodeByPosition(DirectX::XMUINT2 _position);

	void ClearNodes(void);

	void OnGridFinish(void);

	DirectX::XMUINT2 WorldtoGrid(DirectX::SimpleMath::Vector2 _position);
	DirectX::SimpleMath::Vector2 GridtoWorld(DirectX::XMUINT2 _gridPosition);

	void Shutdown(void);

private:
	int m_gridWidth;           //width in BLOCKS of the grid
	int m_gridHeight;          //height in BLOCKS of the grid

	std::vector<Node> m_grid;  //all the blocks on the grid
};