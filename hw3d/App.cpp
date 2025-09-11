#include "App.h"
#include <sstream>
#include <iomanip>
#include "Box.h"
#include "Cylinder.h"
#include "Pyramid.h"
#include "SkinnedBox.h"
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

	const auto transform = dx::XMMatrixRotationRollPitchYaw(pos.pitch, pos.yaw, pos.roll) *
		dx::XMMatrixTranslation(pos.x, pos.y, pos.z);
	nano.Draw(wnd.Gfx(), transform);

	light.Draw(wnd.Gfx());

	// imgui stuff
	{
		// 1.model menu
		ShowModelWindow();

		// 2. camera menu
		cam.SpawnControlWindow();

		// 3. light menu
		light.SpawnControlWindow();
	}

	wnd.Gfx().EndFrame();
}

void App::ShowModelWindow()
{
	if (ImGui::Begin("Model")) 
	{
		using namespace std::string_literals;

		ImGui::Text("Orientation");
		ImGui::SliderAngle("Roll", &pos.roll, -180.0f, 180.0f);
		ImGui::SliderAngle("Pitch", &pos.pitch, -180.0f, 180.0f);
		ImGui::SliderAngle("Yaw", &pos.yaw, -180.0f, 180.0f);

		ImGui::Text("Position");
		ImGui::SliderFloat("X", &pos.x, -20.0f, 20.0f);
		ImGui::SliderFloat("Y", &pos.y, -20.0f, 20.0f);
		ImGui::SliderFloat("Z", &pos.z, -20.0f, 20.0f);

		if (ImGui::Button("Reset")) 
		{
			Reset();
		}
	}
	ImGui::End();
}

void App::Reset()
{
	pos.pitch = 0.0f;
	pos.yaw = 0.0f;
	pos.roll = 0.0f;
	pos.x = 0.0f;
	pos.y = 0.0f;
	pos.z = 0.0f;
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





