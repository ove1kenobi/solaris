//Returns the distance to the sphere and the distance it travels through the sphere.
//ALWAYS NORMALIZE DIR BEFORE THIS FUNCTION!
//If inside sphere distance is 0.
//If it misses: distance = -1.
float2 raySphereIntersect(float3 center, float radius, float3 origin, float3 direction) {
	float3 offset = origin - center;

	float a = 2 * dot(offset, direction);
	float b = dot(offset, offset) - radius * radius;

	float discriminant = a * a - 4 * b;
	if (discriminant) {
		float s = sqrt(discriminant);
		float dstToSphereNear = max(0, (-a - s) / 2);
		float dstToSphereFar = (-a + s) / 2;

		if (dstToSphereFar >= 0) {
			return float2(dstToSphereNear, dstToSphereFar - dstToSphereNear);
		}
	}
	//Did not intersect.
	return float2(-1, 0);
}