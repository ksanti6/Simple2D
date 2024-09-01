#include "Player.h"
#include "Graphics.h"
#include <GLFW/glfw3.h>
#include <math.h>

void Player::Init(void)
{
	m_score = 0;
	m_lives = 3;
	m_position = { 200, 300 };
	m_scale = { 1, 1 };
	m_speed = 200.0f;
	m_previousPos = { 200, 300 };
	m_size = { 100, 100 };

	printf_s("Player Lives: %i\n", m_lives);
}

void Player::Update(float _deltaTime)
{
	
	m_previousPos = m_position;
	m_position += m_direction * m_speed *_deltaTime;



}

void Player::Draw(void)
{
	Graphics& graphics = Graphics::GetInstance();

	graphics.Draw(Graphics::Textures::player, m_position, m_scale);
}

void Player::Shutdown(void)
{
}

void Player::Move(int _key, int _action)
{
	//up
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
	
	//down
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

	//left
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

	//right
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
		

	//m_direction.Normalize();
	m_direction.Clamp({ -1, -1 }, { 1, 1 });
}


Player& Player::GetInstance()
{
	static Player player;
	return player;
}

DirectX::SimpleMath::Vector2 Player::GetPosition(void)
{
	return m_position;
}

void Player::SetPosition(DirectX::SimpleMath::Vector2 _position)
{
	m_position = _position;
	m_previousPos = _position;
}


void Player::ResolveWallCollision(DirectX::SimpleMath::Vector2 _BPosition, 
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

void Player::ResolveEnemyCollision(void)
{
	--m_lives;
	printf_s("Player Lives: %i\n", m_lives);
	
	if (m_lives == 0)
	{
		//gaem over
	}
	else
	{
		//reset positions of player and enemy
	}

	

}

void Player::AdjustScore(int _value)
{
	m_score += _value;
	printf_s("Player Score: %i\n", m_score);
}

DirectX::SimpleMath::Vector2 Player::GetSize(void)
{
	return m_size;
}


