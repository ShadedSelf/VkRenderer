#ifndef GLOBALS
	#define GLOBALS

	#define INFINITY 1.0/0.0
	#define PI acos(-1.0)
	#define Epsilon 0.00001
	// #define Epsilon 0.001

	#define time ubo.ubotime
	#define frame ubo.uboframe

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


	layout(set = 1, binding = 0) uniform UBO
	{
		float ubotime;
		uint uboframe;
		uint t0;
	} ubo;

	layout(set = 1, binding = 1) uniform CamData
	{
		vec3 pos;
		vec3 fw;
		vec3 right;
		vec3 up;
	} cam;

	layout(set = 2, binding = 0) buffer Accum
	{
		vec4 data[];
	} acc;

	layout(set = 2, binding = 1) buffer Bo
	{
		Bounce data[];
	} bo;

	layout(set = 2, binding = 2) buffer Ra
	{
		Ray data[];
	} ra;

	vec3 cameraPos;
	vec3 cameraDir;

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
