#include <bits/stdc++.h>
#include "../inc/SDL.h"
#include "Console.hpp"
#include "GameInit.hpp"
#include "GameLoop.hpp"

int main(int argc, char *argv[])
{
    std::srand(std::time(NULL));
    GameInit game_init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    GameLoop game_loop;
    game_loop.start();

    return 0;
}