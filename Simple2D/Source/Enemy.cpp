#include "Enemy.h"
#include "Graphics.h"

void Enemy::Init(void)
{
	m_position = { 400, 300 };
	m_scale = { 1, 1 };
	m_speed = 200.0f;
	m_previousPos = { 400, 300 };
	m_size = { 100, 100 };
}

void Enemy::Update(float _deltaTime)
{
	//enemy pathing ai - astar?
}

void Enemy::Draw(void)
{
	Graphics& graphics = Graphics::GetInstance();
	graphics.Draw(Graphics::Textures::enemy, m_position, { 1.0f, 1.0f });
}

void Enemy::Shutdown(void)
{

}


Enemy& Enemy::GetInstance()
{
	static Enemy enemy;
	return enemy;
}

DirectX::SimpleMath::Vector2 Enemy::GetPosition(void)
{
	return m_position;
}

void Enemy::SetPosition(DirectX::SimpleMath::Vector2 _position)
{
	m_position = _position;
	m_previousPos = _position;
}


//tbd
void Enemy::ResolveWallCollision(DirectX::SimpleMath::Vector2 _BPosition,
	DirectX::SimpleMath::Vector2 _BWidthHeight)
{
	DirectX::SimpleMath::Vector2 v = m_position - _BPosition;
	if (abs(v.x) > abs(v.y))
	{
		if (v.x > 0) // right
		{
			m_position.x += ((m_size.x + _BWidthHeight.x) / 2.0f) - abs(v.x);
		}
		else // left
		{
			m_position.x -= ((m_size.x + _BWidthHeight.x) / 2.0f) - abs(v.x);
		}
	}
	else
	{
		if (v.y > 0) // top
		{
			m_position.y += ((m_size.y + _BWidthHeight.y) / 2.0f) - abs(v.y);
		}
		else // bottom
		{
			m_position.y -= ((m_size.y + _BWidthHeight.y) / 2.0f) - abs(v.y);
		}
	}
}

DirectX::SimpleMath::Vector2 Enemy::GetSize(void)
{
	return m_size;
}