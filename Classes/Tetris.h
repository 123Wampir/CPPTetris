#include <iostream>
#include <Windows.h>
#include <stdlib.h>

class Cell
{
public:
    Cell();
    Cell(int x, int y);
    int x;
    int y;

private:
};

Cell::Cell()
{
    x = {};
    y = {};
}
Cell::Cell(int x, int y)
{
    this->x = x;
    this->y = y;
}

class Block : public Cell
{
public:
    bool empty;
    Block();
    Block(int x, int y);
    Block(Cell *cell);
};
Block::Block()
{
    empty = true;
}
Block::Block(int x, int y)
{
    empty = true;
    this->x = x;
    this->y = y;
}
Block::Block(Cell *cell)
{
    empty = false;
    x = cell->x;
    y = cell->y;
}
class Figure
{
private:
public:
    int width;
    int heigth;
    int x;
    int y;
    Figure();
    void Rotate();
    void Move(int dx, int dy);
    Cell cells[4];
};
Figure::Figure()
{
    srand(time(0));
    int type = rand() % 7;
    x = 0;
    y = 0;
    width = 0;
    heigth = 0;
    switch (type)
    {
    case 0:
        cells[0] = Cell{0, 0};
        cells[1] = Cell{1, 0}; //.##
        cells[2] = Cell{1, 1}; //..##
        cells[3] = Cell{2, 1};
        width = 3;
        heigth = 2;
        break;
    case 1:
        cells[0] = Cell{0, 1};
        cells[1] = Cell{1, 1}; //..##
        cells[2] = Cell{1, 0}; //.##
        cells[3] = Cell{2, 0};
        width = 3;
        heigth = 2;
        break;
    case 2:
        cells[0] = Cell{0, 0};
        cells[1] = Cell{1, 0}; //.###
        cells[2] = Cell{2, 0}; //...#
        cells[3] = Cell{2, 1};
        width = 3;
        heigth = 2;
        break;
    case 3:
        cells[0] = Cell{0, 1};
        cells[1] = Cell{1, 1}; //...#
        cells[2] = Cell{2, 1}; //.###
        cells[3] = Cell{2, 0};
        width = 3;
        heigth = 2;
        break;
    case 4:
        cells[0] = Cell{0, 1};
        cells[1] = Cell{1, 1}; //..#
        cells[2] = Cell{2, 1}; //.###
        cells[3] = Cell{1, 0};
        width = 3;
        heigth = 2;
        break;
    case 5:
        cells[0] = Cell{0, 0};
        cells[1] = Cell{0, 1}; //.##
        cells[2] = Cell{1, 0}; //.##
        cells[3] = Cell{1, 1};
        width = 2;
        heigth = 2;
        break;
    case 6:
        cells[0] = Cell{0, 0};
        cells[1] = Cell{1, 0}; //.####
        cells[2] = Cell{2, 0};
        cells[3] = Cell{3, 0};
        width = 4;
        heigth = 1;
        break;
    default:
        break;
    }
}

void Figure::Move(int dx, int dy)
{
    x += dx;
    y += dy;
}

void Figure::Rotate()
{
    int temp = width;
    width = heigth;
    heigth = temp;
    for (int i = 0; i < 4; i++)
    {
        temp = cells[i].x;
        cells[i].x = cells[i].y;
        cells[i].y = temp;
        cells[i].x = (width - 1 - cells[i].x) % width;
    }
}

class TetrisGame
{
public:
    bool end;
    int score;
    int width, heigth;
    Figure fig;
    Block **grid;
    TetrisGame();
    void GameLoop();
    void KeyPress();
    void Draw();
    void Destroy();
    void Collide();
    void DestroyLines();
};

TetrisGame::TetrisGame()
{
    score = 0;
    width = 10;
    heigth = 25;
    fig = {};
    grid = new Block *[heigth];
    for (int i = 0; i < heigth; i++)
        grid[i] = new Block[width];
    for (int i = 0; i < heigth; i++)
    {
        for (int j = 0; j < width; j++)
        {
            grid[i][j] = Block(j, i);
        }
    }
}

void TetrisGame::Collide()
{
    bool newFig = false;
    for (int i = 0; i < 4; i++)
    {
        if (fig.cells[i].y + fig.y + 1 >= 0 && fig.cells[i].y + fig.y + 1 < heigth && fig.cells[i].x + fig.x >= 0 && fig.cells[i].x + fig.x < width)
            if (!grid[fig.cells[i].y + fig.y + 1][fig.cells[i].x + fig.x].empty)
            {
                newFig = true;
                break;
            }
        if (fig.cells[i].y + fig.y == heigth - 1)
        {
            newFig = true;
            break;
        }
    }
    if (newFig)
    {
        for (int i = 0; i < 4; i++)
        {
            int x = fig.cells[i].x + fig.x;
            int y = fig.cells[i].y + fig.y;
            grid[y][x] = Block(&fig.cells[i]);
        }
        fig = Figure{};
        score += 10;
    }
}

void TetrisGame::DestroyLines()
{
    for (int i = heigth - 1; i >= 0; i--)
    {
        bool empty = false;
        for (int j = 0; j < width; j++)
        {
            if (grid[i][j].empty)
            {
                empty = true;
                break;
            }
        }
        if (!empty)
        {
            score += 100;
            for (int k = i; k >= 0; k--)
                for (int j = 0; j < width; j++)
                {
                    if (k > 0)
                        grid[k][j] = grid[k - 1][j];
                }
        }
    }
}

void TetrisGame::Draw()
{
    system("cls");
    for (int i = -1; i <= heigth; i++)
    {
        for (int j = -1; j <= width; j++)
        {
            if (i == -1 || i == heigth || j == -1 || j == width)
            {
                if (i == -1)
                {
                    if (j == -1)
                        std::cout << char(201) << char(205);
                    else if (j == width)
                        std::cout << char(205) << char(187);
                    else
                        std::cout << char(205) << char(205);
                }
                else if (i == heigth)
                {
                    if (j == -1)
                        std::cout << char(200) << char(205);
                    else if (j == width)
                        std::cout << char(205) << char(188);
                    else
                        std::cout << char(205) << char(205);
                }
                else if (j == -1)
                    std::cout << char(186) << " ";
                else
                    std::cout << " " << char(186);
            }
            else
            {

                bool empty = true;
                for (int n = 0; n < 4; n++)
                {
                    if (fig.cells[n].x + fig.x == j && fig.cells[n].y + fig.y == i)
                    {
                        std::cout << "[]";
                        empty = false;
                        break;
                    }
                }
                if (empty)
                {
                    if (grid[i][j].empty)
                        std::cout << "  ";
                    else
                        std::cout << "OO";
                }
            }
        }
        std::cout << std::endl;
    }
}
void TetrisGame::GameLoop()
{
    std::ios::sync_with_stdio(false);
    std::cout.tie(NULL);
    int i = 0;
    while (!end)
    {
        Sleep(100);
        if (i % 2 == 0)
        {
            i = i % 2;
            fig.Move(0, 1);
        }
        Collide();
        KeyPress();
        DestroyLines();
        Draw();
        i++;
        printf("Score: %d\n", score);
    }
}
void TetrisGame::KeyPress()
{
    if (GetKeyState(VK_LEFT) & 0x8000)
    {
        if (fig.x - 1 >= 0)
        {
            bool empty = true;
            for (int i = 0; i < 4; i++)
            {
                if (!grid[fig.y + fig.cells[i].y][fig.x + fig.cells[i].x - 1].empty)
                    empty = false;
            }
            if (empty)
                fig.Move(-1, 0);
        }
    }
    else if (GetKeyState(VK_RIGHT) & 0x8000)
    {
        if (fig.x + fig.width < width)
        {
            bool empty = true;
            for (int i = 0; i < 4; i++)
            {
                if (!grid[fig.y + fig.cells[i].y][fig.x + fig.cells[i].x + 1].empty)
                    empty = false;
            }
            if (empty)
                fig.Move(1, 0);
        }
    }
    else if (GetKeyState(VK_UP) & 0x8000)
    {
        fig.Rotate();
    }
    else if (GetKeyState(VK_DOWN) & 0x8000)
    {
        fig.Move(0, 1);
        Collide();
    }
    else if (GetKeyState(VK_ESCAPE) & 0x8000)
    {
        end = true;
    }
}

void TetrisGame::Destroy()
{
    delete[] grid;
}