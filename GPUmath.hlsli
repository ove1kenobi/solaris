//Returns the distance to the sphere and the distance it travels through the sphere.
//ALWAYS NORMALIZE DIR BEFORE THIS FUNCTION!
//If inside sphere distance is 0.
//If it misses: distance = -1.

float2 raySphereIntersect(float3 planetCenter, float radius, float4 origin, float3 direction) {
	float2 intersect = float2(-1.0f, -1.0f);

	float3 offset = origin.xyz - planetCenter;

	float a = 2.0f * dot(offset, direction);
	float b = dot(offset, offset) - radius * radius;

	float discriminant = a * a - 4.0f * b;
	if (discriminant > 0.0f) {
		float s = sqrt(discriminant);
		float dstToSphereNear = max(0.0f, (-a - s) / 2.0f);
		float dstToSphereFar = (((0.0f - a) + s) / 2.0f);

		if (dstToSphereFar >= 0.0f) {
			intersect = float2(dstToSphereNear, dstToSphereFar - dstToSphereNear);
		}
	}
	// returns -1, -1 if it does not intersect.
	return intersect;
}