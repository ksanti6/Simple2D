#pragma once
/**********************************************************************************************************************
* 
* Author : Kiara Santiago
* File   : Player.h
* Purpose: Holds the singleton Player
* 
**********************************************************************************************************************/
#include<directxtk12/SimpleMath.h>  //for Vector2

class Player
{
private:
	explicit Player() 
	{
		m_speed = 0.0f;
		m_score = 0;
		m_lives = 0;
	}
	Player(const Player&) = delete;
	void operator= (const Player&) = delete;

public:
	void Init(void);
	void Update(float _deltaTime);
	void Draw(void);
	void Shutdown(void);

	void Move(int _key, int _action);

	static Player& GetInstance();

	DirectX::SimpleMath::Vector2 GetPosition(void);
	void SetPosition(DirectX::SimpleMath::Vector2 _position);


	void ResolveWallCollision(DirectX::SimpleMath::Vector2 _BPosition,
		DirectX::SimpleMath::Vector2 _BWidthHeight);

	void AdjustScore(int _value);

	void AdjustLives(int _value);

	int GetLives(void);

	DirectX::SimpleMath::Vector2 GetSize(void);


private:
	int m_score;                                     //holds the points the player gets
	int m_lives;                                     //how many lives the player will have
	float m_speed;                                   //how fast the player will move
	DirectX::SimpleMath::Vector2 m_position;         //current position of the player

	DirectX::SimpleMath::Vector2 m_imageSize;        //width and height of the player image
	DirectX::SimpleMath::Vector2 m_previousPos;      //position of player in the last frame
	DirectX::SimpleMath::Vector2 m_direction;        //direction data from input
};