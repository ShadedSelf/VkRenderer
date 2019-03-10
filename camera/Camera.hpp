#ifndef CAMERA
	#define CAMERA

#include <float3.hpp>

struct CamData
{
	float3_raw pos;
	float3_raw fw;
	float3_raw right;
	float3_raw up;
};

class Camera
{
	private:
		Initter *init;

		double xPrev, yPrev;
		float theta = 0;
		float phi = 0;

	public:
			float3 pos;
			float3 fw = FW;
			float3 right = RIGHT;
			float3 up = UP;
		
		Camera(Initter *init, float3 pos = float3())
		{
			this->init = init;

			glfwGetCursorPos(init->screen.window, &xPrev, &yPrev);
			this->pos = pos;
		}

		bool Update(float deltaTime, float mMult, float kMult)
		{
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
				// dir = nanToZero(normalize(dir));
				pos += dir * kMult;

				return true;
			}
			return false;
		}

		CamData Get()
		{
			CamData res;
			res.pos   = pos.getRaw();
			res.fw    = fw.getRaw();
			res.right = right.getRaw();
			res.up    = up.getRaw();
			return res;
		}
};

#endif
