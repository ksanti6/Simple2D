#include "Collision.h"
#include <iostream>
#include <math.h>


namespace Collision
{
	bool AABBCollision(DirectX::SimpleMath::Vector2 _APosition, 
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
			//its a collision
			return false;
		}

		printf_s("collision\n");
		return true;
	}

	//((_APosition.x < (_BPosition.x + (_BWidthHeight.x / 2.0f))) &&
	//	((_APosition.x + (_AWidthHeight.x / 2.0f)) > _BPosition.x) &&
	//	(_APosition.y < (_BPosition.y + (_BWidthHeight.y / 2.0f))) &&
	//	((_APosition.y + (_AWidthHeight.y / 2.0f)) > _BPosition.y))
}