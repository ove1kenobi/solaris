//Returns the distance to the sphere and the distance it travels through the sphere.
//ALWAYS NORMALIZE DIR BEFORE THIS FUNCTION!
//If inside sphere distance is 0.
//If it misses: distance = -1.

float2 raySphereIntersect(float3 planetCenter, float radius, float4 origin, float3 direction) {
	float3 offset = origin.xyz - planetCenter;

	float a = 2.0f * dot(offset, direction);
	float b = dot(offset, offset) - radius * radius;

	float discriminant = a * a - 4.0f * b;
	if (discriminant > 0.0f) {
		float s = sqrt(discriminant);
		float dstToSphereNear = max(0.0f, (-a - s) / 2.0f);
		float dstToSphereFar = (((0.0f - a) + s) / 2.0f);

		if (dstToSphereFar >= 0.0f) {
			return float2(dstToSphereNear, dstToSphereFar - dstToSphereNear);
		}
	}
	//Did not intersect.
	return float2(-1.0f, 0.0f);
	
}

/*
float2 raySphereIntersect(float3 s0, float sr, float4 r0, float3 rd) {
	float a = dot(rd, rd);
	float3 s0_r0 = r0.xyz - s0;
	float b = 2.0 * dot(rd, s0_r0);
	float c = dot(s0_r0, s0_r0) - (sr * sr);
	float disc = b * b - 4.0 * a * c;
	if (disc < 0.0) {
		return float2(-1, 0.0);
	}
	else {
		float dstToNear = (-b - sqrt(disc)) / (2.0 * a);
		float dstToFar = (-b + sqrt(disc)) / (2.0 * a);

		return float2(dstToNear, dstToFar - dstToNear);
	}
}
*/