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

	void Reset(void);

private:
	
	float m_timer;   //timer for win con
	int m_waitTimer; //timer for printing out timer

};