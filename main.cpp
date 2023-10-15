#include "AppWindow.h"

int main()
{
	AppWindow app;
	if (app.init())
	{
		while (app.is_running())
		{
			app.broadcast();
		}
	}

	return 0;
}