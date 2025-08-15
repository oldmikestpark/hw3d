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

			static int i = 0;
			while (!wnd.mouse.IsEmpty()) 
			{
				const auto e = wnd.mouse.Read();
				switch (e.GetType()) 
				{
				case Mouse::Event::Type::WheelUp:
					i++;
					{
						std::ostringstream oss;
						oss << "Up: " << i;
						wnd.SetWindowTitle(oss.str());
					}
					break;
				case Mouse::Event::Type::WheelDown:
					i--;
					{
						std::ostringstream oss;
						oss << "Down: " << i;
						wnd.SetWindowTitle(oss.str());
					}
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