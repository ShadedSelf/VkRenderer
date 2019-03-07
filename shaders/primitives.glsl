#include "globals.glsl"
#include "quaternions.glsl"

vec2 iQuadric(vec3 ro, vec3 rd, mat4 matrix)
{
	vec4 o = vec4(ro, 1.0);
	vec4 d = vec4(rd, 0.0);

	float a = dot(d, matrix * d);
	float b = dot(d, matrix * o);
	float c = dot(o, matrix * o);

	float h = b * b - a * c;
	if (h < 0) { return vec2(INFINITY); }
	float t = sqrt(h);
	vec2 res = (-b + vec2(-t, t)) / a;

	if (res.x < 0.0 && res.y < 0.0)
		return (res.x < res.y) ? res.yx : res.xy;
	if (res.x > 0.0 && res.y > 0.0)
		return (res.x < res.y) ? res.xy : res.yx;
	return (res.x > res.y) ? res.xy : res.yx;
}

vec3 nQuadric(vec3 hitPos, mat4 matrix)
{
	vec3 res;
	res.x = matrix[0][0]*hitPos.x + matrix[1][0]*hitPos.y + matrix[2][0]*hitPos.z + matrix[3][0];
	res.y = matrix[1][0]*hitPos.x + matrix[1][1]*hitPos.y + matrix[2][1]*hitPos.z + matrix[3][1];
	res.z = matrix[2][0]*hitPos.x + matrix[2][1]*hitPos.y + matrix[2][2]*hitPos.z + matrix[3][2];
	return normalize(res);
}

  ////////////////////////////////////////////////////
 ////   Sphere  -->  [ x² + y² + z² - r² = 0 ]   ////
////////////////////////////////////////////////////

vec2 iSphere(vec3 ro, vec3 rd, vec4 sph)
{
	ro -= sph.xyz;
	float b = dot(ro, rd);
	float c = dot(ro, ro) - sph.w * sph.w;
	float h = b * b - c;
	if (h < 0.0) { return vec2(INFINITY); }
	float t = sqrt(h);
	vec2 res = -b + vec2(-t, t);

	if (res.x < 0.0 && res.y < 0.0)
		return (res.x < res.y) ? res.yx : res.xy;
	if (res.x > 0.0 && res.y > 0.0)
		return (res.x < res.y) ? res.xy : res.yx;
	return (res.x > res.y) ? res.xy : res.yx;
}

vec3 nSphere(vec3 wpos, vec3 pos)
{
	wpos -= pos;
	return normalize(wpos);
}

vec2 cSphere(vec3 p, vec4 sph)
{
	p -= sph.xyz;
	float o = atan(p.z, p.x) / PI;
	// float o = abs(atan(p.z, p.x)) / PI;
	float O = acos(p.y / sph.w) / PI;
	return vec2(o, O);
}


  /////////////////////////////////////////////////
 ////   Cylinder  -->  [ x² + y² - r² = 0 ]   ////
/////////////////////////////////////////////////

vec2 iCylinder(vec3 ro, vec3 rd, vec4 cy)
{
	// ro -= cy.xyz;
	float a = dot(rd.xz, rd.xz);
	float b = dot(ro.xz, rd.xz);
	float c = dot(ro.xz, ro.xz) - cy.w * cy.w;
	float h = b * b - a * c;
	if (h < 0) { return vec2(INFINITY); }
	float t = sqrt(h);
	vec2 res = (-b + vec2(-t, t)) / a;

	if (res.x < 0.0 && res.y < 0.0)
		return (res.x < res.y) ? res.yx : res.xy;
	if (res.x > 0.0 && res.y > 0.0)
		return (res.x < res.y) ? res.xy : res.yx;
	return (res.x > res.y) ? res.xy : res.yx;
}

vec3 nCylinder(vec3 hit, vec3 pos, vec3 dir)
{
	return normalize(hit - dir*dot(hit-pos, dir) - pos);
}

vec2 cCylinder(vec3 p, vec4 cy)
{
	// p -= cy.xyz;
	float o = abs(atan(p.z, p.x)) / PI;
	// float o = acos(p.x / cy.w) / PI;
	float O = p.y;
	return vec2(o, O);
}


vec2 iPlane(vec3 ro, vec3 rd, vec4 p)
{
	float x = -(dot(ro, p.xyz) + p.w) / dot(rd, p.xyz);
	float y = -9999999999.9 * dot(rd, p.xyz);
	if (x < 0.0)
		return vec2(y, x);
	return vec2(x, y);
}


  /////////////////////
 ////   Capsule   ////
/////////////////////

vec2 iCapsule(vec3 ro, vec3 rd, vec4 cy, float z)
{
	//ro -= cy.xyz;
	float a = dot(rd.xz, rd.xz);
	float b = dot(ro.xz, rd.xz);
	float c = dot(ro.xz, ro.xz) - cy.w * cy.w;
	float h = b * b - a * c;
	if (h < 0) { return vec2(INFINITY); }
	float t = sqrt(h);

	float t1 = (-b - t) / a;
	float t2 = (-b + t) / a;
	float z1 = ro.y + rd.y * t1;
	float z2 = ro.y + rd.y * t2;
	if (abs(z1) > z)
		t1 = iSphere(ro, rd, vec4(0.0, z * sign(z1), 0.0, cy.w)).x;
	if (abs(z2) > z)
		t2 = iSphere(ro, rd, vec4(0.0, z * sign(z2), 0.0, cy.w)).y;

	vec2 res = vec2(t1, t2);
	if(res.x < 0)
		return res.yx;
	return (res.x < res.y) ? res.xy : res.yx;
}

vec3 nCapsule(vec3 wpos, vec4 cy, float z)
{
	vec3 p = wpos - cy.xyz;
	if (abs(p.y) >= z - Epsilon)
		return nSphere(p, vec3(0.0, z * sign(p.y), 0.0));
	return normalize(p * vec3(1.0, 0.0, 1.0));
}



float wwdot(vec3 a, vec3 b)
{
	return a.x * b.x - (a.y * b.y)*(a.y * b.y)*(1.0 - pow(a.y * b.y, .25)) + a.z * b.z;
}

vec2 iCon(vec3 ro, vec3 rd)
{
	float a = wwdot(rd, rd);
	float b = wwdot(ro, rd);
	float c = wwdot(ro, ro) - 0.0;
	float h = b * b - a * c;
	if (h < 0.0) { return vec2(INFINITY); }
	float t = sqrt(h);
	return (-b + vec2(-t, t)) / a;
}

vec2 iCube(vec3 ro, vec3 rd, vec3 pos, vec3 size)
{
	vec3 m = 1.0 / rd;
    vec3 n = m * ro;
    vec3 k = abs(m) * size;

    vec3 t1 = -n - k;
    vec3 t2 = -n + k;

    float tN = max(max(t1.x, t1.y), t1.z);
    float tF = min(min(t2.x, t2.y), t2.z);

    if (tN > tF || tF < 0.0) return vec2(INFINITY);

    // nor = -sign(rd)*step(t1.yzx,t1.xyz)*step(t1.zxy,t1.xyz);
	// if (tN < 0)
	// 	nor = sign(rd)*step(-t2.yzx,-t2.xyz)*step(-t2.zxy,-t2.xyz);

    if(tN < 0)
    	return vec2(tF, tN);
    return (tN < tF) ? vec2(tN, tF) : vec2(tF, tN);
}

vec3 nCube(vec3 wpos, vec3 pos, vec3 size)
{
	wpos -= pos;
	wpos /= size;

	float m = max(max(abs(wpos.x), abs(wpos.y)), abs(wpos.z));
	// vec3 n = step(abs(wpos), vec3(m - .00001)) * sign(wpos);
	vec3 n = normalize(saturate(abs(wpos) - vec3(m - Epsilon))) * sign(wpos);
	return n;
}


  /////////////////////////////////////////////////////////////
 ////   Torus  -->  [ (√(x² + y²) - R)² + z² - r² = 0 ]   ////
/////////////////////////////////////////////////////////////

vec2 iTorus(vec3 ro, vec3 rd, vec3 pos, float R, float r)
{
	ro -= pos;
	float dd = dot(rd,rd); //1.0
	float od = dot(ro,rd);
	float oo = dot(ro,ro) - R*R - r*r;
	float frs = 4.0 * R*R;

	float a = dd*dd; //1.0
	float b = 4.0 * dd * od;
	float c = 2.0 * dd * oo + 4.0 * od*od + frs * rd.z*rd.z;
	float d = 4.0 * oo * od + 2.0 *	frs * ro.z*rd.z;
	float e = oo*oo - frs * (r*r - ro.z*ro.z);

	//float root = iCube(ro, rd, ZERO, vec3(vec2(R + r), r), rd).x;
	//if (root < 0.0) { return -1.0; }
	vec4 tmp;
	int n = quartic(b, c, d, e, tmp); // A == 1.0, test non normalized RD
	if (n == 4)
	{
		if (tmp.y > tmp.z) { tmp.yz = tmp.zy; }
		if (tmp.x > tmp.y) { tmp.xy = tmp.yx; }
		if (tmp.z > tmp.w) { tmp.zw = tmp.wz; }
		if (tmp.y > tmp.z) { tmp.yz = tmp.zy; }
	}

	if (n == 2)
	{
		// if (tmp.x > tmp.y) { tmp.xy = tmp.yx; }
		if (tmp.x > 0.0 && tmp.y > 0.0)
			return tmp.xy;
		return tmp.yx;
	}

	if (n == 4)
	{
		if (tmp.x > 0.0 && tmp.y > 0.0)
			return tmp.xy;
		if (tmp.x < 0.0 && tmp.y > 0.0)
			return tmp.yx;
		if (tmp.y < 0.0 && tmp.z > 0.0)
			return tmp.zw;
		if (tmp.z < 0.0 && tmp.w > 0.0)
			return tmp.wz;
	}

	// if (tmp.x < 0.0 && tmp.y > 0.0)
	// 	return tmp.yx;
	// if (tmp.y < 0.0 && tmp.z > 0.0)
	// 	return tmp.zy;
	// if ()
	// 	return tmp.xy;


	// if (n != 0 && tmp.x > 0.0)
	// 	return tmp.xy;
	return vec2(INFINITY);
}

vec3 nTorus(vec3 wpos, vec3 pos, vec2 tor)
{
	wpos -= pos;
	return normalize(wpos * (dot(wpos,wpos) - tor.y*tor.y - tor.x*tor.x * vec3(1.0, 1.0, -1.0)));
}
