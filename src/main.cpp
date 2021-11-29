#include "Game.h"
#include "Estado.h"

int main()
{
	auto* g = Game::Instance();

    g->setState(Estado::StateType::MENU_STATE);
    g->runGame();

    g = nullptr;

    return 0;
}

