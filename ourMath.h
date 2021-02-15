#pragma once

//Dot product for XMFLOAT3's
float dot(DirectX::XMFLOAT3 a, DirectX::XMFLOAT3 b);

DirectX::XMFLOAT3 cross(DirectX::XMFLOAT3 a, DirectX::XMFLOAT3 b);

//Spherical linear interpolation.
DirectX::XMFLOAT3 slerp(DirectX::XMFLOAT3 p0, DirectX::XMFLOAT3 p1, float t);

DirectX::XMFLOAT3 operator-(const DirectX::XMFLOAT3 a, const DirectX::XMFLOAT3 b);

DirectX::XMFLOAT3 operator+(const DirectX::XMFLOAT3 a, const DirectX::XMFLOAT3 b);

float length(const DirectX::XMFLOAT3 a);