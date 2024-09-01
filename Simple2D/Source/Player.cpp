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
}

void Player::Update(float _deltaTime)
{
	
	m_previousPos = m_position;
	m_position += m_direction * m_speed *_deltaTime;



}

void Player::Draw(void)
{
	Graphics& graphics = Graphics::GetInstance();

	graphics.Draw(Graphics::Textures::wall, m_position, m_scale);
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
	m_direction.Clamp({ -1, -1 }, { 1, 1 });;;;;;;;;;
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
	//undo movement

	int distance = 0;

	distance = abs(m_position.x - _BPosition.x);

	DirectX::SimpleMath::Vector2 velocity = m_position - m_previousPos;
	DirectX::SimpleMath::Vector2 velNormal = velocity;
	velNormal.Normalize();

	float xOverlap = ((m_width + _BWidthHeight.x) / 2.0f) - distance;
	if (xOverlap > 0)
	{
		
		//velocity.x = _BPosition.x - (m_position.x + m_width);
		velocity.x -= xOverlap * velNormal.x;


		//if (m_position.x > _BPosition.x)
		//{
		//	m_position.x += (((m_width + _BWidthHeight.x) / 2.0f) - distance);
		//}
		//else
		//{
		//	m_posit ion.x -= (((m_width + _BWidthHeight.x) / 2.0f) - distance);
		//}
	}
	
	distance = abs(m_position.y - _BPosition.y);
	float yOverlap = ((m_height + _BWidthHeight.y) / 2.0f) - distance;
	if (yOverlap > 0)
	{
		velocity.y -= yOverlap * velNormal.y;
	}

	m_position = m_previousPos + velocity;
}

void Player::ResolveEnemyCollision(void)
{
	--m_lives;
	
	if (m_lives == 0)
	{
		//gaem over
	}
	else
	{
		//reset positions of player and enemy
	}

	

}
