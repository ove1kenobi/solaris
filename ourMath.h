#pragma once
#include <DirectXMath.h>
#include <cmath>

//Dot product
float dot(DirectX::XMFLOAT3 a, DirectX::XMFLOAT3 b);

//Spherical linear interpolation.
DirectX::XMFLOAT3 slerp(DirectX::XMFLOAT3 p0, DirectX::XMFLOAT3 p1, float t);