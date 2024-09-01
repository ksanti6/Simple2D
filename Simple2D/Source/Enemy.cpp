#include "Enemy.h"
#include "Graphics.h"

void Enemy::Init(void)
{
	m_position = { 400, 300 };
	m_scale = { 1, 1 };
	m_speed = 200.0f;
	m_previousPos = { 400, 300 };
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

}