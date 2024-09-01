#pragma once
#include<directxtk12/SimpleMath.h>

class Enemy
{
private:
	explicit Enemy() {}
	Enemy(const Enemy&) = delete;
	void operator= (const Enemy&) = delete;

public:
	void Init(void);
	void Update(float _deltaTime);
	void Draw(void);
	void Shutdown(void);


	void FollowPlayer(void);


	static Enemy& GetInstance();

	DirectX::SimpleMath::Vector2 GetPosition(void);
	void SetPosition(DirectX::SimpleMath::Vector2 _position);

	void ResolveWallCollision(DirectX::SimpleMath::Vector2 _BPosition,
		DirectX::SimpleMath::Vector2 _BWidthHeight);

	DirectX::SimpleMath::Vector2 GetSize(void);

private:
	float m_speed = 5.0f;
	DirectX::SimpleMath::Vector2 m_position;
	

	DirectX::SimpleMath::Vector2 m_imageSize;


	DirectX::SimpleMath::Vector2 m_previousPos;
	DirectX::SimpleMath::Vector2 m_direction;
};