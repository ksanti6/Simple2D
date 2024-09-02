#pragma once
/**********************************************************************************************************************
*
* Author : Kiara Santiago
* File   : Collision.h
* Purpose: do box to box collision check, is a namespace so it is gatekept a little bit (a class seemed overkill)
*
**********************************************************************************************************************/
#include <directxtk12/SimpleMath.h>  //for vector2

namespace Collision
{
	bool CheckCollision(DirectX::SimpleMath::Vector2 _APosition,
		DirectX::SimpleMath::Vector2 _AWidthHeight,
		DirectX::SimpleMath::Vector2 _BPosition,
		DirectX::SimpleMath::Vector2 _BWidthHeight);


	DirectX::SimpleMath::Vector2 CheckResolveCircleBoxCollision(DirectX::SimpleMath::Vector2 _CirclePosition,
		float _CircleRadius,
		DirectX::SimpleMath::Vector2 _BoxPosition,
		DirectX::SimpleMath::Vector2 _BoxWidthHeight);
}