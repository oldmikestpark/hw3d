#include "App.h"
#include <sstream>
#include <iomanip>
#include "AssTest.h"
#include <memory>
#include "ChiliMath.h"
#include <algorithm>
#include "Surface.h"
#include "GDIPlusManager.h"
#include "imgui/imgui.h"
#include "Vertex.h"

GDIPlusManager gdipm;
namespace dx = DirectX;
App::App()
	:
	wnd(800, 600, "The Donkey Fart Box")
{
	wnd.Gfx().SetProjection(dx::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 40.0f));
}

void App::DoFrame()
{
	const auto dt = timer.Mark() * speed_factor;
	wnd.Gfx().BeginFrame(0.07f, 0.0f, 0.12f);
	wnd.Gfx().SetCamera(cam.GetMatrix());
	light.Bind(wnd.Gfx(), cam.GetMatrix());

	
	nano.Draw(wnd.Gfx());

	light.Draw(wnd.Gfx());

	// imgui stuff
	{
		// 1.model menu
		nano.ShowWindow();

		// 2. camera menu
		cam.SpawnControlWindow();

		// 3. light menu
		light.SpawnControlWindow();
	}

	wnd.Gfx().EndFrame();
}

App::~App()
{
}

int App::Go()
{
	while (true) 
	{
		if (const auto ecode = Window::ProcessMessage()) 
		{
			return *ecode;
		}
		DoFrame();
	}
}





