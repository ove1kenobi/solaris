#include "Engine.h"

Engine::Engine() : m_Running {true}
{

}

const bool Engine::Initialize()
{
	return true;
}

void Engine::Run()
{
    while (m_Running)
    {
        MSG message = {};
        while (PeekMessageA(&message, nullptr, 0u, 0u, PM_REMOVE))
        {
            TranslateMessage(&message);
            DispatchMessageA(&message);
            if (message.message == WM_QUIT)
            {
                m_Running = false;            //Could use future event system to stop running as an event instead (Emil F);    
            }
        }

        //Here we will empty our own event queue:
        /*EventHandler.ProcessEvents() or something */

        //Regular Update:
        //Update();

        //Render:
        //Render();
    }
}