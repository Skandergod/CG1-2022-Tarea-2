


#include "Application.h"

std::unique_ptr<Application> app;

int main(int, char**)
{
	app.reset(new Application());
	app->MainLoop();
    return 0;
}
