#include "Cheese.h"
#include "Player.h"

Cheese::Cheese()
{
	m_position = { 0,0 };
	m_pointValue = 100;
}

Cheese::Cheese(DirectX::SimpleMath::Vector2 _position, int _value)
{
	m_position = _position;
	m_pointValue = _value;
}


void Cheese::SetPosition(DirectX::SimpleMath::Vector2 _position)
{
	m_position = _position;
}

void Cheese::SetPointValue(int _value)
{
	m_pointValue = _value;
}

DirectX::SimpleMath::Vector2 Cheese::GetPosition(void)
{
	return m_position;
}

bool Cheese::GetShouldDestroy(void)
{
	return m_shouldDestroy;
}

void Cheese::ResolvePlayerCollision(void)
{
	Player& player = Player::GetInstance();
	player.AdjustScore(m_pointValue);

	m_shouldDestroy = true;
}
