#pragma once
#include "directxtk12/SimpleMath.h"

class Cheese
{
public:

	Cheese();
	Cheese(DirectX::SimpleMath::Vector2 _position, int _value);

	void SetPosition(DirectX::SimpleMath::Vector2 _position);
	void SetPointValue(int _value);

	DirectX::SimpleMath::Vector2 GetPosition(void);
	bool GetShouldDestroy(void);

	void ResolvePlayerCollision(void);
private:
	bool m_shouldDestroy = false;
	int m_pointValue = 100;

	DirectX::SimpleMath::Vector2 m_position;
};