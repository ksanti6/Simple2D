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
	void Update(float _deltaTime);
	void Draw(void);
	void Shutdown(void);

	void Move(int _key, int _action);

	static Player& GetInstance();

	DirectX::SimpleMath::Vector2 GetPosition(void);
	void SetPosition(DirectX::SimpleMath::Vector2 _position);


	void ResolveWallCollision(DirectX::SimpleMath::Vector2 _BPosition,
		DirectX::SimpleMath::Vector2 _BWidthHeight);

	void ResolveEnemyCollision(void);


private:
	int m_score = 0;
	int m_lives = 3;
	float m_speed = 5.0f;
	DirectX::SimpleMath::Vector2 m_position;
	DirectX::SimpleMath::Vector2 m_scale;

	int m_width = 100;
	int m_height = 100;
	DirectX::SimpleMath::Vector2 m_previousPos;
	DirectX::SimpleMath::Vector2 m_direction;
};