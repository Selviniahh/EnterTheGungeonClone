#include "Managers/GameManager.h"

int main()
{
    ETG::GameManager GM;
    GM.Initialize();

    while (ETG::GameManager::IsRunning())
    {
        //If window is not focused freeze the game  
        if (!ETG::Globals::Window->hasFocus()) continue;
        
        GM.Update();
        GM.Draw();        
    }
    
    return 0;
}
