#pragma once
/**********************************************************************************************************************
*
* Author : Kiara Santiago
* File   : Enemy.h
* Purpose: all things the enemy -> behaviors, etc. (NOT A SINGLETON)
*
**********************************************************************************************************************/
#include<directxtk12/SimpleMath.h>  //for vector2
#include "PathingAlgorithm.h"       //for request

class Enemy
{
public:
	Enemy();
	Enemy(float _speed, DirectX::SimpleMath::Vector2 _position);

	void Update(float _deltaTime);
	void Shutdown(void);

	void ResetPathing(void);
	void FollowPlayer(void);

	DirectX::SimpleMath::Vector2 GetPosition(void);
	DirectX::SimpleMath::Vector2 GetStartPosition(void);
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
	DirectX::SimpleMath::Vector2 m_startPosition;//spawn position of the enemy
	
	DirectX::SimpleMath::Vector2 m_imageSize;    //w x h of the enemy
	DirectX::SimpleMath::Vector2 m_direction;    //the direction the enemy moves, based on the path
};