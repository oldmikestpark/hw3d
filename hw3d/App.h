#pragma once

#include "Window.h"
#include "ChiliTimer.h"
#include "ImguiManager.h"
#include "Camera.h"

class App 
{
public:
	App();
	int Go();
	~App();
private:
	void DoFrame();
private:
	Camera cam;
	ImguiManager imgui;
	Window wnd;
	ChiliTimer timer;
	std::vector<std::unique_ptr<class Drawable>> drawables;
	std::vector<std::unique_ptr<class Melon>> melons;
	float speed_factor = 1.0f;
	static constexpr size_t nDrawable = 180;
};
