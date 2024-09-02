/**********************************************************************************************************************
*
* Author : Kiara Santiago
* File   : Collision.cpp
* Purpose: do box to box collision check, is a namespace so it is gatekept a little bit (a class seemed overkill)
*          now includes circle to box collision!
*
**********************************************************************************************************************/
#include "Collision.h"

//undefining the min and max macros so i can use std:: min and max 
#ifdef min
#undef min
#endif // min

#ifdef max
#undef max
#endif // max

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

	/************************************************
	*
	* circle to box collision detection,
	* returns either 0,0 or resolution value
	* (so i didnt have to do the calculations twice)
	*
	************************************************/
	DirectX::SimpleMath::Vector2 CheckResolveCircleBoxCollision(DirectX::SimpleMath::Vector2 _CirclePosition, 
		float _CircleRadius, DirectX::SimpleMath::Vector2 _BoxPosition, 
		DirectX::SimpleMath::Vector2 _BoxWidthHeight)
	{
		
		DirectX::SimpleMath::Vector2 resolve = { 0,0 };

		float closerX = std::max(_BoxPosition.x - _BoxWidthHeight.x / 2.0f, std::min(_CirclePosition.x, _BoxPosition.x + _BoxWidthHeight.x / 2.0f));
		float closerY = std::max(_BoxPosition.y - _BoxWidthHeight.y / 2.0f, std::min(_CirclePosition.y, _BoxPosition.y + _BoxWidthHeight.y / 2.0f));

		DirectX::SimpleMath::Vector2 distance = { _CirclePosition.x - closerX, _CirclePosition.y - closerY };

		if (distance.Length() <= _CircleRadius)
		{
			float d = _CircleRadius - distance.Length();

			distance.Normalize();

			distance *= d;

			resolve = distance;
		}

		return resolve;
	}
}