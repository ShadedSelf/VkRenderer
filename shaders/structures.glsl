#ifndef STRUCTS
	#define STRUCTS

struct Bounce
{
	vec4 data0;
	vec4 data1;
	vec4 data2;
};

struct Ray
{
	vec3 ro;
	vec3 rd;
};

struct Material
{
	vec3 color;
	float reflectance;
	float refractionIndex;
	float opacity;
	float roughness;
};

struct Transform
{
	vec3 pos;
	vec4 rot;
	vec3 scale;
};

struct Hit
{
	bool inside;
	vec3 normal;
	vec2 coords;
	Material mat;
	Material insideMat;
	float tmp;
	bool portal;
	ivec2 pIndices;
};

struct Sphere
{
	vec4 sph;
	Material mat;
};

struct Plane
{
	vec4 plane;
	Material mat;
};

struct Torus
{
	vec2 tor;
	Material mat;
	Transform trs;
};

struct Capsule
{
	vec4 cap;
	float z;
	Material mat;
	Transform trs;
};

struct Cube
{
	vec3 size;
	Material mat;
	Transform trs;
};

struct Cylinder
{
	float rad;
	Material mat;
	Transform trs;
};

struct Cone
{
	float halfAngle;
	Material mat;
	Transform trs;
};

struct Quadric
{
	Transform transform;
	Material mat;
	mat4 matrix;
};

#endif
