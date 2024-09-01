#pragma once
#include <directxtk12/SimpleMath.h>

class GameScene
{
private:
	explicit GameScene() {}
	GameScene(const GameScene&) = delete;
	void operator=(const GameScene&) = delete;

public:
	static GameScene& GetInstance(void);

	void Init(void);
	void Update(float _deltaTime);
	void Draw(void);
	void Shutdown(void);

private:
	
};