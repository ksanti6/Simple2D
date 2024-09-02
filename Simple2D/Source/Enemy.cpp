/**********************************************************************************************************************
*
* Author : Kiara Santiago
* File   : Enemy.cpp
* Purpose: the behaviors,etc of the enemy
*
**********************************************************************************************************************/
#include "Enemy.h"
#include "Player.h"    //for player position
#include "Grid.h"      //for grid conversion functions

/************************************************
*
* default constructor for the enemy
*
************************************************/
Enemy::Enemy()
{
	m_position = { 0, 0 };
	m_startPosition = { 0,0 };
	m_speed = 125.0f;
	m_imageSize = { 50, 50 };
	m_minDistance = 5.0f;
	m_completedNodes = 0;

	m_currentRequest.isNewRequest = true;
	m_currentRequest.m_finalPath.clear();
	m_currentRequest.m_start = { 0,0 };
	m_currentRequest.m_target = { 0,0 };
}

/************************************************
*
* constructor for the enemy that specifies
* speed and position
*
************************************************/
Enemy::Enemy(float _speed, DirectX::SimpleMath::Vector2 _position)
{
	m_position = _position;
	m_startPosition = _position;
	m_speed = _speed;
	m_imageSize = { 50, 50 };
	m_minDistance = 5.0f;
	m_completedNodes = 0;

	m_currentRequest.isNewRequest = true;
	m_currentRequest.m_finalPath.clear();
	m_currentRequest.m_start = { 0,0 };
	m_currentRequest.m_target = { 0,0 };
}

/************************************************
*
* handles the movement for the enemy and getting
* new paths for the enemy to traverse
*
************************************************/
void Enemy::Update(float _deltaTime)
{
	Grid& grid = Grid::GetInstance();
	Player& player = Player::GetInstance();

	//is the path empty? calculate new path
	if (m_currentRequest.m_finalPath.size() == 0)
	{
		FollowPlayer();

		return;
	}

	//have we gone a bit into the path? calculate new path since the player has probably moved by now
	if (m_completedNodes > 3)
	{
		FollowPlayer();
	}

	DirectX::SimpleMath::Vector2 target = grid.GridtoWorld(m_currentRequest.m_finalPath.front());

	//are we at the current target node?
	if ((abs(m_position.x - target.x) <= m_minDistance) &&
		(abs(m_position.y - target.y) <= m_minDistance))
	{
		//yes!
		//take it off the path
		m_currentRequest.m_finalPath.erase(m_currentRequest.m_finalPath.begin());
		++m_completedNodes;

		//get a new target
		if (m_currentRequest.m_finalPath.size() != 0)
		{
			target = grid.GridtoWorld(m_currentRequest.m_finalPath.front());
		}
	}

	//move towards the target
	DirectX::SimpleMath::Vector2 direction = target - m_position;

	direction.Normalize();
	m_position += direction * m_speed * _deltaTime;
}

/************************************************
*
* shutdown - surprisingly empty but here if needed
*
************************************************/
void Enemy::Shutdown(void)
{
}

/************************************************
*
* if we need to reset the pathing, clear out the 
* request
*
************************************************/
void Enemy::ResetPathing(void)
{
	m_currentRequest.isNewRequest = true;
	m_currentRequest.m_finalPath.clear();
	m_currentRequest.m_start = { 0,0 };
	m_currentRequest.m_target = { 0,0 };
}

/************************************************
*
* create a new request and call the A Star 
* algorithm to determine the path
*
************************************************/
void Enemy::FollowPlayer(void)
{
	Grid& grid = Grid::GetInstance();
	Player& player = Player::GetInstance();
	PathingAlgorithm& algorithm = PathingAlgorithm::GetInstance();

	m_currentRequest.isNewRequest = true;
	m_currentRequest.m_target = grid.WorldtoGrid(player.GetPosition());
	m_currentRequest.m_start = grid.WorldtoGrid(m_position);
	m_currentRequest.m_finalPath.clear();
	m_completedNodes = 0;

	algorithm.TheAlgorithm(m_currentRequest);
}

/************************************************
*
* get the enemies current position
*
************************************************/
DirectX::SimpleMath::Vector2 Enemy::GetPosition(void)
{
	return m_position;
}

/************************************************
*
* get the enemies spawn position
*
************************************************/
DirectX::SimpleMath::Vector2 Enemy::GetStartPosition(void)
{
	return m_startPosition;
}

/************************************************
*
* set the enemies position
*
************************************************/
void Enemy::SetPosition(DirectX::SimpleMath::Vector2 _position)
{
	m_position = _position;
}

/************************************************
*
* collision resolution for wall collisions
*
************************************************/
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

/************************************************
*
* get w x h of the enemy
*
************************************************/
DirectX::SimpleMath::Vector2 Enemy::GetSize(void)
{
	return m_imageSize;
}