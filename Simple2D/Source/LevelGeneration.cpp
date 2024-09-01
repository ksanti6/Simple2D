#include "LevelGeneration.h"
#include "Graphics.h"
#include <iostream>
#include "Player.h"
#include "Enemy.h"
#include "Collision.h"

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
		fscanf_s(file, "%i", &m_tilesW);
		fscanf_s(file, "%i", &m_tilesH);
	}
	


	DirectX::SimpleMath::Vector2 currentPos = { 50,50 };
	int currentWidth = 1;
	int currentHeight = 1;

	Player& player = Player::GetInstance();
	Enemy& enemy = Enemy::GetInstance();


	while (file)
	{
		int value = -1;
		fscanf_s(file, "%i", &value);


		if (value == 1)
		{
			m_wallPositions.push_back(currentPos);
		}
		else if (value == 2)
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

		///std::cout << currentPos.x << " , " << currentPos.y << "\n";

		++currentWidth;
		currentPos.x += 100;

		if (currentWidth > m_tilesW)
		{
			currentWidth = 1;
			++currentHeight;
			currentPos.x = 50;
			currentPos.y += 100;
		}

		if (currentHeight > m_tilesH)
		{
			break;
		}

	}

	//first pos is 50,50 then adjust by 100

	if (file)
	{
		fclose(file);
	}
}

LevelGeneration& LevelGeneration::GetInstance(void)
{
	static LevelGeneration level;
	return level;
}

void LevelGeneration::Init(void)
{
	m_filePath = "./Asset/TestLevel.txt";
	m_scale = { 1, 1 };
	m_size = { 100, 100 };

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
			player.GetSize(), m_wallPositions[k], m_size);

		if (isColliding)
		{
			player.ResolveWallCollision(m_wallPositions[k], m_size);
		}


		//check for enemy colliding against walls
		isColliding = Collision::CheckCollision(enemy.GetPosition(),
			enemy.GetSize(), m_wallPositions[k], m_size);

		if (isColliding)
		{
			enemy.ResolveWallCollision(m_wallPositions[k],m_size);
		}
	}

	for (int k = 0; k < m_cheese.size(); ++k)
	{
		isColliding = Collision::CheckCollision(player.GetPosition(),
			player.GetSize(), m_cheese[k].GetPosition(), m_size);

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
		graphics.Draw(Graphics::Textures::wall, m_wallPositions[k], m_scale);
	}

	for (int k = 0; k < m_cheese.size(); ++k)
	{
		graphics.Draw(Graphics::Textures::cheese, m_cheese[k].GetPosition(), m_scale);
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
