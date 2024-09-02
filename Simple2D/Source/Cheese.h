#pragma once
/**********************************************************************************************************************
*
* Author : Kiara Santiago
* File   : Cheese.h (yum)
* Purpose: holds the functionality of cheese (NOT a singleton, we want multiple of these)
*
**********************************************************************************************************************/
#include "directxtk12/SimpleMath.h" //vector2

class Cheese
{
public:

	Cheese();
	Cheese(DirectX::SimpleMath::Vector2 _position, int _value);

	DirectX::SimpleMath::Vector2 GetPosition(void);
	bool GetShouldDestroy(void);

	void ResolvePlayerCollision(void);
private:
	bool m_shouldDestroy = false;              //has this cheese been consumed? set to destroy
	int m_pointValue = 100;                    //how many points the cheese gives

	DirectX::SimpleMath::Vector2 m_position;   //position the cheese is
};