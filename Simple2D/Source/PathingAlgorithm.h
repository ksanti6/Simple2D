#pragma once
/**********************************************************************************************************************
*
* Author : Kiara Santiago
* File   : PathingAlgorithm.h
* Purpose: a singleton that holds the A Star Pathing Algorithm
*
**********************************************************************************************************************/
#include <vector>                    //for std::vector
#include <queue>                     //for priority queue
#include "Grid.h"                    //for Node

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

	Grid::Node* GetNextNode(void);

	void AddChild(Grid::Node* _node, uint8_t _direction);
	float CalculateCost(Grid::Node* _node);
	void InsertNode(Grid::Node* _node);

public:
	//data for requesting new path from algorithm
	struct Request
	{
		DirectX::XMUINT2 m_start = { 0,0 };        //where the algorithm starts path
		DirectX::XMUINT2 m_target = { 0,0 };       //where the algorithm shoudl end the path

		bool isNewRequest = true;                  //is this the first time we are requesting this particular path

		std::vector<DirectX::XMUINT2> m_finalPath; //where we will store the completed path
	};

	static PathingAlgorithm& GetInstance(void);
	void Init(void);

	bool TheAlgorithm(Request& _request);
	void Shutdown(void);

private:
	DirectX::XMUINT2 m_target;                  //end goal for the path we are currently working on

	std::vector<Grid::Node*> m_openList;        //working list for finding path
	std::priority_queue<Grid::Node*, std::vector<Grid::Node*>, Compare> m_sortedOpenList;  //our open list sorted by final cost
};