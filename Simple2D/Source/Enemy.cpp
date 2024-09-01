#include "Enemy.h"
#include "Graphics.h"

#include "Player.h"
#include "Grid.h"

void Enemy::Init(void)
{
	m_position = { 400, 300 };
	m_speed = 200.0f;
	m_previousPos = { 400, 300 };
	m_imageSize = { 50, 50 };
	m_pathCalculationComplete = true;
	m_takingPathComplete = true;
	m_minDistance = 5.0f;
}

void Enemy::Update(float _deltaTime)
{
	//enemy pathing ai - astar?
	Grid& grid = Grid::GetInstance();

	if (!m_takingPathComplete)
	{
		// go the path

		DirectX::SimpleMath::Vector2 target = grid.GridtoWorld(m_currentRequest.m_finalPath.front());

		if ((abs(m_position.x - target.x) <= m_minDistance) &&
			(abs(m_position.y - target.y) <= m_minDistance))
		{
			m_currentRequest.m_finalPath.erase(m_currentRequest.m_finalPath.begin());

			if (m_currentRequest.m_finalPath.size() == 0)
			{
				m_takingPathComplete = true;
				return;
			}
			
			target = grid.GridtoWorld(m_currentRequest.m_finalPath.front());
		}

		DirectX::SimpleMath::Vector2 direction = target - m_position;

		direction.Normalize();
		m_position += direction * 200.0f * _deltaTime;
	}
	else
	{
		FollowPlayer();
	}
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
	PathingAlgorithm& algorithm = PathingAlgorithm::GetInstance();

	if (m_takingPathComplete && m_pathCalculationComplete)
	{
		m_currentRequest.isNewRequest = true;
		m_currentRequest.m_target = grid.WorldtoGrid(player.GetPosition());
		m_currentRequest.m_start = grid.WorldtoGrid(m_position);
	}

	m_pathCalculationComplete =  algorithm.TheAlgorithm(m_currentRequest);

	if (m_pathCalculationComplete)
	{
		m_takingPathComplete = false;
	}


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