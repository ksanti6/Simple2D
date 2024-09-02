#pragma once
/**********************************************************************************************************************
*
* Author : Kiara Santiago
* File   : GameScene.h
* Purpose: where all the game code lives! a singleton
*
**********************************************************************************************************************/

class GameScene
{
private:
	explicit GameScene() { }
	GameScene(const GameScene&) = delete;
	void operator=(const GameScene&) = delete;

public:
	static GameScene& GetInstance(void);

	void Init(int _level);
	void Update(float _deltaTime);
	void Draw(void);
	void Shutdown(void);

	void Reset(int _level);
};