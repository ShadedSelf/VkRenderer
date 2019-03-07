#ifndef INTERSECT
	#define INTERSECT

#include "cut.glsl"
#include "march.glsl"



Ray transformRay(Ray pRay, Transform trs)
{
	Ray res;
	res.ro = qMulVec(pRay.ro - trs.pos, trs.rot);
	res.rd = qMulVec(pRay.rd, trs.rot);
	return res;
}

float intersect(Ray ray, out Hit hit)
{
	Ray tRay;
	vec3 tmp;
	float t = INFINITY;
	float it = -999999.0;
	hit.inside = false;
	hit.portal = false;

	// Quadrics
	for (int i = 0; i < quadricNum; ++i)
	{
		Quadric q = Quadrics[i];
		vec2 h = iQuadric(ray.ro, ray.rd, q.matrix);
		if (h.x > Epsilon && h.x < t)
		{
			t = h.x;
			vec3 hitPos = ray.ro + ray.rd * t;
			hit.normal = nQuadric(hitPos, q.matrix);
			hit.inside = (sign(dot(hit.normal, ray.ro - hitPos)) < 0.0);
			hit.normal *= sign(dot(hit.normal, ray.ro - hitPos));
			hit.mat = q.mat;
		}
	}
	//	Spheres
	for (int i = 0; i < sphereNum; ++i)
	{
		Sphere s = Spheres[i];
		vec2 h = iSphere(ray.ro, ray.rd, s.sph);
		if (h.x > Epsilon && h.x < t)
		{
			t = h.x;
			hit.normal = nSphere(ray.ro + ray.rd * h.x, s.sph.xyz) * sign(h.y);
			hit.coords = cSphere(ray.ro + ray.rd * h.x, s.sph);
			hit.mat = s.mat;
		}
		if (h.y < 0.0 && h.x > 0.0 && h.y > it)
		{
			it = h.y;
			hit.insideMat = s.mat;
			hit.inside = true;
		}
	}
	//	Cubes
	for (int i = 0; i < cubeNum; ++i)
	{
		Cube cube = Cubes[i];
		tRay = transformRay(ray, cube.trs);
		vec2 h = iCube(tRay.ro, tRay.rd, ZERO, cube.size);
		if (h.x > Epsilon && h.x < t)
		{
			t = h.x;
			hit.normal = qMulVec(nCube(tRay.ro + tRay.rd * t, ZERO, cube.size), qConjugate(cube.trs.rot)) * sign(h.y);
			hit.mat = cube.mat;
		}
		if (h.y < 0.0 && h.x > 0.0 && h.y > it)
		{
			it = h.y;
			hit.insideMat = cube.mat;
			hit.inside = true;
		}
	}
	//	Planes
	for (int i = 0; i < planeNum; ++i)
	{
		vec2 h = iPlane(ray.ro, ray.rd, Planes[i].plane).xx;
		if (h.x > Epsilon && h.x < t)
		{
			t = h.x;
			hit.normal = Planes[i].plane.xyz;
			hit.inside = (sign(h.y) < 0);
			hit.mat = Planes[i].mat;
		}
	}
	//	Capsules
	for (int i = 0; i < capsuleNum; ++i)
	{
		Capsule cap = Capsules[i];
		tRay = transformRay(ray, cap.trs);
		vec2 h = iCapsule(tRay.ro, tRay.rd, cap.cap, cap.z);
		if (h.x > Epsilon && h.x < t)
		{
			t = h.x;
			hit.normal = qMulVec(nCapsule(tRay.ro + tRay.rd * t, cap.cap * vec4(ZERO, 1.0), cap.z), qConjugate(cap.trs.rot)) * sign(h.y);
			hit.mat = cap.mat;
			if (h.y < 0.0 && h.y > it)
			{
				it = h.y;
				hit.insideMat = cap.mat;
				hit.inside = true;
			}
		}
	}
	// 	Tori
	for (int i = 0; i < torusNum; ++i)
	{
		Torus tor = Tori[i];
		tRay = transformRay(ray, tor.trs);
		vec2 h = iTorus(tRay.ro, tRay.rd, ZERO, tor.tor.x, tor.tor.y);
		if (h.x > Epsilon && h.x < t)
		{
			t = h.x;
			hit.normal = qMulVec(nTorus(tRay.ro + tRay.rd * t, ZERO, tor.tor), qConjugate(tor.trs.rot)) * sign(h.y);
			hit.mat = tor.mat;
			if (h.y < 0.0 && h.y > it)
			{
				it = h.y;
				hit.insideMat = tor.mat;
				hit.inside = true;
			}
		}
	}

{
	Cube cube = Cubes[7];
	tRay = transformRay(ray, cube.trs);

	vec2 form = iCube(tRay.ro, tRay.rd, ZERO, cube.size);
	vec2 cutter = iSphere(tRay.ro, tRay.rd, vec4(ZERO, 1.2));

	vec3 formN = nCube(tRay.ro + tRay.rd * form.x, ZERO, cube.size) * sign(form.y);

	vec3 cutterInN = nSphere(tRay.ro + tRay.rd * cutter.y, ZERO);
	vec3 cutterOutN = nSphere(tRay.ro + tRay.rd * cutter.x, ZERO);

	vec3 cutN;
	vec2 h = cut(form, cutter, formN, cutterInN, cutterOutN, cutN);
	if (h.x > Epsilon && h.x < t)
	{
		t = h.x;
		hit.normal = qMulVec(cutN, qConjugate(cube.trs.rot));
		hit.inside = (sign(h.y) < 0);
		hit.mat = cube.mat;
	}
}



	// ray.ro -= UP + FW*2.0 + RIGHT;

	// icosaFolds();
	// float tt = INFINITY;
	// vec3 nnn;
	// bool spec = raymarch(ray, t, tt);
	// if (spec && tt < t)
	// {
	// 	t = tt;
	// 	Material tmp = diffuse();

	// 	hit.normal = calcNormal(ray.ro + ray.rd * t);
	// 	// if (testbool)
	// 	// {
	// 	// 	tmp.reflectance = 0.0;
	// 	// 	tmp.refractionIndex = 1.2;
	// 	// 	tmp.roughness = 0.0;
	// 	// 	hit.inside = insidebool;
	// 	// 	// hit.insideMat.color = rain(atan((ray.ro + ray.rd * t).x, (ray.ro + ray.rd * t).z), 0.0);
	// 	// 	if (insidebool)
	// 	// 		hit.normal *= -1.0;
	// 	// }
	// 	hit.mat = tmp;
	// }

	return t;
}

#endif
