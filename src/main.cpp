#include "Application.h"
#include "core/Log.h"

// Main code
int main(int, char **)
{
	Log::Init();
	LOG_TRACE("Trace message");
	LOG_DEBUG("Debug message");
	LOG_INFO("Info message");
	LOG_WARN("Warn message");
	LOG_ERROR("Error message");
	LOG_CRITICAL("Critical message");

	Application app;
	app.Run();
}
