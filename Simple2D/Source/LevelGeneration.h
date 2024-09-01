#pragma once
#include<string>
#include<vector>
#include <directxtk12/SimpleMath.h>
#include "Cheese.h"
#include <map>

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

	std::vector<DirectX::SimpleMath::Vector2> GetWallPositions(void);

	std::vector<Cheese> GetCheese(void);

	DirectX::SimpleMath::Vector2 GetSize(void);

private:

	std::string m_filePath;
	int m_mapWidth;
	int m_mapHeight;

	DirectX::SimpleMath::Vector2 m_imageSize;

	std::vector<DirectX::SimpleMath::Vector2> m_wallPositions;

	std::vector<Cheese> m_cheese;

};