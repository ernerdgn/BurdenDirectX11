#pragma once
#include <Windows.h>
//#include <WinUser.h>

class Window
{
public:
	Window();
	~Window();

	bool init();
	bool broadcast();
	bool is_running();
	bool release();

	RECT getClientWindowRect();
	void setHWND(HWND hwnd);

	virtual void onCreate() = 0;
	virtual void onUpdate() = 0;
	virtual void onDestroy();

protected:
	HWND m_hwnd;
	bool m_is_running;
};

