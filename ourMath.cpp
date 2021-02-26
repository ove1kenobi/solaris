#include "pch.h"
#include "ourMath.h"

DirectX::XMFLOAT3 normalize(DirectX::XMFLOAT3 vector) {
	float length = sqrtf(powf(vector.x, 2) + powf(vector.y, 2) + powf(vector.z, 2));

	if (length != 0.0f) {
		vector.x /= length;
		vector.y /= length;
		vector.z /= length;
	}

	return vector;
}

float dot(DirectX::XMFLOAT3 a, DirectX::XMFLOAT3 b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
};

DirectX::XMFLOAT3 cross(DirectX::XMFLOAT3 a, DirectX::XMFLOAT3 b) {
	DirectX::XMFLOAT3 result = { 0, 0, 0 };

	result.x = a.y * b.z - a.z * b.y;
	result.y = a.z * b.x - a.x * b.z;
	result.z = a.x * b.y - a.y * b.x;

	return result;
}

//Here be dragons. Look on wikipedia how this works xD
DirectX::XMFLOAT3 slerp(DirectX::XMFLOAT3 p0, DirectX::XMFLOAT3 p1, float t) {
	float ohm = std::acos( dot(normalize(p0), normalize(p1)) );

	float left = (std::sin(std::abs((1 - t) * ohm)) / std::sin(ohm));
	p0.x = p0.x * left;
	p0.y = p0.y * left;
	p0.z = p0.z * left;

	float right = (std::sin(std::abs(t * ohm)) / std::sin(ohm));
	p1.x = p1.x * right;
	p1.y = p1.y * right;
	p1.z = p1.z * right;

	DirectX::XMFLOAT3 result = {};
	result.x = p0.x + p1.x;
	result.y = p0.y + p1.y;
	result.z = p0.z + p1.z;

	return result;
}

DirectX::XMFLOAT3 operator-(const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b)
{
	return { a.x - b.x, a.y - b.y, a.z - b.z };
}

DirectX::XMFLOAT3 operator+(const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b)
{
	return { a.x + b.x, a.y + b.y, a.z + b.z };
}

DirectX::XMFLOAT3 operator*(const DirectX::XMFLOAT3& a, const float b)
{
	return { a.x * b, a.y * b, a.z * b };
}

DirectX::XMFLOAT3 operator*(const float b, const DirectX::XMFLOAT3& a)
{
	return { a.x * b, a.y * b, a.z * b };
}

DirectX::XMFLOAT3 operator/(const DirectX::XMFLOAT3& a, const float b)
{
	return { a.x / b, a.y / b, a.z / b };
}

DirectX::XMFLOAT3 operator/(const float b, const DirectX::XMFLOAT3& a)
{
	return { a.x / b, a.y / b, a.z / b };
}

float length(const DirectX::XMFLOAT3& a)
{
	return sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
}