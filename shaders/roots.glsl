
#define PI acos(-1.0)

  //////////////////////////////////
 ////   Analytical Quadratic   ////
//////////////////////////////////

bool analyticalQuadratic(float a, float b, float c, inout vec2 roots)
{
	float discriminant = b * b - 4.0 * a * c;
	if (discriminant >= 0.0)
	{
		discriminant = sqrt(discriminant);
		roots.x = (-b - discriminant) * 0.5 / a;
		roots.y = (-b + discriminant) * 0.5 / a;
		return true;
	}
	return false;
}


  /////////////////////////////
 ////   Numerical Cubic   ////
/////////////////////////////

float cf(float a, float b, float c, float d, float x)
{
	return a*x*x*x + b*x*x + c*x + d;
}

float cfp(float a, float b, float c, float x)
{
	return 3.0*a*x*x + 2.0*b*x + c;
}

bool numericalCubic(float a, float b, float c, float d, inout float root)
{
    float x, x1, fx, fx1;
	x1 = root;

    fx = cf(a, b, c, d, x1);
    fx1 = cfp(a, b, c, x1);

    for (int i = 0; i < 25; ++i)
    {
        x = x1;
		fx = cf(a, b, c, d, x);
		fx1 = cfp(a, b, c, x);
        x1 = x - (fx/fx1);

		if (abs(x1 - x) <= .001)
		{
			root = x;
			return true;
		}
	}
    return false;
}


  ///////////////////////////////
 ////   Numerical Quartic   ////
///////////////////////////////

float evaluate(float[5] numbers, int n, float x)
{
	float result = 0;
	for (int i = n - 1; i >= 0; --i)
		result = result * x + numbers[i];
	return result;
}

// float qf(float a, float b, float c, float d, float e, float x)
// {
// 	return a*x*x*x*x + b*x*x*x + c*x*x + d*x + e;
// }

// float qfp(float a, float b, float c, float d, float x)
// {
// 	return a*x*x*x + b*x*x + c*x + d;
// }

float qf(float a, float b, float c, float d, float e, float x)
{
	float result = 0.0;
	result = result * x + a;
	result = result * x + b;
	result = result * x + c;
	result = result * x + d;
	result = result * x + e;
	return result;
}

float qfp(float a, float b, float c, float d, float x)
{
	float result = 0.0;
	result = result * x + a;
	result = result * x + b;
	result = result * x + c;
	result = result * x + d;
	return result;
}

bool numericalQuartic(float a, float b, float c, float d, float e, inout float root)
{
    float x, x1, fx, fx1;
	x1 = root;

    fx = qf(a, b, c, d, e, x1);
	fx1 = qfp(4.0*a, 3.0*b, 2.0*c, d, x1);

    for (int i = 0; i < 10; ++i)
    {
        x = x1;
		fx = qf(a, b, c, d, e, x);
		fx1 = qfp(4.0*a, 3.0*b, 2.0*c, d, x1);
        x1 = x - (fx/fx1);

		if (abs(x1 - x) <= .001)
		{
			root = x;
			return true;
		}
	}
    return false; //return the root nd true anyways??
}



bool solveQuadraticOther(float a, float b, float c, inout float root)
{
	if ((b*b - c) < 0.0)
		return false;
	if(a == 0.0 || abs(a/b) < 1.0e-6)
	{
		if (abs(b) < 1.0e-6)
			return false;
		else
		{
			root = -c/b;
			return true;
		}
	}

	float discriminant = b * b - 4.0 * a * c;
	if(discriminant >= 0.0)
	{
		discriminant = sqrt(discriminant);
		root = (b - discriminant) * -0.5 / a;
		// root = min((b - discriminant) * -0.5 / a, (b + discriminant) * -0.5 / a);
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
bool solveQuadratic(float a, float b, float c, inout float root)
{
	if ((b*b - c) < 0.0)
		return false;
	if(a == 0.0 || abs(a/b) < 1.0e-6)
	{
		if(abs(b) < 1.0e-6)
			return false;
		else
		{
			root = -c/b;
			return true;
		}
	}

	float discriminant = b * b - 4.0 * a * c;
	if(discriminant >= 0.0)
	{
		discriminant = sqrt(discriminant);
		root = (b + discriminant) * -0.5 / a;
		// root = min((b - discriminant) * -0.5 / a, (b + discriminant) * -0.5 / a);
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
bool solveCubic(float a, float b, float c, float d, inout float root)
{
	float B = b/a, C = c/a, D = d/a;

	float Q 			= ( B*B - C*3.0 ) / 9.0;
	float R 			= ( 2.0*B*B*B - 9.0*B*C + 27.0*D ) / 54.0;
	float discriminant 	= R*R - Q*Q*Q;
	float off 			= B/3.0;

	if(discriminant < 0.0)
	{
        float theta = acos( R / sqrt( Q*Q*Q ) );
		float r = -2.0 * sqrt( Q );
		float r1, r2, r3;

        r1 = r * cos(  theta			 / 3.0) - off;
        r2 = r * cos( (theta + 2.0 * PI) / 3.0) - off;
        r3 = r * cos( (theta - 2.0 * PI) / 3.0) - off;

		// root = 1000000.0;
		// if ( r1 >= 0.0				) root = r1;
		// if ( r2 >= 0.0 && r2 < root ) root = r2;
		// if ( r3 >= 0.0 && r3 < root ) root = r3;
		root = min(min(r1, r2), r3);
	}
	else
	{
        float A2 = -pow( abs( R ) + sqrt( discriminant ), 1.0/3.0 );
		A2 *= sign( R );
		root = A2 + ( Q / A2 ) - off;
	}
	return true;
}

//----------------------------------------------------------------------------
bool solveQuartic(float a, float b, float c, float d, float e, inout float root)
{
	// if(a == 0.0 || abs(a/b) < 1.0e-5 || abs(a/c) < 1.0e-5 || abs(a/d) < 1.0e-5)
	// 	return solveCubic(b, c, d, e, root);

	float B = b/a, C = c/a, D = d/a, E = e/a;
	float I = C - 3.0*B*B/8.0; //f
	float J = D + B*B*B/8.0 - B*C*0.5; //g
	float K = E - 3.0*B*B*B*B/256.0 + B*B*C/16.0 - B*D/4.0;//h

	float z;
	if(solveCubic(1.0, I+I, I*I - 4.0*K, -(J*J), z))
	{
		float value = z*z*z + z*z*(I+I) + z*(I*I - 4.0*K) - J*J;

		float p = sqrt(z);
		float r = -p;
		float q = (I + z - J/p)*0.5;
		float s = (I + z + J/p)*0.5;

		// root = 10000.0;
		// vec2 r0;
		// if (analyticalQuadratic(1.0, p, q, r0))
		// {
		// 	r0 -= B/4.0;
		// 	if (r0.x >= 0.0) root = r0.x;
		// 	if (r0.y >= 0.0 && r0.y < root) root = r0.y;
		// }
		// if (analyticalQuadratic(1.0, r, s, r0))
		// {
		// 	r0 -= B/4.0;
		// 	if (r0.x >= 0.0) root = r0.x;
		// 	if (r0.y >= 0.0 && r0.y < root) root = r0.y;
		// }
		// if (root < 1000.0)
		// 	return true;

		bool foundRoot = false, foundARoot;
		float aRoot;
		foundRoot = solveQuadratic(1.0, p, q, root);
		root -= B/4.0;

		foundARoot = solveQuadratic(1.0, r, s, aRoot);
		aRoot -= B/4.0;
		if((foundRoot && foundARoot && ((aRoot < root && aRoot >= 0.0)
			|| root < 0.0)) || (!foundRoot && foundARoot))
		{
			root = aRoot;
			foundRoot = true;
		}

		foundARoot = solveQuadraticOther(1.0, p, q, aRoot);
		aRoot -= B/4.0;
		if((foundRoot && foundARoot && ((aRoot < root && aRoot >= 0.0)
			|| root < 0.0)) || (!foundRoot && foundARoot))
		{
			root = aRoot;
			foundRoot = true;
		}

		foundARoot = solveQuadraticOther(1.0, r, s, aRoot);
		aRoot -= B/4.0;
		if((foundRoot && foundARoot && ((aRoot < root && aRoot >= 0.0)
			|| root < 0.0)) || (!foundRoot && foundARoot))
		{
			root = aRoot;
			foundRoot = true;
		}
		return foundRoot;
	}
	return false;
}




float evalcubic(float x, float a, float b, float c, float d)
{
  return ((x*a+b)*x+c)*x+d;
}

float evalquad(float x, float a, float b, float c)
{
  return (x*a+b)*x+c;
}

bool quadratic(float a, float b, float c, out vec2 t)
{
	float d = b*b - 4.0*a*c;
	if (d < 0.0)
		return false;
	d = sqrt(d);
	// if (b < 0.0)
	// 	d = -d;
	t.x = 0.5 * (-b - d) / a;
	t.y = 0.5 * (-b + d) / a;
	if (t.x > t.y)
		t.xy = t.yx;
	return true;
}

float cubic(float a, float b, float c)
{
	// if (c > -1e-6)
	// {
	// 	if (b > 1e-10) return -c/b;
	// 	if (b > -1e-4) return 0.0;
	// }
	float k = 1.0;
	k = pow(-c, 1.0 / 3.0);
	a /= k; b /= k*k; c = -1.0;
	bool reciprocate = evalcubic(1.0, 1.0, a, b, c) < 0.0;
	if (reciprocate)
	{
		float a1 = -b;
		float b1 = -a;
		a = a1; b = b1;
	}
	float x0 = 0.0, x1 = 0.5, x2 = 1.0;
	for (int i = 0; i < 15; i++)
	{
		const float ep = 1e-5;
		float t = evalcubic(x1, 1.0, a, b, c);
		if (abs(t) < ep)
			break;
		x0 = (t <  0.0) ? x1 : x0;
		x2 = (t >= 0.0) ? x1 : x2;
		// if (x2 - x0 < ep)
		// 	break;
		x1 = x1 - t / evalquad(x1, 3.0, 2.0 * a, b);
		if (x1 <= x0 || x1 >= x2)
			x1 = 0.5 * (x0 + x2);
	}
	if (reciprocate)
		return k / x1;
	return k * x1;
}

int quartic(float c1, float c2, float c3, float c4, out vec4 res)
{
	float alpha = 0.5 * c1;
	float A = c2 - alpha*alpha;
	float B = c3 - alpha * A;
	float a, b, beta, ksi;
	// if (B == 0.0)
	// 	ksi = 0.0;
	// else
		ksi = cubic(2.0 * A - alpha*alpha, A*A + 2.0 * B * alpha - 4.0 * c4, -B*B);
	a = sqrt(ksi);
	beta = 0.5 * (A + ksi);
	// if (ksi == 0.0)
	// 	b = sqrt(max(beta*beta - c4, 0.0));
	// else
		b = 0.5 * a * (alpha - B / ksi);
	vec2 tmp;
	int resn = 0;
	if (quadratic(1.0, alpha + a, beta + b, tmp))
	{
		res.xy = tmp.xy;
		resn += 2;
	}
	if (quadratic(1.0, alpha - a, beta - b, tmp))
	{
		res.zw = res.xy;
		res.xy = tmp.xy;
		resn += 2;
	}
	return resn;
}
