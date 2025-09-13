#pragma once

#include "ChiliWin.h"
#include "ChiliException.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"
#include <optional>
#include <memory>
#include "WindowsThrowMacros.h"

class Window 
{
public:
	class Exception : public ChiliException
	{
		using ChiliException::ChiliException;
	public:
		static std::string TranslateErrorCode(HRESULT hr) noexcept;
	};
	class HrException : public Exception
	{
	public:
		HrException(int line, const char* file, HRESULT hr) noexcept;
		const char* what() const noexcept override;
		virtual const char* GetType() const noexcept override;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorDescription() const noexcept;
	private:
		HRESULT hr;
	};
	class NoGfxException : public Exception 
	{
	public:
		using Exception::Exception;
		const char* GetType() const noexcept override;
	};
private:
	class WindowClass 
	{
	public:
		static const char* GetName() noexcept;
		static HINSTANCE GetInstance() noexcept;
	private:
		WindowClass() noexcept;
		~WindowClass();
		WindowClass(const WindowClass&) = delete;
		WindowClass operator=(const WindowClass&) = delete;
		static constexpr const char* wndClassName = "Chili Direct3D Engine Window";
		static WindowClass wndClass;
		HINSTANCE hInst;
	};
public:
	Window(int width, int height, const char* name);
	~Window();
	Window(const Window&) = delete;
	Window operator=(const Window&) = delete;
	void SetWindowTitle(const std::string& title);
	static std::optional<int> ProcessMessage() noexcept;
	Graphics& Gfx();
	void EnableCursor() noexcept;
	void DisableCursor() noexcept;
private:
	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT CALLBACK HandleMsg(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
private:
	void ConfineCursor() noexcept;
	void FreeCursor() noexcept;
	void HideCursor() noexcept;
	void ShowCursor() noexcept;
	void EnableImGuiMouse() noexcept;
	void DisableImGuiMouse() noexcept;
public:
	Keyboard kbd;
	Mouse mouse;
private:
	bool cursorEnabled = true;
	int width;
	int height;
	HWND hWnd;
	std::unique_ptr<Graphics> pGfx;
	std::vector<BYTE> rawBuffer;
};
