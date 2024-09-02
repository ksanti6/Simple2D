#pragma once
/**********************************************************************************************************************
*
* Author : Kiara Santiago
* File   : Enemy.h
* Purpose: all things the enemy -> behaviors, etc. (a singleton)
*
**********************************************************************************************************************/
#include<directxtk12/SimpleMath.h>  //for vector2
#include "PathingAlgorithm.h"       //for request

class Enemy
{
private:
	explicit Enemy() 
	{
		m_speed = 0.0f;
		m_minDistance = 0.0f;
		m_completedNodes = 0;
	}
	Enemy(const Enemy&) = delete;
	void operator= (const Enemy&) = delete;

public:
	void Init(void);
	void Update(float _deltaTime);
	void Draw(void);
	void Shutdown(void);

	void ResetPathing(void);
	void FollowPlayer(void);

	static Enemy& GetInstance();

	DirectX::SimpleMath::Vector2 GetPosition(void);
	void SetPosition(DirectX::SimpleMath::Vector2 _position);

	void ResolveWallCollision(DirectX::SimpleMath::Vector2 _BPosition,
		DirectX::SimpleMath::Vector2 _BWidthHeight);

	DirectX::SimpleMath::Vector2 GetSize(void);

private:
	float m_speed;                               //speed the enemy moves at
	float m_minDistance;                         //how close the enemy needs to be the path node for it to be considered reached
	int m_completedNodes;                        //the number of nodes completed in the path
	PathingAlgorithm::Request m_currentRequest;  //the current requesting path for the enemy

	DirectX::SimpleMath::Vector2 m_position;     //current position of the enemy
	
	DirectX::SimpleMath::Vector2 m_imageSize;    //w x h of the enemy
	DirectX::SimpleMath::Vector2 m_direction;    //the direction the enemy moves, based on the path
};