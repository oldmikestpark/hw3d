#pragma once

#include "Window.h"
#include "ChiliTimer.h"
#include "ImguiManager.h"
#include "Camera.h"
#include "PointLight.h"
#include "Mesh.h"
#include "TestPlane.h"
#include <set>

class App 
{
public:
	App();
	int Go();
	~App();
private:
	void DoFrame();
	void ShowRawInputWindow();
private:
	int x = 0, y = 0;
	Camera cam;
	ImguiManager imgui;
	Window wnd;
	ChiliTimer timer;
	PointLight light = PointLight(wnd.Gfx());
	float speed_factor = 1.0f;
	Model wall{wnd.Gfx(), "Models\\brick_wall\\brick_wall.obj"};
	TestPlane tp{wnd.Gfx(), 1.0f};
	// Model nano{ wnd.Gfx(), "Models\\nano_textured\\nanosuit.obj" };
};
