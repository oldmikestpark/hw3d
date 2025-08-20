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
	while (const auto e = wnd.mouse.Read()) 
	{
		switch (e->GetType()) 
		{
		case Mouse::Event::Type::Move:
			std::ostringstream oss;
			oss << "Mouse moved to: (" << e->GetPosX() << ", " << e->GetPosY() << ")";
			wnd.SetWindowTitle(oss.str());
			break;
		}
	}
	const float c = sin(timer.Peek()) / 2.0f + 0.5f;
	wnd.Gfx().ClearBuffer(c, c, 1.0f);
	wnd.Gfx().DrawTestTriangle();
	wnd.Gfx().EndFrame();
}

