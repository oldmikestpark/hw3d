#pragma once

#include "Window.h"
#include "ChiliTimer.h"
#include "ImguiManager.h"
#include "Camera.h"
#include "PointLight.h"
#include "Model.h"
#include <set>

class App 
{
public:
	App();
	int Go();
	~App();
private:
	void DoFrame();
	void ShowModelWindow();
	void Reset();
private:
	Camera cam;
	ImguiManager imgui;
	Window wnd;
	ChiliTimer timer;
	PointLight light = PointLight(wnd.Gfx());
	float speed_factor = 1.0f;
	Model nano{ wnd.Gfx(), "Models\\nanosuit.obj" };
	struct
	{
		float pitch = 0.0f;
		float yaw = 0.0f;
		float roll = 0.0f;
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
	}pos;
};
