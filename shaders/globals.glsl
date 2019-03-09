#ifndef GLOBALS
	#define GLOBALS

	#define INFINITY 1.0/0.0
	#define PI acos(-1.0)
	#define Epsilon 0.00001
	// #define Epsilon 0.001

	#define iTime globals.time
	#define iFrame globals.frame

	#define RIGHT	vec3(1.0, 0.0, 0.0)
	#define UP		vec3(0.0, 1.0, 0.0)
	#define FW		vec3(0.0, 0.0, 1.0)
	#define ZERO 	vec3(0.0, 0.0, 0.0)
	#define ONE		vec3(1.0, 1.0, 1.0)

	#define saturate(x) clamp(x, 0.0, 1.0)

	uint pixelID;
	uint iSeed;

	float randMe()
	{
		iSeed += (iSeed << 10u);
		iSeed ^= (iSeed >>  6u);
		iSeed += (iSeed <<  3u);
		iSeed ^= (iSeed >> 11u);
		iSeed += (iSeed << 15u);

		const uint ieeeMantissa = 0x007FFFFFu;
		const uint ieeeOne      = 0x3F800000u;

		uint tmp = iSeed;
		tmp &= ieeeMantissa;
		tmp |= ieeeOne;

		return uintBitsToFloat(tmp) - 1.0;
	}

	layout (set = 0, binding = 0, rgba8) uniform image2D render;

	layout(set = 0, binding = 1) uniform gg
	{
		float time;
		uint frame;
	} Globals;

	layout(set = 0, binding = 2) uniform cc
	{
		vec3 pos;
		vec3 fw;
		vec3 right;
		vec3 up;
	} Cam;

	layout(set = 0, binding = 3) buffer aa
	{
		vec4 data[];
	} Accum;

	layout(set = 0, binding = 4) buffer bb
	{
		Bounce data[];
	} Bounces;

	layout(set = 0, binding = 5) buffer rr
	{
		Ray data[];
	} Rays;

	int sphereNum;
	Sphere Spheres[8];

	int planeNum;
	Plane Planes[8];

	int torusNum;
	Torus Tori[8];

	int capsuleNum;
	Capsule Capsules[8];

	int cubeNum;
	Cube Cubes[8];

	int cylinderNum;
	Cylinder Cylinders[8];

	int coneNum;
	Cone Cones[8];

	int quadricNum;
	Quadric Quadrics[8];

#endif
