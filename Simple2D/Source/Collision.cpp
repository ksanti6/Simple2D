/**********************************************************************************************************************
*
* Author : Kiara Santiago
* File   : Collision.cpp
* Purpose: do box to box collision check, is a namespace so it is gatekept a little bit (a class seemed overkill)
*
**********************************************************************************************************************/
#include "Collision.h"


namespace Collision
{
	/************************************************
    *
    * box to box collision check between box A and
	* box B
	* return true if collision, false otherwise
    *
    ************************************************/
	bool CheckCollision(DirectX::SimpleMath::Vector2 _APosition, 
		DirectX::SimpleMath::Vector2 _AWidthHeight, 
		DirectX::SimpleMath::Vector2 _BPosition, 
		DirectX::SimpleMath::Vector2 _BWidthHeight)
	{
		if (abs(_APosition.x - _BPosition.x) >= ((_AWidthHeight.x + _BWidthHeight.x) / 2.0f))
		{
			return false;
		}
		if (abs(_APosition.y - _BPosition.y) >= ((_AWidthHeight.y + _BWidthHeight.y) / 2.0f))
		{
			return false;
		}

		return true;
	}
}