#define NOMINMAX
#ifndef UNICODE
#define UNICODE
#endif

// #include <cstdlib>
#include <ctime>
// #include <iostream>
#include <list>
// #include <memory>
// #include <string>

#include "../include/dx.h"

using namespace KennyKerr;
using namespace KennyKerr::Direct2D;

Color const COLOR_WHITE(1.0f, 1.0f, 1.0f);
Color const COLOR_BLUE(0.26f, 0.56f, 0.87f);

static Factory1 factory;
static DeviceContext target;
static SolidColorBrush brush;
static Dxgi::SwapChain1 swapChain;
static DirectWrite::TextLayout textLayout;

const int SPRITE_SIZE = 20;
const int SCREEN_SIZE = 20;
const int FRAMES_PER_SECOND = 15;
const int INITIAL_TAIL = 5;

const int SCREEN_WIDTH = SPRITE_SIZE * SCREEN_SIZE;
const int SCREEN_HEIGHT = SPRITE_SIZE * SCREEN_SIZE;

struct Point
{
  public:
    int X;
    int Y;

    Point(int x, int y) : X(x), Y(y) {}
};

class Snake
{
  private:
    int x;
    int y;
    std::list<Point> trail;

  public:
    int tail;
    int dx;
    int dy;

    Snake()
    {
        trail = std::list<Point>();
        tail = INITIAL_TAIL;
        x = 10;
        y = 10;
        dx = 0;
        dy = 1;
    }

    bool checkCollision(int x, int y)
    {
        for (Point point : trail)
        {
            if (point.X == x && point.Y == y)
            {
                return true;
            }
        }
        return false;
    }

    void update()
    {
        x = (x + dx + SCREEN_SIZE) % SCREEN_SIZE;
        y = (y + dy + SCREEN_SIZE) % SCREEN_SIZE;

        if (checkCollision(x, y))
        {
            x = y = 10;
            dx = dy = 0;
            tail = INITIAL_TAIL;
        }

        trail.push_back(Point(x, y));
        while ((int)trail.size() > tail)
        {
            trail.pop_front();
        }
    }

    void draw()
    {
        // Render green filled quad
        for (Point point : trail)
        {
        }
    }
};

class Apple
{
  public:
    int x;
    int y;

    Apple()
    {
        x = 3;
        y = 3;
        std::srand(std::time(nullptr));
    }

    void reposition(Snake *snake)
    {
        do
        {
            x = std::rand() % SCREEN_SIZE;
            y = std::rand() % SCREEN_SIZE;
        } while (snake->checkCollision(x, y));
    }

    void draw()
    {
        // Render red filled quad
    }
};

class Game
{
  private:
    Snake *snake;
    Apple *apple;

  public:
    Game() : snake(new Snake()), apple(new Apple()) {}

    ~Game()
    {
        if (apple)
            delete (apple);
        if (snake)
            delete (snake);
    }

    void Tick()
    {
        Update();
        Draw();
    }

    bool Initialize() { return true; }

    bool HandleInput() { return true; }

    void Update()
    {
        snake->update();
        if (snake->checkCollision(apple->x, apple->y))
        {
            snake->tail++;
            apple->reposition(snake);
        }
    }

    void Draw()
    {
        apple->draw();
        snake->draw();
    }
};

int main(int, char **)
{
    ComInitialize com;

    factory = CreateFactory();
    // CreateDeviceIndependentResources();

    WNDCLASS wc = {};
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    // wc.hInstance = module;
    wc.lpszClassName = L"window";
    wc.style = CS_HREDRAW | CS_VREDRAW;

    wc.lpfnWndProc = [](HWND window, UINT message, WPARAM wparam,
                        LPARAM lparam) -> LRESULT {
        if (WM_PAINT == message)
        {
            PAINTSTRUCT ps;
            VERIFY(BeginPaint(window, &ps));
            // Render(window);
            EndPaint(window, &ps);
            return 0;
        }

        if (WM_SIZE == message)
        {
            if (target && SIZE_MINIMIZED != wparam)
            {
                // ResizeSwapChainBitmap();
                // Render(window);
            }

            return 0;
        }

        if (WM_DISPLAYCHANGE == message)
        {
            // Render(window);
            return 0;
        }

        if (WM_DESTROY == message)
        {
            PostQuitMessage(0);
            return 0;
        }

        return DefWindowProc(window, message, wparam, lparam);
    };

    RegisterClass(&wc);

    CreateWindow(wc.lpszClassName, L"dx.codeplex.com",
                 WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT,
                 CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr, nullptr, nullptr);

    MSG message;
    BOOL result;

    while (result = GetMessage(&message, 0, 0, 0))
    {
        if (-1 != result)
            DispatchMessage(&message);
    }

    // ReleaseDevice();
    factory.Reset();
}