#pragma once
/**********************************************************************************************************************
*
* Author : Kiara Santiago
* File   : LevelGeneration.h
* Purpose: singleton of level generation - loads in data from a file to place all the objects in the level
*
**********************************************************************************************************************/
#include<string>                       //string
#include<vector>                       //std vector
#include <directxtk12/SimpleMath.h>    //vector2
#include "Cheese.h"                    //we hold the cheese

class LevelGeneration
{
private:
	explicit LevelGeneration() 
	{
		m_mapHeight = 0;
		m_mapWidth = 0;
	}
	LevelGeneration(const LevelGeneration&) = delete;
	void operator= (const LevelGeneration&) = delete;

	void ReadFromFile(void);
	
public:

	static LevelGeneration& GetInstance(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Shutdown(void);

	DirectX::SimpleMath::Vector2 GetSize(void);

	void ResetPlayerEnemyPositions(void);

private:

	std::vector<std::string> m_filePath;                        //holds all the possible levels to spawn
	int m_currentLevel;                                         //the chosen level spawned from vector

	int m_mapWidth;                                             //width in BLOCKS of the map
	int m_mapHeight;                                            //height in BLOCKS of the map

	DirectX::SimpleMath::Vector2 m_startingEnemy;               //enemy position from read in file
	DirectX::SimpleMath::Vector2 m_startingPlayer;              //player position from read in file

	DirectX::SimpleMath::Vector2 m_imageSize;                   //W x H of all objects on the map

	std::vector<DirectX::SimpleMath::Vector2> m_wallPositions;  //container of wall positions for collision
	std::vector<Cheese> m_cheese;                               //container of all cheese objects
};