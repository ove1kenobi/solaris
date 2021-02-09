//Inspired by a library written in GLSL for unity.
//Even though I wrote this code myself I very much just used the code that was already written.
//For this reason I decided to include the copyright and author text from the original file that you can see here below.

// Noise Shader Library for Unity - https://github.com/keijiro/NoiseShader
// Translation and modification by Keijiro Takahashi.

// Description : Array and textureless GLSL 2D/3D/4D simplex noise functions.
// Author : Ian McEwan, Ashima Arts.
// Maintainer : ijm
// License : Copyright (C) 2011 Ashima Arts. All rights reserved.
// Distributed under the MIT License. See LICENSE file.
// https://github.com/ashima/webgl-noise

float3 mod289(float3 x)
{
    return x - floor(x / 289.0) * 289.0;
}

float4 mod289(float4 x)
{
    return x - floor(x / 289.0) * 289.0;
}

float4 permute(float4 x)
{
    return mod289((x * 34.0 + 1.0) * x);
}

float4 taylorInvSqrt(float4 r)
{
    return 1.79284291400159 - r * 0.85373472095314;
}

float snoise(float3 v)
{
    const float2 C = float2(1.0 / 6.0, 1.0 / 3.0);

    // First corner
    float3 i = floor(v + dot(v, C.yyy));
    float3 x0 = v - i + dot(i, C.xxx);

    // Other corners
    float3 g = step(x0.yzx, x0.xyz);
    float3 l = 1.0 - g;
    float3 i1 = min(g.xyz, l.zxy);
    float3 i2 = max(g.xyz, l.zxy);

    // x1 = x0 - i1  + 1.0 * C.xxx;
    // x2 = x0 - i2  + 2.0 * C.xxx;
    // x3 = x0 - 1.0 + 3.0 * C.xxx;
    float3 x1 = x0 - i1 + C.xxx;
    float3 x2 = x0 - i2 + C.yyy;
    float3 x3 = x0 - 0.5;

    // Permutations
    i = mod289(i); // Avoid truncation effects in permutation
    float4 p =
        permute(permute(permute(i.z + float4(0.0, i1.z, i2.z, 1.0))
            + i.y + float4(0.0, i1.y, i2.y, 1.0))
            + i.x + float4(0.0, i1.x, i2.x, 1.0));

    // Gradients: 7x7 points over a square, mapped onto an octahedron.
    // The ring size 17*17 = 289 is close to a multiple of 49 (49*6 = 294)
    float4 j = p - 49.0 * floor(p / 49.0);  // mod(p,7*7)

    float4 x_ = floor(j / 7.0);
    float4 y_ = floor(j - 7.0 * x_);  // mod(j,N)

    float4 x = (x_ * 2.0 + 0.5) / 7.0 - 1.0;
    float4 y = (y_ * 2.0 + 0.5) / 7.0 - 1.0;

    float4 h = 1.0 - abs(x) - abs(y);

    float4 b0 = float4(x.xy, y.xy);
    float4 b1 = float4(x.zw, y.zw);

    //float4 s0 = float4(lessThan(b0, 0.0)) * 2.0 - 1.0;
    //float4 s1 = float4(lessThan(b1, 0.0)) * 2.0 - 1.0;
    float4 s0 = floor(b0) * 2.0 + 1.0;
    float4 s1 = floor(b1) * 2.0 + 1.0;
    float4 sh = -step(h, 0.0);

    float4 a0 = b0.xzyw + s0.xzyw * sh.xxyy;
    float4 a1 = b1.xzyw + s1.xzyw * sh.zzww;

    float3 g0 = float3(a0.xy, h.x);
    float3 g1 = float3(a0.zw, h.y);
    float3 g2 = float3(a1.xy, h.z);
    float3 g3 = float3(a1.zw, h.w);

    // Normalise gradients
    float4 norm = taylorInvSqrt(float4(dot(g0, g0), dot(g1, g1), dot(g2, g2), dot(g3, g3)));
    g0 *= norm.x;
    g1 *= norm.y;
    g2 *= norm.z;
    g3 *= norm.w;

    // Mix final noise value
    float4 m = max(0.6 - float4(dot(x0, x0), dot(x1, x1), dot(x2, x2), dot(x3, x3)), 0.0);
    m = m * m;
    m = m * m;

    float4 px = float4(dot(x0, g0), dot(x1, g1), dot(x2, g2), dot(x3, g3));
    return 42.0 * dot(m, px);
}

float4 snoise_grad(float3 v)
{
    const float2 C = float2(1.0 / 6.0, 1.0 / 3.0);

    // First corner
    float3 i = floor(v + dot(v, C.yyy));
    float3 x0 = v - i + dot(i, C.xxx);

    // Other corners
    float3 g = step(x0.yzx, x0.xyz);
    float3 l = 1.0 - g;
    float3 i1 = min(g.xyz, l.zxy);
    float3 i2 = max(g.xyz, l.zxy);

    // x1 = x0 - i1  + 1.0 * C.xxx;
    // x2 = x0 - i2  + 2.0 * C.xxx;
    // x3 = x0 - 1.0 + 3.0 * C.xxx;
    float3 x1 = x0 - i1 + C.xxx;
    float3 x2 = x0 - i2 + C.yyy;
    float3 x3 = x0 - 0.5;

    // Permutations
    i = mod289(i); // Avoid truncation effects in permutation
    float4 p =
        permute(permute(permute(i.z + float4(0.0, i1.z, i2.z, 1.0))
            + i.y + float4(0.0, i1.y, i2.y, 1.0))
            + i.x + float4(0.0, i1.x, i2.x, 1.0));

    // Gradients: 7x7 points over a square, mapped onto an octahedron.
    // The ring size 17*17 = 289 is close to a multiple of 49 (49*6 = 294)
    float4 j = p - 49.0 * floor(p / 49.0);  // mod(p,7*7)

    float4 x_ = floor(j / 7.0);
    float4 y_ = floor(j - 7.0 * x_);  // mod(j,N)

    float4 x = (x_ * 2.0 + 0.5) / 7.0 - 1.0;
    float4 y = (y_ * 2.0 + 0.5) / 7.0 - 1.0;

    float4 h = 1.0 - abs(x) - abs(y);

    float4 b0 = float4(x.xy, y.xy);
    float4 b1 = float4(x.zw, y.zw);

    //float4 s0 = float4(lessThan(b0, 0.0)) * 2.0 - 1.0;
    //float4 s1 = float4(lessThan(b1, 0.0)) * 2.0 - 1.0;
    float4 s0 = floor(b0) * 2.0 + 1.0;
    float4 s1 = floor(b1) * 2.0 + 1.0;
    float4 sh = -step(h, 0.0);

    float4 a0 = b0.xzyw + s0.xzyw * sh.xxyy;
    float4 a1 = b1.xzyw + s1.xzyw * sh.zzww;

    float3 g0 = float3(a0.xy, h.x);
    float3 g1 = float3(a0.zw, h.y);
    float3 g2 = float3(a1.xy, h.z);
    float3 g3 = float3(a1.zw, h.w);

    // Normalise gradients
    float4 norm = taylorInvSqrt(float4(dot(g0, g0), dot(g1, g1), dot(g2, g2), dot(g3, g3)));
    g0 *= norm.x;
    g1 *= norm.y;
    g2 *= norm.z;
    g3 *= norm.w;

    // Compute noise and gradient at P
    float4 m = max(0.6 - float4(dot(x0, x0), dot(x1, x1), dot(x2, x2), dot(x3, x3)), 0.0);
    float4 m2 = m * m;
    float4 m3 = m2 * m;
    float4 m4 = m2 * m2;
    float3 grad =
        -6.0 * m3.x * x0 * dot(x0, g0) + m4.x * g0 +
        -6.0 * m3.y * x1 * dot(x1, g1) + m4.y * g1 +
        -6.0 * m3.z * x2 * dot(x2, g2) + m4.z * g2 +
        -6.0 * m3.w * x3 * dot(x3, g3) + m4.w * g3;
    float4 px = float4(dot(x0, g0), dot(x1, g1), dot(x2, g2), dot(x3, g3));
    return 42.0 * float4(grad, dot(m4, px));
}

//--------------------------------------------------------------------------------------------
//Other functions related to noise.

float fractalNoise(float3 p) {
	float noiseSum = 0;
	float amplitude = 1;
	float frequency = 1;

	for (int i = 0; i < 5; i++) {
		noiseSum += snoise(p * frequency) * amplitude;
		frequency *= 2;

		amplitude *= 0.5f;
	}

	return noiseSum;
}

float simpleNoise(float3 pos, float4 params[3]) {
    // Extract parameters for readability
    float3 offset = params[0].xyz;
    //float3 offset = float3(0, 0, 0);
    int numLayers = int(params[0].w);
    float persistence = params[1].x;
    float lacunarity = params[1].y;
    float scale = params[1].z;
    float multiplier = params[1].w;
    float verticalShift = params[2].z;

    // Sum up noise layers
    float noiseSum = 0;
    float amplitude = 1;
    float frequency = scale;
    for (int i = 0; i < numLayers; i++) {
        noiseSum += snoise(pos * frequency + offset) * amplitude;
        amplitude *= persistence;
        frequency *= lacunarity;
    }
    return noiseSum * multiplier + verticalShift;
}

float simpleNoise(float3 pos, int numLayers, float scale, float persistence, float lacunarity, float multiplier) {
    float noiseSum = 0;
    float amplitude = 1;
    float frequency = scale;
    for (int i = 0; i < numLayers; i++) {
        noiseSum += snoise(pos * frequency) * amplitude;
        amplitude *= persistence;
        frequency *= lacunarity;
    }
    return noiseSum * multiplier;
}

float simpleNoise(float3 pos, float scale, float multiplier) {
    const int numLayers = 4;
    const float persistence = .5;
    const float lacunarity = 2;

    return simpleNoise(pos, numLayers, scale, persistence, lacunarity, multiplier);
}

float simpleNoise(float3 pos) {
    const float scale = 1;
    const float multiplier = 1;

    return simpleNoise(pos, scale, multiplier);
}

float ridgidNoise(float3 pos, float4 params[3]) {
    // Extract parameters for readability
    float3 offset = params[0].xyz;
    int numLayers = int(params[0].w);
    float persistence = params[1].x;
    float lacunarity = params[1].y;
    float scale = params[1].z;
    float multiplier = params[1].w;
    float power = params[2].x;
    float gain = params[2].y;
    float verticalShift = params[2].z;

    // Sum up noise layers
    float noiseSum = 0;
    float amplitude = 1;
    float frequency = scale;
    float ridgeWeight = 1;

    for (int i = 0; i < numLayers; i++) {
        float noiseVal = 1 - abs(snoise(pos * frequency + offset));
        noiseVal = pow(abs(noiseVal), power);
        noiseVal *= ridgeWeight;
        ridgeWeight = saturate(noiseVal * gain);

        noiseSum += noiseVal * amplitude;
        amplitude *= persistence;
        frequency *= lacunarity;
    }
    return noiseSum * multiplier + verticalShift;
}

// Sample the noise several times at small offsets from the centre and average the result
// This reduces some of the harsh jaggedness that can occur
float smoothedRidgidNoise(float3 pos, float4 params[3]) {
    float3 sphereNormal = normalize(pos);
    float3 axisA = cross(sphereNormal, float3(0, 1, 0));
    float3 axisB = cross(sphereNormal, axisA);

    float offsetDst = params[2].w * 0.01;
    float sample0 = ridgidNoise(pos, params);
    float sample1 = ridgidNoise(pos - axisA * offsetDst, params);
    float sample2 = ridgidNoise(pos + axisA * offsetDst, params);
    float sample3 = ridgidNoise(pos - axisB * offsetDst, params);
    float sample4 = ridgidNoise(pos + axisB * offsetDst, params);
    return (sample0 + sample1 + sample2 + sample3 + sample4) / 5;
}

// Smooth minimum of two values, controlled by smoothing factor k
// When k = 0, this behaves identically to min(a, b)
float smoothMin(float a, float b, float k) {
    k = max(0, k);
    // https://www.iquilezles.org/www/articles/smin/smin.htm
    float h = max(0, min(1, (b - a + k) / (2 * k)));
    return a * h + b * (1 - h) - k * h * (1 - h);
}

// Smooth maximum of two values, controlled by smoothing factor k
// When k = 0, this behaves identically to max(a, b)
float smoothMax(float a, float b, float k) {
    k = min(0, -k);
    float h = max(0, min(1, (b - a + k) / (2 * k)));
    return a * h + b * (1 - h) - k * h * (1 - h);
}

float Blend(float startHeight, float blendDst, float height) {
    return smoothstep(startHeight - blendDst / 2, startHeight + blendDst / 2, height);
}