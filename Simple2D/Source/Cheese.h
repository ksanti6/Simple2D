#pragma once
#include "directxtk12/SimpleMath.h"

class Cheese
{
public:
	void SetPosition(DirectX::SimpleMath::Vector2 _position);
private:
	int m_pointValue = 100;

	DirectX::SimpleMath::Vector2 m_position;
};