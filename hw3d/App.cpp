#include "App.h"
#include <sstream>
#include <iomanip>

App::App()
	:
	wnd(800, 600, "The Donkey Fart Box")
{}

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

void App::DoFrame()
{
	static std::string title;
	while (const auto e = wnd.kbd.ReadKey()) 
	{
		if (e->IsPressed() && e->GetCode() == VK_BACK) 
		{
			title.clear();
			wnd.SetWindowTitle(title);
		}
	}
	while (const auto c = wnd.kbd.ReadChar()) 
	{
		if (*c != 0x8) 
		{
			title += *c;
			wnd.SetWindowTitle(title);
		}
	}
	const float c = sin(timer.Peek()) / 2.0f + 0.5f;
	wnd.Gfx().ClearBuffer(c, c, 1.0f);
	wnd.Gfx().DrawTestTriangle();
	wnd.Gfx().EndFrame();
}

