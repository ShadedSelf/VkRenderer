// uint hash( uint x )
// {
//     x += (x << 10u);
//     x ^= (x >>  6u);
//     x += (x <<  3u);
//     x ^= (x >> 11u);
//     x += (x << 15u);
//     return x;
// }

// uint hash(uvec2 v) { return hash( v.x ^ hash(v.y)                         ); }
// uint hash(uvec3 v) { return hash( v.x ^ hash(v.y) ^ hash(v.z)             ); }
// uint hash(uvec4 v) { return hash( v.x ^ hash(v.y) ^ hash(v.z) ^ hash(v.w) ); }

// float floatConstruct(uint m)
// {
//     const uint ieeeMantissa = 0x007FFFFFu;
//     const uint ieeeOne      = 0x3F800000u;

//     m &= ieeeMantissa;
//     m |= ieeeOne;

//     float  f = uintBitsToFloat(m);
//     return f - 1.0;
// }

// float random(float x) { return floatConstruct(hash(floatBitsToUint(x))); }
// float random(vec2  v) { return floatConstruct(hash(floatBitsToUint(v))); }
// float random(vec3  v) { return floatConstruct(hash(floatBitsToUint(v))); }
// float random(vec4  v) { return floatConstruct(hash(floatBitsToUint(v))); }



vec3 randomSphereDirection()
{
    vec2 h = vec2(randMe(), randMe()) * vec2(2.0, 6.28318530718) - vec2(1.0, 0.0);
    float phi = h.y;
	return vec3(sqrt(1.0 - h.x*h.x) * vec2(sin(phi), cos(phi)), h.x);
}

vec3 randomHemisphereDirection(vec3 n)
{
	vec3 dr = randomSphereDirection();
	return dot(dr,n) * dr;
}

vec3 ortho(vec3 v)
{
	return abs(v.x) > abs(v.z) ? vec3(-v.y, v.x, 0.0) : vec3(0.0, -v.z, v.y);
}

vec3 getConeSample(vec3 dir, float extent)
{
	dir = normalize(dir);
	vec3 o1 = normalize(ortho(dir));
	vec3 o2 = normalize(cross(dir, o1));
	vec2 r = vec2(randMe(), randMe());
	r.x = r.x * 2.0 * PI;
	r.y = 1.0 - r.y * extent;
	float oneminus = sqrt(1.0 - r.y*r.y);
	return cos(r.x) * oneminus * o1 + sin(r.x) * oneminus * o2 + r.y * dir;
}

// vec3 coneDirection()
// {
// 	float u = randMe();
// 	float v = randMe();

// 	vec3 uu = normalize(cross((abs(nor.z) < 0.999 ? FW : RIGHT), nor));
// 	vec3 vv = cross(nor, uu);
// }

vec3 cosineDirection(vec3 nor)
{
	float u = randMe();
	float v = randMe();

	vec3 uu = normalize(ortho(nor));
	vec3 vv = cross(nor, uu);

	float c = cos(2.0 * PI * v) * sqrt(u);
	float s = sin(2.0 * PI * v) * sqrt(u);

	return normalize(uu * c + vv * s + nor * sqrt(1.0 - u));
}

// vec3 cosineDirection(in vec3 nor)
// {
// 	float u = randMe();
// 	float v = randMe();

// 	float a = 6.2831853 * v;
// 	u = 2.0 * u - 1.0;
// 	return normalize(nor + vec3(sqrt(1.0 - u * u) * vec2(cos(a), sin(a)), u));
// }

vec2 randomTrianglePoint(vec2 a, vec2 b, vec2 c, vec2 rand)
{
	rand = (rand.x + rand.y > 1.0) ? 1.0 - rand : rand;

	float aT = 1.0 - rand.x - rand.y;
	float bT = rand.x;
	float cT = rand.y;

	return a*aT + b*bT + c*cT;
}

vec2 get(uint seed)
{
	const vec2 a = vec2( 0.0,    1.0);
    const vec2 b = vec2( 0.866, -0.5);
    const vec2 c = vec2(-0.866, -0.5);

	vec2 randS = vec2(randMe(), randMe());
	return randomTrianglePoint(a, b, c, randS);
}

float ff(float n1, float n2, vec3 normal, vec3 incident, float reflectance)
{
	float r0 = (n1-n2) / (n1+n2);
	r0 *= r0;
	float cosX = -dot(normal, incident);
	if (n1 > n2)
	{
		float n = n1/n2;
		float sinT2 = n*n*(1.0-cosX*cosX);
		if (sinT2 > 1.0)
			return 1.0;
		cosX = sqrt(1.0-sinT2);
	}
	float x = 1.0-cosX;
	float ret = r0+(1.0-r0)*x*x*x*x*x;
	ret = (reflectance + (1.0-reflectance) * ret);
	return ret;
}
