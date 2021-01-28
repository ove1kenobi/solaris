#include "Planet.h"

Planet::Planet() {

}

Planet::~Planet() {

}

bool Planet::init(float x, float y, float z, float r) {
	this->m_radius = r;
	this->m_center.x = x;
	this->m_center.y = y;
	this->m_center.z = z;

	if (!createSphere()) {
		//Throw
		return -1;
	}
}

bool Planet::createSphere() {
	//Starting octahedron points.
	DirectX::XMFLOAT3 p0, p1, p2, p3, p4, p5, p6;
	//So that the triangles have the same length on all 3 sides.
	float x = this->m_radius + this->m_center.x;
	float z = this->m_radius + this->m_center.z;
	float yOffset = ((this->m_radius * this->m_radius) * 2) + this->m_center.y;
	//Points of the octahedron
	p0 = {  0,  sqrt(yOffset)	,  0 };	//Top
	p1 = { -x,  0				, -z };	//Front left
	p2 = { -x,  0				,  z }; //Back left
	p3 = {  x,  0				,  z }; //Back right
	p4 = {  x,  0				, -z }; //Front right
	p5 = {  0, -sqrt(yOffset)	,  0 }; //Bottom

	std::vector<DirectX::XMFLOAT3> vertices = { p0, p1, p2, p3, p4, p5, p6 };

	//vector representing the lines of the octahedron.
	std::vector<int> lines = { };

	//GÖR OM
	//Represt the triangles with the lines. (Indices of the last vector)
	std::vector<int> triangles = {
		//Top Triangles
		0, 1, 2,
		0, 2, 3,
		0, 3, 4,
		0, 4, 1,
		//Bottom Triangles
		5, 4, 3,
		5, 3, 2,
		5, 2, 1,
		5, 1, 4
	};
}															