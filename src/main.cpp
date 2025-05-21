#include "Application.h"
int main()
{
    // Parse window config settings
	WindowSettings windowSettings = ParseWindowConfig(Paths::WindowConfig);

    Application application = Application(windowSettings);
    application.Run();
}
