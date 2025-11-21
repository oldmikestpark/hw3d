#include "App.h"
#include <sstream>
#include <iomanip>
#include <memory>
#include "ChiliMath.h"
#include <algorithm>
#include "Surface.h"
#include "imgui/imgui.h"
#include "VertexBuffer.h"
#include <string>
#include "ChiliUtil.h"
#include "DynamicConstant.h"

namespace dx = DirectX;
App::App(const std::string& commandLine)
	:
	commandLine(commandLine),
	wnd(1920, 1080, "The Donkey Fart Box"),
	scriptCommander(TokenizeQuoted(commandLine)),
	light(wnd.Gfx())
{
	auto ps = std::make_shared<Dcb::Struct>(0);
	Dcb::Struct& s = *ps;
	s.Add<Dcb::Struct>("butts");
	s["butts"].Add<Dcb::Float3>("dicks");
	s["butts"].Add<Dcb::Float>("pube");
	Dcb::Buffer b(std::move(ps));
	auto ref_d{ b["butts"]["dicks"] };
	ref_d = DirectX::XMFLOAT3{69.0f, 0.0f, 0.0f};
	auto ref_p{b["butts"]["pube"]};
	ref_p = 420.0f;
	DirectX::XMFLOAT3 v{ref_d};
	float v_f{ref_p};
	auto& ff{v};
	auto& fff{v_f};
	
	s.Add<Dcb::Float>("woot");
	s.Add<Dcb::Array>("arr");
	s["arr"].Set<Dcb::Float>(4);

	//wall.SetRootTransform(dx::XMMatrixTranslation(-12.0f, 0.0f, 0.0f));
	//tp.SetPos({12.0f, 0.0f, 0.0f});
	//gobber.SetRootTransform(DirectX::XMMatrixTranslation(0.0f, 0.0f, -4.0f));
	//wall.SetRootTransform(DirectX::XMMatrixTranslation(0.0f, -7.0f, 6.0f));
	bluePlane.SetPos(cam.GetPos());
	redPlane.SetPos(cam.GetPos());

	wnd.Gfx().SetProjection(dx::XMMatrixPerspectiveLH(1.0f, 9.0f / 16.0f, 0.5f, 400.0f));
}

void App::DoFrame()
{
	const auto dt = timer.Mark() * speed_factor;
	wnd.Gfx().BeginFrame(0.07f, 0.0f, 0.12f);
	wnd.Gfx().SetCamera(cam.GetMatrix());
	light.Bind(wnd.Gfx(), cam.GetMatrix());
	
	// nano.Draw(wnd.Gfx());
	// wall.Draw(wnd.Gfx());
	// tp.Draw(wnd.Gfx());
	// gobber.Draw(wnd.Gfx());

	light.Draw(wnd.Gfx());
	sponza.Draw(wnd.Gfx());
	bluePlane.Draw(wnd.Gfx());
	redPlane.Draw(wnd.Gfx());

	while (const auto e = wnd.kbd.ReadKey()) 
	{
		if (!e->IsPressed()) 
		{
			continue;
		}
		switch (e->GetCode()) 
		{
		case VK_ESCAPE:
			if (wnd.CursorEnabled())
			{
				wnd.DisableCursor();
				wnd.mouse.EnableRaw();
			}
			else
			{
				wnd.EnableCursor();
				wnd.mouse.DisableRaw();
			}
			break;
		}
	}

	if (!wnd.CursorEnabled()) 
	{
		if (wnd.kbd.KeyIsPressed('W')) 
		{
			cam.Translate({0.0f, 0.0f, dt});
		}
		if (wnd.kbd.KeyIsPressed('A')) 
		{
			cam.Translate({-dt, 0.0f, 0.0f});
		}
		if (wnd.kbd.KeyIsPressed('S')) 
		{
			cam.Translate({0.0f, 0.0f, -dt});
		}
		if (wnd.kbd.KeyIsPressed('D')) 
		{
			cam.Translate({dt, 0.0f, 0.0f});
		}
		if (wnd.kbd.KeyIsPressed('R')) 
		{
			cam.Translate({ 0.0f, dt, 0.0f });
		}
		if (wnd.kbd.KeyIsPressed('F')) 
		{
			cam.Translate({0.0f, -dt, 0.0f});
		}
	}

	while (const auto delta = wnd.mouse.ReadRawDelta()) 
	{
		if (!wnd.CursorEnabled()) 
		{
			cam.Rotate((float)delta->x, (float)delta->y);
		}
	}

	// imgui stuff
	{
		// 1.model menu
		// nano.ShowWindow(wnd.Gfx(), "Nano");

		// 2. camera menu
		cam.SpawnControlWindow();

		// 3. light menu
		light.SpawnControlWindow();

		// 4. raw input menu
		ShowRawInputWindow();

		// 5. block wall plane menu
		// tp.SpawnControlWindow(wnd.Gfx());

		// 6. wall menu
		// wall.ShowWindow(wnd.Gfx(), "Wall");

		// 7. gobber menu
		// gobber.ShowWindow(wnd.Gfx(), "gobber");

		// 8. Sponza menu
		sponza.ShowWindow(wnd.Gfx(), "Sponza");

		bluePlane.SpawnControlWindow(wnd.Gfx(), "Blue Plane");
		redPlane.SpawnControlWindow(wnd.Gfx(), "Red Plane");
	}

	wnd.Gfx().EndFrame();
}

void App::ShowRawInputWindow()
{
	while (const auto d = wnd.mouse.ReadRawDelta()) 
	{
		x += d->x;
		y += d->y;
	}
	if (ImGui::Begin("Raw Input")) 
	{
		ImGui::Text("Tally : (&d,%d)", x, y);
		ImGui::Text("Cursor: %s", wnd.CursorEnabled() ? "Enabled" : "Disabled");
	}
	ImGui::End();
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





