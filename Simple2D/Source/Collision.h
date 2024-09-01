#pragma once
#include <directxtk12/SimpleMath.h>

namespace Collision
{
	bool CheckCollision(DirectX::SimpleMath::Vector2 _APosition,
		DirectX::SimpleMath::Vector2 _AWidthHeight,
		DirectX::SimpleMath::Vector2 _BPosition,
		DirectX::SimpleMath::Vector2 _BWidthHeight);
}