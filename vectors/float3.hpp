#ifndef FLOAT3
	#define FLOAT3

#include <cmath>
#include <algorithm>
#include <iostream>

struct float3_raw
{
	float x, y, z, pad;
};

class float3
{
	private:
		float pad;
		
	public:
		float x, y, z;
		float3()
		{
			x = y = z = 0.0;
		}
		float3(float a)
		{
			x = y = z = a;
		}
		float3(float a, float b, float c)
		{
			x = a;
			y = b;
			z = c;
		}
		float3_raw getRaw()
		{
			float3_raw res;
			res.x = x;
			res.y = y;
			res.z = z;
			res.pad = 1;
			return res;
		}
		friend float3& operator += (float3 &a, const float3 &b);
};

//------------------------------------------------------------------------

float3 operator - (const float3 &a, const float3 &b)
{
	return float3(a.x - b.x, a.y - b.y, a.z - b.z);
}

float3 operator - (const float3 &a)
{
	return float3(-a.x, -a.y, -a.z);
}

float3 operator + (const float3 &a, const float3 &b)
{
	return float3(a.x + b.x, a.y + b.y, a.z + b.z);
}

float3& operator += (float3 &a, const float3 &b)
{
	a.x += b.x;
	a.y += b.y;
	a.z += b.z;
	return a;
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

std::ostream& operator << (std::ostream &os, const float3 &a)
{
    os << a.x << ", " << a.y << ", " << a.z;
    return os;
}

//------------------------------------------------------------------------

float length(float3 a)
{
	return std::sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
}

float3 normalize(float3 a)
{
	return a / length(a);
}

float dot(float3 a, float3 b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

float3 cross(float3 a, float3 b)
{
	return float3
	(
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x
	);
}

float3 abs(float3 x)
{
	return float3(abs(x.x), abs(x.y), abs(x.z));
}

float3 min(float3 a, float3 b)
{
	return float3(std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z));
}

float3 max(float3 a, float3 b)
{
	return float3(std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z));
}

float3 clamp(float3 a, float3 minV, float3 maxV)
{
	return min(max(a, minV), maxV);
}

float3 saturate(float3 a)
{
	return clamp(a, float3(0), float3(1));
}

float3 nanToZero(float3 a)
{
	return float3
	(
		(a.x != a.x) ? 0 : a.x,
		(a.y != a.y) ? 0 : a.y,
		(a.z != a.z) ? 0 : a.z
	);
}

const float3 RIGHT  = float3(1.0, 0.0, 0.0);
const float3 UP 	= float3(0.0, 1.0, 0.0);
const float3 FW 	= float3(0.0, 0.0, 1.0);
const float3 ONE 	= float3(1.0, 1.0, 1.0);
const float3 ZERO 	= float3(0.0, 0.0, 0.0);

#endif
