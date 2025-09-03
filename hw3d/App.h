#pragma once

#include "Window.h"
#include "ChiliTimer.h"
#include "ImguiManager.h"
#include "Camera.h"
#include "PointLight.h"
#include <set>

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
	std::vector<class Box*> boxes;
	PointLight light = PointLight(wnd.Gfx());
	float speed_factor = 1.0f;
	static constexpr size_t nDrawable = 180;
	std::optional<int> comboBoxIndex;
	std::set<int> boxControlIds;
};
