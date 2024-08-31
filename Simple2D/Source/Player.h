#pragma once
#include<directxtk12/SimpleMath.h>

class Player
{
private:
	explicit Player() {}
	Player(const Player&) = delete;
	void operator= (const Player&) = delete;

public:
	void Init(void);
	void Update(void);
	void Shutdown(void);

	void Move(int _key, int _action);

	static Player& GetInstance();

private:
	int m_score = 0;
	int m_lives = 3;
	float m_speed = 5.0f;
	DirectX::SimpleMath::Vector2 m_position;
	DirectX::SimpleMath::Vector2 m_scale;

};