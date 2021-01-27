//TODO: Move includes from EntryPoint to pch (Emil F) 
#include <crtdbg.h>
#include "Engine.h"

int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPreviousInstance, 
					 _In_ LPSTR lpCommandLine, _In_ int nCmdShow)
{
	#if defined(DEBUG) || defined (_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	#endif
	Engine engine;
	if (engine.Initialize())
	{
		engine.Run();
	}
	return 0;
}