/**********************************************************************************************************************
*
* Author : Kiara Santiago
* File   : LevelGeneration.cpp
* Purpose: implementation of level generation -> read from txt file to spawn objects
*
**********************************************************************************************************************/
#include "LevelGeneration.h"
#include "Graphics.h"            //for drawing cheese and walls
#include "Player.h"              //for setting players spawn position and collision
#include "Collision.h"           //for collision checks for all/most objects
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
	float baseSpeed = 125.0f;
	int speedVariant = 50;

	Player& player = Player::GetInstance();
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
				Enemy temp(baseSpeed + (rand() % speedVariant), currentPos);
				m_enemy.push_back(temp);
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
* takes a level so we know what level to spawn
*
************************************************/
void LevelGeneration::Init(int _level)
{
	//seed for random so its actually random
	srand(time(0));

	//locations of the level data files
	m_filePath.push_back("./Asset/Level_A.txt");
	m_filePath.push_back("./Asset/Level_B.txt");
	m_filePath.push_back("./Asset/Level_C.txt");
	m_filePath.push_back("./Asset/Level_D.txt");

	//choose file to load and load it
	if (_level == 0)
	{
		m_currentLevel = rand() % m_filePath.size();
	}
	else
	{
		m_currentLevel = _level - 1;
	}
	
	m_imageSize = { 50, 50 };

	ReadFromFile();
}

/************************************************
*
* handles updating and collisons for walls,
* cheese, and enemies
*
************************************************/
void LevelGeneration::Update(float _deltaTime)
{
	//update enemies
	for (int k = 0; k < m_enemy.size(); ++k)
	{
		m_enemy[k].Update(_deltaTime);
	}

	Player& player = Player::GetInstance();

	bool isColliding = false;
	DirectX::SimpleMath::Vector2 resolve = { 0,0 };

	//wall collisions
	for (int k = 0; k < m_wallPositions.size(); ++k)
	{
		//check for player colliding against walls
		//we use circle box collision for player for smoother rounding of corners
		resolve = Collision::CheckResolveCircleBoxCollision(player.GetPosition(), player.GetSize().x / 2.0f,
			m_wallPositions[k], m_imageSize);

		player.SetPosition(player.GetPosition() + resolve);

		//check for enemy colliding against walls
		//box to box for enemy
		for (int j = 0; j < m_enemy.size(); ++j)
		{
			isColliding = Collision::CheckCollision(m_enemy[j].GetPosition(),
				m_enemy[j].GetSize(), m_wallPositions[k], m_imageSize);

			if (isColliding)
			{
				m_enemy[j].ResolveWallCollision(m_wallPositions[k], m_imageSize);
			}
		}
	}

	//player and enemy collisions
	for (int k = 0; k < m_enemy.size(); ++k)
	{
		isColliding = Collision::CheckCollision(player.GetPosition(),
			player.GetSize(), m_enemy[k].GetPosition(), m_enemy[k].GetSize());

		if (isColliding)
		{
			player.AdjustLives(1);
			player.AdjustScore(-100);

			//not dead yet
			if (player.GetLives() > 0)
			{
				//reset positions
				ResetPlayerEnemyPositions();
			}
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
* draw the walls, enemys, cheese
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

	//enemy
	for (int k = 0; k < m_enemy.size(); ++k)
	{
		graphics.Draw(Graphics::Textures::enemy, m_enemy[k].GetPosition(), m_imageSize);
	}
}

/************************************************
*
* shutdown - clears wall and cheese vectors
* shutdown enemies then clear
*
************************************************/
void LevelGeneration::Shutdown(void)
{
	for (int k = 0; k < m_enemy.size(); ++k)
	{
		m_enemy[k].Shutdown();
	}
	m_enemy.clear();
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

	player.SetPosition(m_startingPlayer);

	for (int k = 0; k < m_enemy.size(); ++k)
	{
		m_enemy[k].SetPosition(m_enemy[k].GetStartPosition());
		m_enemy[k].ResetPathing();
	}
}

/************************************************
*
* returns how many cheese are on the map
*
************************************************/
int LevelGeneration::GetNumberofCheeseLeft(void)
{
	return m_cheese.size();
}
