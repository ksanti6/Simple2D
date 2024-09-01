#pragma once
#include <vector>
#include <queue>
#include "Grid.h"
#include <directxtk12/SimpleMath.h>

class PathingAlgorithm
{
private:
	explicit PathingAlgorithm() 
	{
		m_target = { 0,0 };
	}
	PathingAlgorithm(const PathingAlgorithm&) = delete;
	void operator=(const PathingAlgorithm&) = delete;

	class Compare
	{
	public:
		bool operator()(const Grid::Node* _nodeA, const Grid::Node* _nodeB) const;
	};

public:

	struct Request
	{
		DirectX::XMUINT2 m_start = { 0,0 };
		DirectX::XMUINT2 m_target = { 0,0 };

		bool isNewRequest = true;

		std::vector<DirectX::XMUINT2> m_finalPath;
	};
	static PathingAlgorithm& GetInstance(void);

	bool TheAlgorithm(Request& _request);

	Grid::Node* GetNextNode(void);

	void AddChild(Grid::Node* _node, uint8_t _direction);

	float CalculateCost(Grid::Node* _node);

	void InsertNode(Grid::Node* _node);


private:
	DirectX::XMUINT2 m_target;

	std::vector<Grid::Node*> m_openList;
	std::priority_queue<Grid::Node*, std::vector<Grid::Node*>, Compare> m_sortedOpenList;
};