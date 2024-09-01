#pragma once
#include<string>
#include<vector>
#include <directxtk12/SimpleMath.h>
#include "Cheese.h"

class LevelGeneration
{
private:
	explicit LevelGeneration() {}
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

private:

	std::string m_filePath;
	int m_tilesW;
	int m_tilesH;

	DirectX::SimpleMath::Vector2 m_size;
	DirectX::SimpleMath::Vector2 m_scale;

	std::vector<DirectX::SimpleMath::Vector2> m_wallPositions;

	std::vector<Cheese> m_cheese;

};