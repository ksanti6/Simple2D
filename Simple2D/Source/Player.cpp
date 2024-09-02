/**********************************************************************************************************************
*
* Author : Kiara Santiago
* File   : Player.cpp
* Purpose: hold the implementation of the player singleton (behaviors, etc)
*
**********************************************************************************************************************/
#include "Player.h"
#include "Graphics.h"        //for drawing
#include <GLFW/glfw3.h>      //for input


/************************************************
* 
* Initialize the values of the player
* 
************************************************/
void Player::Init(void)
{
	m_score = 0;
	m_lives = 3;
	m_position = { 0, 0 };
	m_speed = 200.0f;
	m_imageSize = { 50, 50 };

	printf_s("Player Lives: %i\n", m_lives);
}

/************************************************
*
* moves player based on recent input
*
************************************************/
void Player::Update(float _deltaTime)
{
	m_position += m_direction * m_speed *_deltaTime;
}

/************************************************
*
* draws the player
*
************************************************/
void Player::Draw(void)
{
	Graphics& graphics = Graphics::GetInstance();

	graphics.Draw(Graphics::Textures::player, m_position, m_imageSize);
}

/************************************************
*
* shutdown function, if theres anything that needs
* to be released it would go here
*
************************************************/
void Player::Shutdown(void)
{
}

/************************************************
*
* interprets keyboard input and calculates the
* players direction to move
*
************************************************/
void Player::Move(int _key, int _action)
{
	//up - w or up arrow key
	if ((_key == GLFW_KEY_W || _key == GLFW_KEY_UP) && 
		_action == GLFW_PRESS)
	{
		m_direction.y -= 1;
	}
	else if ((_key == GLFW_KEY_W || _key == GLFW_KEY_UP) &&
		_action == GLFW_RELEASE)
	{
		m_direction.y += 1;
	}
	
	//down - s or down arrow key
	if ((_key == GLFW_KEY_S || _key == GLFW_KEY_DOWN) &&
		_action == GLFW_PRESS)
	{
		m_direction.y += 1;
	}
	else if ((_key == GLFW_KEY_S || _key == GLFW_KEY_DOWN) &&
		_action == GLFW_RELEASE)
	{
		m_direction.y -= 1;
	}

	//left - a or left arrow key
	if ((_key == GLFW_KEY_A || _key == GLFW_KEY_LEFT) &&
		_action == GLFW_PRESS)
	{
		m_direction.x -= 1;
	}
	else if ((_key == GLFW_KEY_A || _key == GLFW_KEY_LEFT) &&
		_action == GLFW_RELEASE)
	{
		m_direction.x += 1;
	}

	//right - d or right arrow key
	if ((_key == GLFW_KEY_D || _key == GLFW_KEY_RIGHT) &&
		_action == GLFW_PRESS)
	{
		m_direction.x += 1;
	}
	else if ((_key == GLFW_KEY_D || _key == GLFW_KEY_RIGHT) &&
		_action == GLFW_RELEASE)
	{
		m_direction.x -= 1;
	}
		
	//we clamp the magnitude of the direction
	m_direction.Clamp({ -1, -1 }, { 1, 1 });
}

/************************************************
*
* gets an instance of the player singleton
*
************************************************/
Player& Player::GetInstance()
{
	static Player player;
	return player;
}

/************************************************
*
* getter function - players current position
*
************************************************/
DirectX::SimpleMath::Vector2 Player::GetPosition(void)
{
	return m_position;
}

/************************************************
*
* setter function - players position (mostly used
* when loading in the level or reseting the player)
*
************************************************/
void Player::SetPosition(DirectX::SimpleMath::Vector2 _position)
{
	m_position = _position;
}

/************************************************
*
* resolving player collision with a wall tile
* (the player cannot walk through walls)
*
************************************************/
void Player::ResolveWallCollision(DirectX::SimpleMath::Vector2 _BPosition, 
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
* give or take away points from the player
*
************************************************/
void Player::AdjustScore(int _value)
{
	m_score += _value;
	printf_s("Player Score: %i\n", m_score);
}

/************************************************
*
* take away (or give i suppose) lives from the 
* player
*
************************************************/
void Player::AdjustLives(int _value)
{
	m_lives -= _value;
	printf_s("Player Lives: %i\n", m_lives);
}

/************************************************
*
* getter - players current live count
*
************************************************/
int Player::GetLives(void)
{
	return m_lives;
}

/************************************************
*
* getter - space occupied by player
*
************************************************/
DirectX::SimpleMath::Vector2 Player::GetSize(void)
{
	return m_imageSize;
}