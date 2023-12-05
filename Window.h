#pragma once
#include <Windows.h>
//#include <WinUser.h>

class Window
{
public:
	Window();
	~Window();

	bool is_running();

	RECT getClientWindowRect();

	virtual void onCreate();
	virtual void onUpdate();
	virtual void onDestroy();
	virtual void onFocus();
	virtual void onKillFocus();
	
private:
	bool broadcast();

protected:
	HWND m_hwnd;
	bool m_is_running;
	bool is_initialized = false;
};

