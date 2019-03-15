#ifndef MARCH
 #define MARCH

#define PHI (1.618033988749895)


vec3 _PreRotVec = normalize(UP + FW * 3.1);
float _PreRotAngle = 4.0;

vec3 _PostRotVec = -RIGHT - UP*0.4;
float _PostRotAngle = -0.5;

uint _Iterations = 40;

// vec3 _OffSet = vec3(1.4, 0.61, 0.0);
vec3 _OffSet = vec3(1.2, 0.51, 0.0);

float _Scale = 2.0;
float _Rad = 0.1;

vec4 folds[6];
int numFolds;
void icosaFolds()
{
	numFolds = 6;

	vec3 n1 = normalize(vec3(-PHI, PHI - 1.0, 1.0));
	vec3 n2 = normalize(vec3(1.0, -PHI, PHI + 1.0));

	folds[0] = vec4(1, 1, 1, 1);
	folds[1] = vec4(n2, 0);
	folds[2] = vec4(1, 0, 1, 1);
	folds[3] = vec4(n1, 0);
	folds[4] = vec4(n2, 0);
	folds[5] = vec4(0, 0, 1, 1);
}

float fcolor(vec3 p)
{
	float m = 1,
		  o = 1;
	p = qMulVec(p, createRotation(_PreRotVec, -_PreRotAngle));
	for (int n = 0; n < _Iterations; n++)
	{
		p = qMulVec(p, createRotation(_PreRotVec, _PreRotAngle));
		for (int j = 0; j < numFolds; j++)
		{
			p =	mix
			(
				p - 2.0 * max(0.0, dot(p, folds[j].xyz) - 0.0) * folds[j].xyz,
				mix(p, abs(p), folds[j].xyz),
				folds[j].w
			);
		}
		p = qMulVec(p - _OffSet, createRotation(_PostRotVec, _PostRotAngle)) + _OffSet;
		p = p * _Scale - _OffSet * (_Scale - 1.0);

		float t = distance(p, vec3(0.75, 0.25, 0.3));
		t *= t;
		m = max(m, t);
		o = min(o, t / m);
	}
	return o;
}

float fraction(vec3 p)
{
	p = qMulVec(p, createRotation(_PreRotVec, -_PreRotAngle));
	for (int n = 0; n < _Iterations; n++)
	{
		p = qMulVec(p, createRotation(_PreRotVec, _PreRotAngle));
		for (int j = 0; j < numFolds; j++)
		{
			p =	mix
			(
				p - 2.0 * max(0.0, dot(p, folds[j].xyz) - 0.0) * folds[j].xyz,
				mix(p, abs(p), folds[j].xyz),
				folds[j].w
			);
		}
		p = qMulVec(p - _OffSet, createRotation(_PostRotVec, _PostRotAngle)) + _OffSet;
		p = p * _Scale - _OffSet * (_Scale - 1.0);
	}
	return (length(p) - _Rad) * pow(_Scale, -float(_Iterations));
}




mat2 r2(float th)
{
	vec2 a = sin(vec2(PI * 0.5, 0) + th);
	return mat2(a, -a.y, a.x);
}

float mob(vec3 p)
{
	float a = atan(p.z, p.x);
	p.xz *= r2(a);
	p.x -= 2.5;
	p.xy *= r2(a * 0.25);
	vec2 d = abs(p.xy) - vec2(0.5, 0.5);
	float dd = min(max(d.x, d.y), 0) + length(max(d, 0.0));

    vec2 f = abs(p.xy) - vec2(0.6, 0.25);
	float ff = min(max(f.x, f.y), 0) + length(max(f, 0.0));
	vec2 e = abs(p.xy) - vec2(0.25, 0.6);
	float ee = min(max(e.x, e.y), 0) + length(max(e, 0.0));
    float aa = min(ff, ee);
    // float cc = max(dd, -aa);
    return aa;

    return dd;
}

vec3 opPolarTwistRep(vec3 p, float radius, float rot, float reps)
{
	float a = atan(p.z, p.x);
	float ia = floor(reps*a / 6.2831853);
	ia = (ia + .5) / reps * 6.2831853;
	p.xz *= r2(ia);
	p.x -= radius;
	p.xy *= r2(a * rot);

	return abs(p);
}

float boxrep(vec3 p)
{
	p = opPolarTwistRep(p, 2.5, 0.25, 16.0);

	vec3 d = abs(p) - vec3(0.2, 0.65, 0.2);
	float dd = min(max(d.x, max(d.y, d.z)), 0) + length(max(d, 0));

	vec3 e = abs(p) - vec3(0.65, 0.2, 0.2);
	float ee = min(max(e.x, max(e.y, e.z)), 0) + length(max(e, 0));

	// vec3 f = abs(p) - vec3(0.1, 0.1, 0.6);
	// float ff = min(max(f.x, max(f.y, f.z)), 0) + length(max(f, 0));
	// return ff;
	return min(dd, ee);
}

float boxreps(vec3 p)
{
	p = opPolarTwistRep(p, 2.5, 0.25, 16.0);

	vec3 d = abs(p) - vec3(0.15, 0.65, 0.15);
	float dd = min(max(d.x, max(d.y, d.z)), 0) + length(max(d, 0));

	vec3 e = abs(p) - vec3(0.65, 0.15, 0.15);
	float ee = min(max(e.x, max(e.y, e.z)), 0) + length(max(e, 0));

	return min(dd, ee);
}

float ballreps(vec3 p)
{
    p = opPolarTwistRep(p, 2.5, 0.5, 16.0);
    p -= UP;
    return length(p) - 0.5;
}

bool testbool;
bool insidebool;
float map(vec3 p)
{
	// testbool = true;
	// vec3 f = abs(p - FW * 2.0) - vec3(0.5, 0.5, 0.5);
	// return min(max(f.x, max(f.y, f.z)), 0) + length(max(f, 0));

	// float a = boxrep(p);
	// float b = mob(p);
	// float c = boxreps(p);
	
	// float o = max(b, -a);
	// // testbool = c < o;
	// // return o;
    // // return b;
	// float m = min(min(o, c), ballreps(p));
	// return m;
    // float f = min(m, fraction((p / 3.0) - UP * 1.0 + RIGHT * 2.0 + FW*3.0));
    // return f;

	return fraction(p);
}

vec3 calcNormal(in vec3 pos)
{
	vec2 e = vec2(1.0, -1.0) * 0.5773 * 0.0005;
	return normalize(
		e.xyy * map(pos + e.xyy) +
		e.yyx * map(pos + e.yyx) +
		e.yxy * map(pos + e.yxy) +
		e.xxx * map(pos + e.xxx));
}

bool raymarch(Ray ray, float drawDist, out float lt)
{
	float t = 0;
	float d = 100;
	drawDist = min(drawDist, 25.0);
	for (int i = 0; i < 200 && d > /*Epsilon/*/0.00001 && t < drawDist; i++)
	{
		d = map(ray.ro + ray.rd * t);
		insidebool = d < 0.0;
		d = abs(d);
		t += d;
		// t += d * 0.9;
	}
	lt = t;
	return (t < drawDist);
}

vec3 rain(float a, float t)
{
	return sin((fract((a / PI) * 0.5 + 0.5) + t + 0.75/**/ +
		vec3(0.0, 0.33333333, 0.66666666)) * PI * 2.0) * 0.5 + 0.5;
}

 #endif
