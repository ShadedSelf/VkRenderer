Material diffuse()
{
	Material res;
	res.color = ONE;
	res.roughness = 1.0;
	res.refractionIndex = 1.0;
	res.reflectance = 1.0;
	res.opacity = 1.0;
	return res;
}

void init04()
{
	sphereNum = 1;
	Spheres[0].sph = vec4(ZERO, 1.25);
	Spheres[0].mat.color = ONE * 2.0;
	Spheres[0].mat.reflectance = -1.0;

	cubeNum = 0;
	Cubes[0].size = vec3(3.0, 0.5, 3.0);
	Cubes[0].trs.pos = vec3(-15.0, 3.0, -30.0);
	Cubes[0].trs.rot = QIDENTITY;
	Cubes[0].mat.color = vec3(1.0, 1.0, 1.0);
	Cubes[0].mat.reflectance = -1.0;
	Cubes[0].mat.roughness = 1.0;
	Cubes[0].mat.refractionIndex = 0.0;
	Cubes[0].mat.opacity = 1.0;

	Cubes[1].size = vec3(10000.0, 10000.0, 0.5);
	Cubes[1].trs.pos = vec3(2.0, 0.0, -30.0);
	Cubes[1].trs.rot = QIDENTITY;
	Cubes[1].mat.color = vec3(1.0, 1.0, 1.0);
	Cubes[1].mat.reflectance = 1.0;
	Cubes[1].mat.roughness = 1.0;
	Cubes[1].mat.refractionIndex = 0.0;
	Cubes[1].mat.opacity = 1.0;

	planeNum = 0;
	Planes[0].plane = vec4(UP, 2.0);
	Planes[0].mat = diffuse();
	Planes[0].mat.color = vec3(1.0, 0.5, 0.0);

}

void init()
{
	quadricNum = 0;
	Quadrics[0].matrix =
		mat4(1, 0, 0, 0,
			 0, -1, 0, 0,
			 0, 0, 1, 0,
			 0, 0, 0, -1);
	Quadrics[0].mat.color = ONE;
	Quadrics[0].mat.reflectance = 1.0;
	Quadrics[0].mat.refractionIndex = 1.6;
	Quadrics[0].mat.roughness = 1.0;
	Quadrics[0].mat.opacity = 0.0;


	sphereNum = 4;
	Spheres[0].sph = vec4(-2.5, 1.0, 1.0, 1.25);
	Spheres[0].mat.color = ONE * 2.0;
	Spheres[0].mat.reflectance = -1.0;

	Spheres[1].sph = vec4(vec3(-3.0, -0.4, -2.5), 1.1);
	Spheres[1].mat.color = vec3(1.0, 0.0, 1.0);
	Spheres[1].mat.opacity = 0.0;
	Spheres[1].mat.reflectance = 0.0;
	Spheres[1].mat.refractionIndex = 2.6;
	Spheres[1].mat.roughness = 0.0;

	Spheres[2].sph = vec4(ZERO, 1.0);
	Spheres[2].mat.color = vec3(1.0, 1.0, 1.0);
	Spheres[2].mat.opacity = 0.0;
	Spheres[2].mat.reflectance = 1.0;
	Spheres[2].mat.refractionIndex = 1.6;
	Spheres[2].mat.roughness = 1.0;

	Spheres[3].sph = vec4(1.0, 3.0, 1.0, 0.25);
	Spheres[3].mat.color = vec3(0.1, 0.75, 1.0) * 3.0;
	Spheres[3].mat.reflectance = -1.0;

	// Spheres[4].sph = vec4(FW * -5.0 + RIGHT * -2.0 + UP, 1.0);
	// Spheres[4].mat.color = vec3(1.0, 1.0, 1.0);
	// Spheres[4].mat.opacity = 0.0;
	// Spheres[4].mat.reflectance = 1.0;
	// Spheres[4].mat.refractionIndex = 1.6;
	// Spheres[4].mat.roughness = 1.0;


	cubeNum = 5;
	// cubeNum = 0;
	Cubes[0].size = vec3(5.0, 0.5, 8.0);
	Cubes[0].trs.pos = UP * -2.0;
	Cubes[0].trs.rot = QIDENTITY;
	Cubes[0].mat.color = vec3(1.0, 0.5, 0.0);
	// Cubes[0].mat.color = ONE;
	Cubes[0].mat.reflectance = 0.0;
	Cubes[0].mat.roughness = 1.0;
	Cubes[0].mat.refractionIndex = 0.0;
	Cubes[0].mat.opacity = 1.0;

	Cubes[1].size = vec3(0.5, 5.0, 5.0);
	Cubes[1].trs.pos = RIGHT * 2.0;
	Cubes[1].trs.rot = createRotation(UP, -0.4);
	Cubes[1].mat.color = ONE;
	Cubes[1].mat.reflectance = 1.0;
	Cubes[1].mat.refractionIndex = 1.3;
	Cubes[1].mat.opacity = 0.0;
	Cubes[1].mat.roughness = 0.0;

	Cubes[2].size = vec3(5.0, 5.0, 0.5);
	Cubes[2].trs.pos = FW * 2.0;
	Cubes[2].trs.rot = QIDENTITY;
	Cubes[2].mat.color = vec3(0.0, 0.5, 1.0);
	Cubes[2].mat.reflectance = 0.0;
	Cubes[2].mat.roughness = 1.0;
	Cubes[2].mat.refractionIndex = 0.0;
	Cubes[2].mat.opacity = 1.0;

	Cubes[3].size = vec3(5.0, 0.5, 5.0);
	Cubes[3].trs.pos = vec3(-0.0, 4.0, 4.0);
	Cubes[3].trs.rot = QIDENTITY;
	Cubes[3].mat.color = vec3(0.5, 0.5, 1.0);
	Cubes[3].mat.reflectance = 0.0;
	Cubes[3].mat.roughness = 1.0;
	Cubes[3].mat.refractionIndex = 0.0;
	Cubes[3].mat.opacity = 1.0;


	Cubes[4].size = vec3(0.5, 5.0, 5.0);
	Cubes[4].trs.pos = RIGHT * -6.0;
	Cubes[4].trs.rot = createRotation(UP, -0.4);
	Cubes[4].mat.color = ONE;
	Cubes[4].mat.reflectance = 1.0;
	Cubes[4].mat.refractionIndex = 1.3;
	Cubes[4].mat.opacity = 0.0;
	Cubes[4].mat.roughness = 0.0;



	planeNum = 0;
	Planes[0].plane = vec4(-UP + RIGHT, 20.0);
	Planes[0].mat.color = ONE;
	Planes[0].mat.reflectance = -1.0;






	Cubes[7].size = ONE;
	Cubes[7].trs.pos = vec3(-3.0, -0.4, -2.5);
	Cubes[7].trs.rot = QIDENTITY;
	Cubes[7].mat.color = ONE;
	Cubes[7].mat.reflectance = 1.0;
	Cubes[7].mat.roughness = 0.0;
	Cubes[7].mat.refractionIndex = 1.3;
	Cubes[7].mat.opacity = 0.0;


	capsuleNum = 0;
	Capsules[0].cap = vec4(ZERO + UP * 3.0, 1.0);
	Capsules[0].z = 2.0;
	Capsules[0].trs.pos = vec3(6.5, 1.0, 0.0);
	Capsules[0].trs.rot = QIDENTITY;
	Capsules[0].mat.color = UP;
	Capsules[0].mat.reflectance = 0.0;
	Capsules[0].mat.roughness = 0.0;
	Capsules[0].mat.opacity = 1.0;
	Capsules[0].mat.refractionIndex = 1.6;



	torusNum = 0;
	Tori[0].tor = vec2(1.0, 0.5);
	Tori[0].trs.pos = vec3(-1.0, 1.0, -3.0);
	Tori[0].trs.rot = createRotation(UP, PI * 0.5);
	Tori[0].mat.color = ONE;
	Tori[0].mat.reflectance = 0.0;
	Tori[0].mat.roughness = 0.0;
	Tori[0].mat.opacity = 0.0;
	Tori[0].mat.refractionIndex = 1.3;
}









void init1()
{

	sphereNum = 1;

	Spheres[0].sph = vec4(vec3(-2.0, -1.5, 1.0), 1.25);
	Spheres[0].mat.color = ONE;
	Spheres[0].mat.opacity = 0.0;
	Spheres[0].mat.reflectance = 0.0;
	Spheres[0].mat.refractionIndex = 1.6;
	Spheres[0].mat.roughness = 0.0;



	cubeNum = 7;
	Cubes[0].size = vec3(5.0, 0.5, 5.0);
	Cubes[0].trs.pos = UP * 5.0;
	Cubes[0].trs.rot = QIDENTITY;
	Cubes[0].mat.color = ONE;
	Cubes[0].mat.reflectance = 0.0;
	Cubes[0].mat.roughness = 1.0;
	Cubes[0].mat.refractionIndex = 0.0;
	Cubes[0].mat.opacity = 1.0;

	Cubes[1].size = vec3(5.0, 0.5, 5.0);
	Cubes[1].trs.pos = UP * -4.0;
	Cubes[1].trs.rot = QIDENTITY;
	Cubes[1].mat.color = ONE;
	Cubes[1].mat.reflectance = 1.0;
	Cubes[1].mat.refractionIndex = 1.3;
	Cubes[1].mat.opacity = 0.0;
	Cubes[1].mat.roughness = 1.0;

	Cubes[2].size = vec3(5.0, 5.0, 0.5);
	Cubes[2].trs.pos = FW * 5.0;
	Cubes[2].trs.rot = QIDENTITY;
	Cubes[2].mat.color = ONE;
	Cubes[2].mat.reflectance = 1.0;
	Cubes[2].mat.roughness = 1.0;
	Cubes[2].mat.refractionIndex = 0.0;
	Cubes[2].mat.opacity = 1.0;

	Cubes[3].size = vec3(0.5, 5.0, 5.0);
	Cubes[3].trs.pos = RIGHT * 5.0;
	Cubes[3].trs.rot = QIDENTITY;
	Cubes[3].mat.color = vec3(0.0, 0.5, 1.0);
	Cubes[3].mat.reflectance = 1.0;
	Cubes[3].mat.roughness = 1.0;
	Cubes[3].mat.refractionIndex = 0.0;
	Cubes[3].mat.opacity = 1.0;

	Cubes[4].size = vec3(0.5, 5.0, 5.0);
	Cubes[4].trs.pos = RIGHT * -5.0;
	Cubes[4].trs.rot = QIDENTITY;
	Cubes[4].mat.color = vec3(1.0, 0.5, 0.0);
	Cubes[4].mat.reflectance = 1.0;
	Cubes[4].mat.refractionIndex = 1.3;
	Cubes[4].mat.opacity = 0.0;
	Cubes[4].mat.roughness = 1.0;

	Cubes[5].size = vec3(1.0, 1.0, 1.0);
	Cubes[5].trs.pos = UP * 5.49 + FW * 2.0;
	Cubes[5].trs.rot = QIDENTITY;
	Cubes[5].mat.color = ONE * 5.0;
	Cubes[5].mat.reflectance = -1.0;
	Cubes[5].mat.refractionIndex = 1.3;
	Cubes[5].mat.opacity = 0.0;
	Cubes[5].mat.roughness = 1.0;

	Cubes[6].size = vec3(1.5, 5.0, 1.5);
	Cubes[6].trs.pos = RIGHT * 1.5 - UP * 3.0 + FW;
	Cubes[6].trs.rot = createRotation(UP, PI * 0.25);
	Cubes[6].mat.color = ONE;
	Cubes[6].mat.reflectance = 1.0;
	Cubes[6].mat.refractionIndex = 1.3;
	Cubes[6].mat.opacity = 0.0;
	Cubes[6].mat.roughness = 1.0;















	planeNum = 0;
	Planes[0].plane = vec4(-UP + RIGHT, 20.0);
	Planes[0].mat.color = ONE;
	Planes[0].mat.reflectance = -1.0;






	Cubes[7].size = ONE;
	Cubes[7].trs.pos = vec3(-3.0, -0.4, -2.5);
	Cubes[7].trs.rot = QIDENTITY;
	Cubes[7].mat.color = ONE;
	Cubes[7].mat.reflectance = 1.0;
	Cubes[7].mat.roughness = 0.0;
	Cubes[7].mat.refractionIndex = 1.3;
	Cubes[7].mat.opacity = 0.0;


	capsuleNum = 0;
	Capsules[0].cap = vec4(ZERO + UP * 3.0, 1.0);
	Capsules[0].z = 2.0;
	Capsules[0].trs.pos = vec3(6.5, 1.0, 0.0);
	Capsules[0].trs.rot = QIDENTITY;
	Capsules[0].mat.color = UP;
	Capsules[0].mat.reflectance = 0.0;
	Capsules[0].mat.roughness = 0.0;
	Capsules[0].mat.opacity = 1.0;
	Capsules[0].mat.refractionIndex = 1.6;



	torusNum = 0;
	Tori[0].tor = vec2(1.0, 0.5);
	Tori[0].trs.pos = vec3(-1.0, 1.0, -3.0);
	Tori[0].trs.rot = createRotation(UP, PI * 0.5);
	Tori[0].mat.color = ONE;
	Tori[0].mat.reflectance = 0.0;
	Tori[0].mat.roughness = 0.0;
	Tori[0].mat.opacity = 0.0;
	Tori[0].mat.refractionIndex = 1.3;
}
