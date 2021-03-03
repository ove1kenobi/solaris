#pragma once

// Normalization for XMFLOAT3's
DirectX::XMFLOAT3 normalize(DirectX::XMFLOAT3 vector);

//Dot product for XMFLOAT3's
float dot(DirectX::XMFLOAT3 a, DirectX::XMFLOAT3 b);

//Cross product for XMFLOAT3's
DirectX::XMFLOAT3 cross(DirectX::XMFLOAT3 a, DirectX::XMFLOAT3 b);

//Linear interpolation,
float lerp(float a, float b, float t);

//Spherical linear interpolation.
DirectX::XMFLOAT3 slerp(DirectX::XMFLOAT3 p0, DirectX::XMFLOAT3 p1, float t);

DirectX::XMFLOAT3 operator-(const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b);

DirectX::XMFLOAT3 operator+(const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b);

DirectX::XMFLOAT3 operator*(const DirectX::XMFLOAT3& a, const float b);
DirectX::XMFLOAT3 operator*(const float b, const DirectX::XMFLOAT3& a);
DirectX::XMFLOAT3 operator/(const DirectX::XMFLOAT3& a, const float b);
DirectX::XMFLOAT3 operator/(const float b, const DirectX::XMFLOAT3& a);

// Returns the lenght of a XMFLOAT3 vector
float length(const DirectX::XMFLOAT3& a);

float distance(const DirectX::XMFLOAT3 a, const DirectX::XMFLOAT3 b);