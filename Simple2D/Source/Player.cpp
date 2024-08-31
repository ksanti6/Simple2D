#include "Player.h"
#include "Graphics.h"
#include <GLFW/glfw3.h>

void Player::Init(void)
{
	m_score = 0;
	m_lives = 3;
	m_position = { 200, 300 };
	m_scale = { 1, 1 };
	m_speed = 30.0f;
}

void Player::Update(void)
{
	Graphics& graphics = Graphics::GetInstance();

	graphics.Draw(Graphics::Textures::player, m_position, m_scale);


}

void Player::Shutdown(void)
{
}

void Player::Move(int _key, int _action)
{
	DirectX::SimpleMath::Vector2 _direction = { 0,0 };

	if (_key == GLFW_KEY_W && (_action == GLFW_PRESS || _action == GLFW_REPEAT))
	{
		_direction.y -= 1;
	}
	if (_key == GLFW_KEY_S && (_action == GLFW_PRESS || _action == GLFW_REPEAT))
	{
		_direction.y += 1;
	}
	if (_key == GLFW_KEY_A && (_action == GLFW_PRESS || _action == GLFW_REPEAT))
	{
		_direction.x -= 1;
	}
	if (_key == GLFW_KEY_D && (_action == GLFW_PRESS || _action == GLFW_REPEAT))
	{
		_direction.x += 1;
	}

	m_position += _direction * m_speed;
}


Player& Player::GetInstance()
{
	static Player player;
	return player;
}
