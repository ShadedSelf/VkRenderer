#ifndef CUT
	#define CUT

// Updateme
vec2 cut(vec2 form, vec2 cutter, vec3 formN, vec3 cutterInN, vec3 cutterOutN, out vec3 n)
{
	vec2 res = form;
	n = formN;

	if (isinf(form.x))
		return vec2(INFINITY);
	if (isinf(cutter.x) || cutter.x < 0.0)
		return form;

	// Both outside
	if (cutter.y > 0.0 && form.y > 0.0)
	{
		if (cutter.x < form.x && cutter.y > form.x)
		{
			res.x = (cutter.y > form.y) ? INFINITY : cutter.y;
			n = -cutterInN;
		}
		return res;
	}

	// Both inside
	if (cutter.y < 0.0 && form.y < 0.0)
	{
		if (cutter.x < form.x)
		{
			res.x = cutter.x;
			res.y = form.x;
			n = -cutterOutN;
			return res;
		}
		return vec2(INFINITY);
	}

	// Inside cutter
	if (cutter.y < 0.0 && form.y > 0.0)
	{
		if (form.x > cutter.x)
			return res;
		if (cutter.x < form.y)
		{
			res.x = cutter.x;
			n = -cutterOutN;
			return res;
		}
		return vec2(INFINITY);
	}

	// Inside form
	if (cutter.y > 0.0 && form.y < 0.0)
	{
		if (cutter.x < form.x)
		{
			res.x = cutter.x;
			n = cutterOutN;
		}
	}
	return res;
}

#endif
