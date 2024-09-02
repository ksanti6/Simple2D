/**********************************************************************************************************************
*
* Author : Kiara Santiago
* File   : LevelGeneration.cpp
* Purpose: implementation of level generation -> read from txt file to spawn objects
*
**********************************************************************************************************************/
#include "LevelGeneration.h"
#include "Graphics.h"            //for drawing cheese and walls
#include "Player.h"              //for setting players spawn position and wall collision
#include "Enemy.h"               //for setting enemies spawn position and wall collision
#include "Collision.h"           //for wall collision with player and enemy
#include "Grid.h"                //for initializing grid values

/************************************************
*
* we read from the chosen txt file to set a bunch
* of values
*
************************************************/
void LevelGeneration::ReadFromFile(void)
{
	//open the file
	FILE* file;
	fopen_s(&file, m_filePath[m_currentLevel].c_str(), "rt");

	if (!file)
	{
		printf_s("ERROR: LevelGeneration, file could not open.\n");
	}

	//read in the first 2 values (width x height of map)
	if (file)
	{
		fscanf_s(file, "%i", &m_mapWidth);
		fscanf_s(file, "%i", &m_mapHeight);
	}
	
	//set up some variables to help iterate thru data in file
	//very first position is in the top left corner
	//positions are based on the center of the object so it must be half the object size
	DirectX::SimpleMath::Vector2 currentPos = { m_imageSize.x / 2.0f, m_imageSize.y / 2.0f};
	uint32_t currentWidth = 0;
	uint32_t currentHeight = 0;

	Player& player = Player::GetInstance();
	Enemy& enemy = Enemy::GetInstance();
	Grid& grid = Grid::GetInstance();

	grid.SetWidthAndHeight(m_mapWidth, m_mapHeight);

	//while there is still data to be read
	while (file)
	{
		//get the value
		int value = -1;
		fscanf_s(file, "%i", &value);
		
		//interpret the value
		if (value == 1)  //walls
		{
			m_wallPositions.push_back(currentPos);
			grid.AddNodeByPosition({currentWidth, currentHeight}, false);
		}
		else
		{
			grid.AddNodeByPosition({currentWidth, currentHeight}, true);

			if (value == 2) //player
			{
				m_startingPlayer = currentPos;
				player.SetPosition(currentPos);
			}
			else if (value == 3) //enemy
			{
				m_startingEnemy = currentPos;
				enemy.SetPosition(currentPos);
			}
			else if (value == 4) //chese
			{
				Cheese temp(currentPos, 100);
				m_cheese.push_back(temp);
			}
		}
		
		//increment position - by length (or half depending) of the objects size
		++currentWidth;
		currentPos.x += m_imageSize.x;

		//did we reach the end of the row?
		if (currentWidth >= m_mapWidth)
		{
			currentWidth = 0;
			++currentHeight;
			currentPos.x = m_imageSize.x / 2.0f;
			currentPos.y += m_imageSize.y;
		}

		//did we reach the end of the collumn?
		if (currentHeight >= m_mapHeight)
		{
			break;
		}
	}

	//close the file
	if (file)
	{
		fclose(file);
	}

	//determine directions of the items on the grid (this is for pathing algorithm)
	grid.OnGridFinish();
}

/************************************************
*
* get an instance of the level generation
*
************************************************/
LevelGeneration& LevelGeneration::GetInstance(void)
{
	static LevelGeneration level;
	return level;
}

/************************************************
*
* initialize the level generation data
*
************************************************/
void LevelGeneration::Init(void)
{
	//seed for random so its actually random
	srand(time(0));

	//locations of the level data files
	m_filePath.push_back("./Asset/Level_A.txt");
	m_filePath.push_back("./Asset/Level_B.txt");
	m_filePath.push_back("./Asset/Level_C.txt");
	m_filePath.push_back("./Asset/Level_D.txt");

	//choose file to load and load it
	m_currentLevel = rand() % m_filePath.size();
	
	m_imageSize = { 50, 50 };

	ReadFromFile();
}

/************************************************
*
* handles collisions for wall and cheese related
* collisions and if cheese has been consumed
*
************************************************/
void LevelGeneration::Update(void)
{
	Player& player = Player::GetInstance();
	Enemy& enemy = Enemy::GetInstance();

	bool isColliding = false;

	//wall collisions
	for (int k = 0; k < m_wallPositions.size(); ++k)
	{
		//check for player colliding against walls
		isColliding = Collision::CheckCollision(player.GetPosition(),
			player.GetSize(), m_wallPositions[k], m_imageSize);

		if (isColliding)
		{
			player.ResolveWallCollision(m_wallPositions[k], m_imageSize);
		}

		//check for enemy colliding against walls
		isColliding = Collision::CheckCollision(enemy.GetPosition(),
			enemy.GetSize(), m_wallPositions[k], m_imageSize);
		
		if (isColliding)
		{
			enemy.ResolveWallCollision(m_wallPositions[k],m_imageSize);
		}
	}

	//cheese collision - player only
	for (int k = 0; k < m_cheese.size(); ++k)
	{
		isColliding = Collision::CheckCollision(player.GetPosition(),
			player.GetSize(), m_cheese[k].GetPosition(), m_imageSize);

		if (isColliding)
		{
			m_cheese[k].ResolvePlayerCollision();
		}
	}

	//get rid of consumed cheese
	for (int k = 0; k < m_cheese.size(); ++k)
	{
		if (m_cheese[k].GetShouldDestroy())
		{
			m_cheese.erase(m_cheese.begin()+ k);
		}
	}
}

/************************************************
*
* draw the walls and cheese
*
************************************************/
void LevelGeneration::Draw(void)
{
	Graphics& graphics = Graphics::GetInstance();

	//walls
	for (int k = 0; k < m_wallPositions.size(); ++k)
	{
		graphics.Draw(Graphics::Textures::wall, m_wallPositions[k], m_imageSize);
	}

	//cheese
	for (int k = 0; k < m_cheese.size(); ++k)
	{
		graphics.Draw(Graphics::Textures::cheese, m_cheese[k].GetPosition(), m_imageSize);
	}
}

/************************************************
*
* shutdown - clears wall and cheese vectors
*
************************************************/
void LevelGeneration::Shutdown(void)
{
	m_wallPositions.clear();
	m_cheese.clear();
}

/************************************************
*
* getter - W x H of the objects
*
************************************************/
DirectX::SimpleMath::Vector2 LevelGeneration::GetSize(void)
{
	return m_imageSize;
}

/************************************************
*
* resets the player and enemies positions to the
* spawn positions
*
************************************************/
void LevelGeneration::ResetPlayerEnemyPositions(void)
{
	Player& player = Player::GetInstance();
	Enemy& enemy = Enemy::GetInstance();

	player.SetPosition(m_startingPlayer);
	enemy.SetPosition(m_startingEnemy);
}