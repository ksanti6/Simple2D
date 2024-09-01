#include "Collision.h"
#include <iostream>
#include <math.h>


namespace Collision
{
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

		//printf_s("collision\n");
		return true;
	}
}