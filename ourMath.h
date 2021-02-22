#pragma once

// Returns the lenght of a vector
float length(DirectX::XMFLOAT3 vector);

// Normalization for XMFLOAT3's
DirectX::XMFLOAT3 normalize(DirectX::XMFLOAT3 vector);

//Dot product for XMFLOAT3's
float dot(DirectX::XMFLOAT3 a, DirectX::XMFLOAT3 b);

//Cross product for XMFLOAT3's
DirectX::XMFLOAT3 cross(DirectX::XMFLOAT3 a, DirectX::XMFLOAT3 b);

//Spherical linear interpolation.
DirectX::XMFLOAT3 slerp(DirectX::XMFLOAT3 p0, DirectX::XMFLOAT3 p1, float t);