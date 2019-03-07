#ifndef DEBOUNCE
    #define DEBOUNCE

#include "structures.glsl"

Ray decodeRay(Bounce bounce)
{
	Ray res;
	res.ro = bounce.data0.xyz;
	res.rd = bounce.data1.xyz;
	return res;
}

void encodeRay(inout Bounce bounce, Ray ray)
{
	bounce.data0.xyz = ray.ro;
	bounce.data1.xyz = ray.rd;
}

uint decodeFrame(Bounce bounce)
{
	return uint(bounce.data1.w);
}

void encodeFrame(inout Bounce bounce, uint fr)
{
	bounce.data1.w = float(fr);
}

vec3 decodeColor(Bounce bounce)
{
	return bounce.data2.xyz;
}

void encodeColor(inout Bounce bounce, vec3 color)
{
	bounce.data2.xyz = color;
}

uint decodeBounceCount(Bounce bounce)
{
	return uint(bounce.data0.w);
}

void encodeBounceCount(inout Bounce bounce, uint bounceCount)
{
	bounce.data0.w = float(bounceCount);
}

bool decodeIsDone(Bounce bounce)
{
	return (bounce.data2.w > 0.5);
}

void encodeIsDone(inout Bounce bounce, bool isDone)
{
	bounce.data2.w = (isDone) ? 1.0 : 0.0;
}

#endif