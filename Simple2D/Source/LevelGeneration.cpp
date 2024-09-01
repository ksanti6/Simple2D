#include "LevelGeneration.h"
#include "Graphics.h"
#include <iostream>
#include "Player.h"
#include "Enemy.h"

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
		fscanf_s(file, "%i", &m_width);
		fscanf_s(file, "%i", &m_height);
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

		///std::cout << currentPos.x << " , " << currentPos.y << "\n";

		++currentWidth;
		currentPos.x += 100;

		if (currentWidth > m_width)
		{
			currentWidth = 1;
			++currentHeight;
			currentPos.x = 50;
			currentPos.y += 100;
		}

		if (currentHeight > m_height)
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

	ReadFromFile();
}

void LevelGeneration::DrawLevel(void)
{
	Graphics& graphics = Graphics::GetInstance();

	for (int k = 0; k < m_wallPositions.size(); ++k)
	{
		graphics.Draw(Graphics::Textures::wall, m_wallPositions[k], m_scale);
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
