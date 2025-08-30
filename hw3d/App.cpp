#include "App.h"
#include <sstream>
#include <iomanip>
#include "Box.h"
#include "Pyramid.h"
#include "Melon.h"
#include "Sheet.h"
#include "SkinnedBox.h"
#include <memory>
#include "ChiliMath.h"
#include <algorithm>
#include "Surface.h"
#include "GDIPlusManager.h"
#include "imgui/imgui.h"

GDIPlusManager gdipm;

App::App()
	:
	wnd(800, 600, "The Donkey Fart Box")
{
	class Factory
	{
	public:
		Factory(Graphics& gfx)
			:
			gfx(gfx)
		{}
		std::unique_ptr<Drawable> operator()()
		{
			switch (typedist(rng))
			{
			case 0:
				return std::make_unique<Pyramid>(
					gfx, rng,
					adist, ddist,
					odist, rdist
				);
			case 1:
				return std::make_unique<Box>(
					gfx, rng,
					adist, ddist,
					odist, rdist,
					bdist
				);
			case 2:
				return std::make_unique<Melon>(
					gfx, rng,
					adist, ddist,
					odist, rdist,
					longdist, latdist
				);
			case 3:
				return std::make_unique<Sheet>(
					gfx, rng,
					adist, ddist,
					odist, rdist
				);
			case 4:
				return std::make_unique<SkinnedBox>(
					gfx, rng, adist, ddist,
					odist, rdist
				);
			default:
				assert(false && "bad drawable type in factory");
				return {};
			}
		}
	private:
		Graphics& gfx;
		std::mt19937 rng{ std::random_device{}() };
		std::uniform_real_distribution<float> adist{ 0.0f, 3.1415f * 2.0f };
		std::uniform_real_distribution<float> ddist{ 0.0f, 3.1415f * 1.0f };
		std::uniform_real_distribution<float> odist{ 0.0f, 3.1415f * 0.08f };
		std::uniform_real_distribution<float> rdist{ 6.0f, 20.0f };
		std::uniform_real_distribution<float> bdist{ 0.4f, 3.0f };
		std::uniform_int_distribution<int> longdist{ 10, 40 };
		std::uniform_int_distribution<int> latdist{5, 20};
		std::uniform_int_distribution<int> typedist{ 0, 4 };
	};

	drawables.reserve(nDrawable);
	std::generate_n(std::back_inserter(drawables), nDrawable, Factory{ wnd.Gfx() });

	wnd.Gfx().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 40.0f));
}

void App::DoFrame()
{
	const auto dt = timer.Mark() * speed_factor;
	wnd.Gfx().BeginFrame(0.07f, 0.0f, 0.12f);
	for (auto& b : drawables)
	{
		b->Update(wnd.kbd.KeyIsPressed(VK_SPACE) ? 0.0f : dt);
		b->Draw(wnd.Gfx());
	}

	// 1. Simulation Speed Menu
	{
		static char buffer[1024];
		if (ImGui::Begin("Simulation Speed"))
		{
			ImGui::SliderFloat("Speed Factor", &speed_factor, 0.0f, 4.0f);
			ImGui::Text("Application average % .3f ms / frame(% .1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

			ImGui::End();
		}
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





