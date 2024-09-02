/**********************************************************************************************************************
*
* Author : Kiara Santiago
* File   : Cheese.cpp
* Purpose: behavior of the cheese (give points)
*
**********************************************************************************************************************/
#include "Cheese.h"
#include "Player.h"  //who we give points to

/************************************************
*
* default constructor
*
************************************************/
Cheese::Cheese()
{
	m_position = { 0,0 };
	m_pointValue = 100;
}

/************************************************
*
* constructor, sets point value and position
*
************************************************/
Cheese::Cheese(DirectX::SimpleMath::Vector2 _position, int _value)
{
	m_position = _position;
	m_pointValue = _value;
}

/************************************************
*
* returns the current position of the cheese
*
************************************************/
DirectX::SimpleMath::Vector2 Cheese::GetPosition(void)
{
	return m_position;
}

/************************************************
*
* returns if this cheese should be destroyed
*
************************************************/
bool Cheese::GetShouldDestroy(void)
{
	return m_shouldDestroy;
}

/************************************************
*
* if colliding with the player, be consumed
* and give points
*
************************************************/
void Cheese::ResolvePlayerCollision(void)
{
	Player& player = Player::GetInstance();
	player.AdjustScore(m_pointValue);

	m_shouldDestroy = true;
}