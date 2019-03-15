
#define PI acos(-1.0)

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
