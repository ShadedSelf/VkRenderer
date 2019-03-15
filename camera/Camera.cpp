
bool Camera::Update(float deltaTime, float mMult, float kMult)
{
	if (locked)
		return false;

	double xPos, yPos;
	glfwGetCursorPos(init->screen.window, &xPos, &yPos);
	
	double xDelta = xPos - xPrev;
	double yDelta = yPos - yPrev;

	float r = glfwGetKey(init->screen.window, GLFW_KEY_D) ? 1.0
			: glfwGetKey(init->screen.window, GLFW_KEY_A) ? -1.0
			: 0;
	float f = glfwGetKey(init->screen.window, GLFW_KEY_W) ? 1.0
			: glfwGetKey(init->screen.window, GLFW_KEY_S) ? -1.0
			: 0;
	float u = glfwGetKey(init->screen.window, GLFW_KEY_SPACE) ? 1.0
			: glfwGetKey(init->screen.window, GLFW_KEY_LEFT_SHIFT) ? -1.0
			: 0;

	if (xPrev != xPos || yPrev != yPos || length(float3(r, u, f)) > 0.5)
	{
		xPrev = xPos;
		yPrev = yPos;

		theta += xDelta * deltaTime * mMult;
		phi -= yDelta * deltaTime * mMult;

		fw = float3
		(
			sin(theta) * cos(phi),
			sin(phi),
			cos(theta) * cos(phi)
		);
		right = normalize(cross(UP, fw));
		up = normalize(cross(fw, right));

		float3 dir = (float3(sin(theta), 0, cos(theta)) * f) + (right * r) + (UP * u);
		dir = nanToZero(normalize(dir));
		pos += dir * deltaTime * kMult;

		return true;
	}
	return false;
}

CamData Camera::GetRaw()
{
	CamData res;
	res.pos   = pos.getRaw();
	res.fw    = fw.getRaw();
	res.right = right.getRaw();
	res.up    = up.getRaw();
	return res;
}

void Camera::Lock()
{
	locked = true;
}

void Camera::Unlock()
{
	locked = false;
}
