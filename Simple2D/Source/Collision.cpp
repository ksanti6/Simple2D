#include "Collision.h"
#include <iostream>


namespace Collision
{
	bool AABBCollision(DirectX::SimpleMath::Vector2 _APosition, 
		DirectX::SimpleMath::Vector2 _AWidthHeight, 
		DirectX::SimpleMath::Vector2 _BPosition, 
		DirectX::SimpleMath::Vector2 _BWidthHeight)
	{
		if ((_APosition.x < (_BPosition.x + _BWidthHeight.x)) &&
			((_APosition.x + _AWidthHeight.x) > _BPosition.x) &&
			(_APosition.y < (_BPosition.y + _BWidthHeight.y)) &&
			((_APosition.y + _AWidthHeight.y) > _BPosition.y))
		{
			//its a collision
			printf_s("collision detected\n");
			return true;
		}

		return false;
	}
}