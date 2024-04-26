#include "Game.hpp"

int main(int argc, char *argv[])
{
    std::srand(std::time(nullptr));
    Game *game = new Game("Fix Your PC");
    float frame_start, frame_time, frame_delay = 1000.0 / game->fps;

    game->initSDL2();
    game->loadMedia();

    while (game->running)
    {
        frame_start = float(SDL_GetTicks64());

        game->handleEvent();
        game->updateScreen();

        frame_time = float(SDL_GetTicks64()) - frame_start;
        if (frame_delay > frame_time)
            SDL_Delay(frame_delay - frame_time);
    }

    game->quitMedia();
    game->quitSDL2();

    delete game;
    game = nullptr;
    return 0;
}