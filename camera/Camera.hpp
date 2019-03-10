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
			this->pos = pos;

			glfwGetCursorPos(init->screen.window, &xPrev, &yPrev);
		}

		bool Update(float deltaTime, float mMult, float kMult);
		CamData GetRaw();
};

#include <Camera.cpp>

#endif
