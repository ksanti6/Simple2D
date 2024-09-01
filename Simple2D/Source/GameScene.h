#pragma once
#include <directxtk12/SimpleMath.h>

class GameScene
{
private:
	explicit GameScene()
	{
		m_timer = 0.0f;
		m_waitTimer = 0;
	}
	GameScene(const GameScene&) = delete;
	void operator=(const GameScene&) = delete;

public:
	static GameScene& GetInstance(void);

	void Init(void);
	void Update(float _deltaTime);
	void Draw(void);
	void Shutdown(void);

private:
	
	float m_timer;
	int m_waitTimer;

};