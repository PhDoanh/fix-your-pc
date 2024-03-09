#include "Game.hpp"

int main(int argc, char *argv[])
{
    std::srand(std::time(NULL));
    Game FixYourPC;
    FixYourPC.start();

    return 0;
}