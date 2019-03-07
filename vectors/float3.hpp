#ifndef FLOAT3
	#define FLOAT3

#include <cmath>
#include <algorithm>

class float3
{
public:
	float x, y, z, w;
	float3()
	{
		x = y = z = 0.0f;
	}
	float3(float a, float b, float c)
	{
		x = a;
		y = b;
		z = c;
	}
};

//------------------------------------------------------------------------

float3 operator - (const float3 &a, const float3 &b)
{
	return float3(a.x - b.x, a.y - b.y, a.z - b.z);
}

float3 operator + (const float3 &a, const float3 &b)
{
	return float3(a.x + b.x, a.y + b.y, a.z + b.z);
}

float3 operator * (const float3 &a, const float &b)
{
	return float3(a.x * b, a.y * b, a.z * b);
}

float3 operator * (const float3 &a, const float3 &b)
{
	return float3(a.x * b.x, a.y * b.y, a.z * b.z);
}

float3 operator / (const float3 &a, const float &b)
{
	return float3(a.x / b, a.y / b, a.z / b);
}

float3 operator / (const float3 &a, const float3 &b)
{
	return float3(a.x / b.x, a.y / b.y, a.z / b.z);
}

//------------------------------------------------------------------------

float length(float3 a)
{
	return std::sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
}

float3 normalize(float3 a)
{
	float l = length(a);
	return float3(a.x / l, a.y / l, a.z / l);
}

float dot(float3 a, float3 b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

float3 cross(float3 a, float3 b)
{
	float3 res;
	res.x = a.y * b.z - a.z * b.y;
	res.y = a.z * b.x - a.x * b.z;
	res.z = a.x * b.y - a.y * b.x;
	return res;
}

float3 abs(float3 x)
{
	return float3(abs(x.x), abs(x.y), abs(x.z));
}

float3 max(float3 a, float3 b)
{
	return float3(std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z));
}

const float3 RIGHT  = float3(1.0, 0.0, 0.0);
const float3 UP 	= float3(0.0, 1.0, 0.0);
const float3 FW 	= float3(0.0, 0.0, 1.0);

#endif
