#include <iostream>
#include <Tetris.h>
int main(void)
{
    TetrisGame game{};
    game.GameLoop();
    game.Destroy();
    char a;
    std::cin >> a;
}