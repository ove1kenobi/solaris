#include "pch.h"
#include "ourMath.h"

float dot(DirectX::XMFLOAT3 a, DirectX::XMFLOAT3 b) {
	//Normalize the vectors.
	float aLen = std::sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
	float bLen = std::sqrt(b.x * b.x + b.y * b.y + b.z * b.z);
	a.x = a.x / aLen;
	a.y = a.y / aLen;
	a.z = a.z / aLen;

	b.x = b.x / bLen;
	b.y = b.y / bLen;
	b.z = b.z / bLen;
	
	return a.x * b.x + a.y * b.y + a.z * b.z;
};

//Here be dragons. Look on wikipedia how this works xD
DirectX::XMFLOAT3 slerp(DirectX::XMFLOAT3 p0, DirectX::XMFLOAT3 p1, float t) {
	float ohm = std::acos(dot(p0, p1));

	float left = (std::sin(std::abs((1 - t) * ohm)) / std::sin(ohm));
	p0.x = p0.x * left;
	p0.y = p0.y * left;
	p0.z = p0.z * left;

	float right = (std::sin(std::abs(t * ohm)) / std::sin(ohm));
	p1.x = p1.x * right;
	p1.y = p1.y * right;
	p1.z = p1.z * right;

	DirectX::XMFLOAT3 result;
	result.x = p0.x + p1.x;
	result.y = p0.y + p1.y;
	result.z = p0.z + p1.z;

	return result;
}