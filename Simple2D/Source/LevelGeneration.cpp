#include "LevelGeneration.h"
#include "Graphics.h"
#include <iostream>
#include "Player.h"
#include "Enemy.h"
#include "Collision.h"
#include "Grid.h"

void LevelGeneration::ReadFromFile(void)
{
	FILE* file;
	fopen_s(&file, m_filePath.c_str(), "rt");

	if (!file)
	{
		printf_s("ERROR: LevelGeneration, file could not open.\n");
	}

	//first 2 values = wdith x height of map
	if (file)
	{
		fscanf_s(file, "%i", &m_mapWidth);
		fscanf_s(file, "%i", &m_mapHeight);
	}
	


	DirectX::SimpleMath::Vector2 currentPos = { 25,25 };
	uint32_t currentWidth = 0;
	uint32_t currentHeight = 0;

	Player& player = Player::GetInstance();
	Enemy& enemy = Enemy::GetInstance();
	Grid& grid = Grid::GetInstance();

	grid.SetWidthAndHeight(m_mapWidth, m_mapHeight);


	while (file)
	{
		int value = -1;
		fscanf_s(file, "%i", &value);

		

		if (value == 1)
		{
			m_wallPositions.push_back(currentPos);
			grid.AddNodeByPosition({currentWidth, currentHeight}, false);
		}
		else
		{
			grid.AddNodeByPosition({currentWidth, currentHeight}, true);

			if (value == 2)
			{
				player.SetPosition(currentPos);
			}
			else if (value == 3)
			{
				enemy.SetPosition(currentPos);
			}
			else if (value == 4)
			{
				Cheese temp(currentPos, 100);
				m_cheese.push_back(temp);

			}
		}
		

		///std::cout << currentPos.x << " , " << currentPos.y << "\n";

		++currentWidth;
		currentPos.x += 50;

		if (currentWidth >= m_mapWidth)
		{
			currentWidth = 0;
			++currentHeight;
			currentPos.x = 25;
			currentPos.y += 50;
		}

		if (currentHeight >= m_mapHeight)
		{
			break;
		}

	}


	if (file)
	{
		fclose(file);
	}

	grid.OnGridFinish();
}

LevelGeneration& LevelGeneration::GetInstance(void)
{
	static LevelGeneration level;
	return level;
}

void LevelGeneration::Init(void)
{
	m_filePath = "./Asset/TestLevel.txt";
	m_imageSize = { 50, 50 };

	ReadFromFile();
}

void LevelGeneration::Update(void)
{
	Player& player = Player::GetInstance();
	Enemy& enemy = Enemy::GetInstance();

	bool isColliding = false;

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

	for (int k = 0; k < m_cheese.size(); ++k)
	{
		isColliding = Collision::CheckCollision(player.GetPosition(),
			player.GetSize(), m_cheese[k].GetPosition(), m_imageSize);

		if (isColliding)
		{
			m_cheese[k].ResolvePlayerCollision();
		}
	}



	for (int k = 0; k < m_cheese.size(); ++k)
	{
		if (m_cheese[k].GetShouldDestroy())
		{
			m_cheese.erase(m_cheese.begin()+ k);
		}
	}
}

void LevelGeneration::Draw(void)
{
	Graphics& graphics = Graphics::GetInstance();

	for (int k = 0; k < m_wallPositions.size(); ++k)
	{
		graphics.Draw(Graphics::Textures::wall, m_wallPositions[k], m_imageSize);
	}

	for (int k = 0; k < m_cheese.size(); ++k)
	{
		graphics.Draw(Graphics::Textures::cheese, m_cheese[k].GetPosition(), m_imageSize);
	}
}

void LevelGeneration::Shutdown(void)
{
	m_wallPositions.clear();
}

std::vector<DirectX::SimpleMath::Vector2> LevelGeneration::GetWallPositions(void)
{
	return m_wallPositions;
}

std::vector<Cheese> LevelGeneration::GetCheese(void)
{
	return m_cheese;
}

DirectX::SimpleMath::Vector2 LevelGeneration::GetSize(void)
{
	return m_imageSize;
}
