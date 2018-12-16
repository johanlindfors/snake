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
#include <windows.h>
#include "../include/dx.h"
#include "../include/DeviceResources.h"
#include "../include/StepTimer.h"

#include <DirectXColors.h>

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

struct Point {
  public:
    int X;
    int Y;

    Point(int x, int y) : X(x), Y(y) {}
};

class Snake {
  private:
    int x;
    int y;
    std::list<Point> trail;

  public:
    int tail;
    int dx;
    int dy;

    Snake() {
        trail = std::list<Point>();
        tail = INITIAL_TAIL;
        x = 10;
        y = 10;
        dx = 0;
        dy = 1;
    }

    bool checkCollision(int x, int y) {
        for (Point point : trail) {
            if (point.X == x && point.Y == y) {
                return true;
            }
        }
        return false;
    }

    void update() {
        x = (x + dx + SCREEN_SIZE) % SCREEN_SIZE;
        y = (y + dy + SCREEN_SIZE) % SCREEN_SIZE;

        if (checkCollision(x, y)) {
            x = y = 10;
            dx = dy = 0;
            tail = INITIAL_TAIL;
        }

        trail.push_back(Point(x, y));
        while ((int)trail.size() > tail) {
            trail.pop_front();
        }
    }

    void draw() {
        // Render green filled quad
        for (Point point : trail) {
        }
    }
};

class Apple {
  public:
    int x;
    int y;

    Apple() {
        x = 3;
        y = 3;
        std::srand(std::time(nullptr));
    }

    void reposition(int x, int y) {
        this->x = x;
        this->y = y;
    }

    void draw() {
        // Render red filled quad
    }
};

class Game {
  private:
    std::unique_ptr<Snake> snake;
    std::unique_ptr<Apple> apple;
    std::unique_ptr<DX::DeviceResources> deviceResources;
    DX::StepTimer timer;

  public:
    Game() 
        : snake(std::make_unique<Snake>())
        , apple(std::make_unique<Apple>()) {
            deviceResources = std::make_unique<DX::DeviceResources>();
        }

    ~Game() { }

    void Tick() {
        timer.Tick([&]()
        {
            Update();
        });
        Draw();
    }

    bool Initialize(HWND window, int width, int height) {
        deviceResources->SetWindow(window, width, height);

        deviceResources->CreateDeviceResources();
        //CreateDeviceDependentResources();

        deviceResources->CreateWindowSizeDependentResources();
        //CreateWindowSizeDependentResources();

        return true; 
    }

    bool HandleInput() { return true; }

    void Update() {
        snake->update();
        if (snake->checkCollision(apple->x, apple->y)) {
            snake->tail++;
            int x = 0;
            int y = 0;
            do {
                x = std::rand() % SCREEN_SIZE;
                y = std::rand() % SCREEN_SIZE;
            } while (snake->checkCollision(x, y));
            apple->reposition(x, y);
        }
    }

    void Draw() {
        // Clear the window
        auto context = deviceResources->GetD3DDeviceContext();
        auto renderTarget = deviceResources->GetRenderTargetView();
        auto depthStencil = deviceResources->GetDepthStencilView();

        context->ClearRenderTargetView(renderTarget, DirectX::Colors::Black);
        context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
        context->OMSetRenderTargets(1, &renderTarget, depthStencil);

        // Set the viewport.
        auto viewport = deviceResources->GetScreenViewport();
        context->RSSetViewports(1, &viewport);

        apple->draw();
        snake->draw();

        deviceResources->Present();
    }
};

int main(int, char **) {
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

    RECT rect = { 0, 0, static_cast<LONG>(SCREEN_WIDTH), static_cast<LONG>(SCREEN_HEIGHT) };

    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

    auto width = rect.right - rect.left;
    auto height = rect.bottom - rect.top;

    auto window = CreateWindow(wc.lpszClassName, L"Snake written in C++ with dx.h",
                 WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT,
                 width, height, nullptr, nullptr, nullptr, nullptr);

    auto game = std::make_unique<Game>();
    if(game->Initialize(window, SCREEN_WIDTH, SCREEN_HEIGHT)) {
        MSG message = {};
        while (WM_QUIT != message.message)
        {
            if (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE))
            {
                TranslateMessage(&message);
                DispatchMessage(&message);
            }
            else
            {
                game->Tick();
            }
        }
    }
    // ReleaseDevice();
    factory.Reset();
    game.reset();
}