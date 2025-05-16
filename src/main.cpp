#include "Application.h"

int main()
{
    // Read window settings
	WindowSettings windowSettings = ParseWindowConfig(Paths::WindowConfig);

    Application application = Application(windowSettings);
    application.Run();
}
