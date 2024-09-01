#pragma once
#include<string>
#include<vector>
#include <directxtk12/SimpleMath.h>

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

	void DrawLevel(void);

	void Shutdown(void);

	std::vector<DirectX::SimpleMath::Vector2> GetWallPositions(void);

private:

	std::string m_filePath;
	int m_width;
	int m_height;
	DirectX::SimpleMath::Vector2 m_scale;

	std::vector<DirectX::SimpleMath::Vector2> m_wallPositions;

};