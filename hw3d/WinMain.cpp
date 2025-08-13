#include "Window.h"

int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR	  lpCmdLine,
	int       nCmdShow)
{
	Window wnd(800, 300, "Donkey Fart Box");
	Window wnd2(200, 650, "Donkey Fart Box2");

	// message pump
	MSG msg;
	BOOL gResult;

	while ((gResult = GetMessage(&msg, nullptr, 0, 0)) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
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