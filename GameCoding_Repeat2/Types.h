#pragma once
#include <DirectXMath.h>
#include <windows.h>
#include "SimpleMath.h"

using int8 = __int8;
using int16 = __int16;
using int32 = __int32;
using int64 = __int64; // long long

using uint8 = unsigned	__int8;
using uint16 = unsigned __int16;
using uint32 = unsigned __int32;
using uint64 = unsigned __int64;


// DirectX
//using Vec2 = DirectX::XMFLOAT2;
//using Vec3 = DirectX::XMFLOAT3;
//using Vec4 = DirectX::XMFLOAT4;

using Color = DirectX::XMFLOAT4; // RGB

using Vec2		= DirectX::SimpleMath::Vector2;
using Vec3		= DirectX::SimpleMath::Vector3;
using Vec4		= DirectX::SimpleMath::Vector4;
using Matrix	= DirectX::SimpleMath::Matrix;




