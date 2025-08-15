#include "Window.h"
#include <sstream>

int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR	  lpCmdLine,
	int       nCmdShow)
{
	try
	{
		Window wnd(800, 300, "Donkey Fart Box");

		// message pump
		MSG msg;
		BOOL gResult;

		while ((gResult = GetMessage(&msg, nullptr, 0, 0)) > 0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			while (!wnd.mouse.IsEmpty()) 
			{
				const auto e = wnd.mouse.Read();
				switch (e.GetType()) 
				{
				case Mouse::Event::Type::Leave:
					wnd.SetWindowTitle("Gone!!!!");
					break;
				case Mouse::Event::Type::Move:
					std::ostringstream oss;
					oss << "Mouse moved to (" << e.GetPosX() << ", " << e.GetPosY() << ")";
					wnd.SetWindowTitle(oss.str());
					break;
				}
			}
		}

		if (gResult == -1)
		{
			return -1;
		}
		else
		{
			return (int)msg.wParam;
		}
	}
	catch (const ChiliException& e) 
	{
		MessageBox(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
	}
	catch (const std::exception& e) 
	{
		MessageBox(nullptr, e.what(), "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	catch (...) 
	{
		MessageBox(nullptr, "No details available", "Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	return -1;
}