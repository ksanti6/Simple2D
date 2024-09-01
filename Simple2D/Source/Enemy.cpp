#include "Enemy.h"
#include "Graphics.h"
#include "PathingAlgorithm.h"
#include "Player.h"
#include "Grid.h"

void Enemy::Init(void)
{
	m_position = { 400, 300 };
	m_speed = 200.0f;
	m_previousPos = { 400, 300 };
	m_imageSize = { 50, 50 };
}

void Enemy::Update(float _deltaTime)
{
	//enemy pathing ai - astar?
}

void Enemy::Draw(void)
{
	Graphics& graphics = Graphics::GetInstance();
	graphics.Draw(Graphics::Textures::enemy, m_position, m_imageSize);
}

void Enemy::Shutdown(void)
{

}

void Enemy::FollowPlayer(void)
{
	Grid& grid = Grid::GetInstance();
	Player& player = Player::GetInstance();
	PathingAlgorithm::Request request;

	request.isNewRequest = true;
	request.m_target = grid.WorldtoGrid(player.GetPosition());
	request.m_start = grid.WorldtoGrid(m_position);


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
			m_position.x += ((m_imageSize.x + _BWidthHeight.x) / 2.0f) - abs(v.x);
		}
		else // left
		{
			m_position.x -= ((m_imageSize.x + _BWidthHeight.x) / 2.0f) - abs(v.x);
		}
	}
	else
	{
		if (v.y > 0) // top
		{
			m_position.y += ((m_imageSize.y + _BWidthHeight.y) / 2.0f) - abs(v.y);
		}
		else // bottom
		{
			m_position.y -= ((m_imageSize.y + _BWidthHeight.y) / 2.0f) - abs(v.y);
		}
	}
}

DirectX::SimpleMath::Vector2 Enemy::GetSize(void)
{
	return m_imageSize;
}