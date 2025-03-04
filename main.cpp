#include "src/Managers/GameManager.h"

int main()
{
    ETG::GameManager GM{};

    while (ETG::GameManager::IsRunning())
    {
        //Track every Frame 
        GM.ProcessEvents();

        //If the window unfocused, sleep the thread
        if (!GM.WindowHasFocus())
        {
            sf::sleep(sf::milliseconds(10));
        }
            
        GM.Update();
        GM.Draw();
    }

    return 0;
}
