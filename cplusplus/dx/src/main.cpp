#define NOMINMAX
#ifndef UNICODE
#define UNICODE
#endif

#include "../include/dx.h"
#include <ctime>
#include <list>
#include <windows.h>
#include <functional>

using namespace KennyKerr;
using namespace KennyKerr::Direct2D;

Color const COLOR_BLACK(0.0f, 0.0f, 0.0f);
Color const COLOR_RED(1.0f, 0.0f, 0.0f);
Color const COLOR_GREEN(0.0f, 1.0f, 0.0f);

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

class Timer {
private:
  Wam::SimpleTimer timer;
  double lastTime;
  double targetElapsedMilliseconds;

public:

  void SetTarget(double target) {
    targetElapsedMilliseconds = target;
    lastTime = timer.GetTime();
  }

  void Tick(const std::function<void()>& tickCallback) {
    auto now = timer.GetTime();
    auto delta = now - lastTime;
      if(delta >= targetElapsedMilliseconds) {
      tickCallback();
      lastTime = now;
    }
  }
};

class Snake {
private:
  int x;
  int y;
  std::list<Point> trail;
  SolidColorBrush brush;

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

  void Initialize(DeviceContext target) {
    brush = target.CreateSolidColorBrush(COLOR_GREEN);
  }

  bool CheckCollision(int objx, int objy) {
    for (Point point : trail) {
      if (point.X == objx && point.Y == objy) {
        return true;
      }
    }
    return false;
  }

  void Update() {
    x = (x + dx + SCREEN_SIZE) % SCREEN_SIZE;
    y = (y + dy + SCREEN_SIZE) % SCREEN_SIZE;

    if (CheckCollision(x, y)) {
      x = y = 10;
      dx = dy = 0;
      tail = INITIAL_TAIL;
    }

    trail.push_back(Point(x, y));
    while ((int)trail.size() > tail) {
      trail.pop_front();
    }
  }

  void Draw(DeviceContext target) {
    // Render green filled quad
    for (Point point : trail) {
      auto left = point.X * SPRITE_SIZE + 1.f;
      auto top = point.Y * SPRITE_SIZE + 1.f;
      auto right = left + SPRITE_SIZE - 1.f;
      auto bottom = top + SPRITE_SIZE - 1.f;
      target.FillRectangle(RectF(left, top, right, bottom), brush);
    }
  }
};

class Apple {
private:
  SolidColorBrush brush;

public:
  int x;
  int y;

  Apple() {
    x = 3;
    y = 3;
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
  }

  void Initialize(DeviceContext target) {
    brush = target.CreateSolidColorBrush(COLOR_RED);
  }

  void Reposition(int newx, int newy) {
    this->x = newx;
    this->y = newy;
  }

  void Draw(DeviceContext target) {
    auto left = this->x * SPRITE_SIZE + 1.f;
    auto top = this->y * SPRITE_SIZE + 1.f;
    auto right = left + SPRITE_SIZE - 1.f;
    auto bottom = top + SPRITE_SIZE - 1.f;
    target.FillRectangle(RectF(left, top, right, bottom), brush);
  }
};

class Game {
private:
  std::unique_ptr<Snake> snake;
  std::unique_ptr<Apple> apple;
  Factory1 factory;
  DeviceContext target;
  SolidColorBrush brush;
  Dxgi::SwapChain1 swapChain;
  DirectWrite::TextLayout textLayout;

  Timer timer;

public:
  Game() : snake(std::make_unique<Snake>()), apple(std::make_unique<Apple>()) {
    factory = CreateFactory();
    //timer.SetFixedTimeStep(true);
    timer.SetTarget(1.f / FRAMES_PER_SECOND);
  }

  ~Game() { factory.Reset(); }

  void Tick() {
    timer.Tick([&]() {
      Update();
    });
    Draw();
  }

  bool Initialize(HWND window) {
    auto device = Direct3D::CreateDevice();
    target = factory.CreateDevice(device).CreateDeviceContext();
    auto dxgi = device.GetDxgiFactory();

    Dxgi::SwapChainDescription1 description;
    description.SwapEffect = Dxgi::SwapEffect::Discard;

    swapChain = dxgi.CreateSwapChainForHwnd(device, window, description);

    auto props = BitmapProperties1(
        BitmapOptions::Target | BitmapOptions::CannotDraw,
        PixelFormat(Dxgi::Format::B8G8R8A8_UNORM, AlphaMode::Ignore));

    target.SetTarget(target.CreateBitmapFromDxgiSurface(swapChain, props));

    auto const dpi = factory.GetDesktopDpi();
    target.SetDpi(dpi, dpi);

    apple->Initialize(target);
    snake->Initialize(target);

    return true;
  }

  bool HandleInput(WPARAM key) {
    switch (key) {
    case VK_LEFT:
      if (snake->dx == 0) {
        snake->dx = -1;
        snake->dy = 0;
      }
      return false;

    case VK_RIGHT:
      if (snake->dx == 0) {
        snake->dx = 1;
        snake->dy = 0;
      }
      return false;

    case VK_UP:
      if (snake->dy == 0) {
        snake->dx = 0;
        snake->dy = -1;
      }
      return false;

    case VK_DOWN:
      if (snake->dy == 0) {
        snake->dx = 0;
        snake->dy = 1;
      }
      return false;

    case VK_ESCAPE:
      return true;

    default:
      return false;
    }
  }

  void Update() {
    snake->Update();
    if (snake->CheckCollision(apple->x, apple->y)) {
      snake->tail++;
      int x = 0;
      int y = 0;
      do {
        x = std::rand() % SCREEN_SIZE;
        y = std::rand() % SCREEN_SIZE;
      } while (snake->CheckCollision(x, y));
      apple->Reposition(x, y);
    }
  }

  void Draw() {
    // Clear the window
    target.BeginDraw();

    target.Clear(COLOR_BLACK);

    apple->Draw(target);
    snake->Draw(target);

    target.EndDraw();

    auto const hr = swapChain.Present();
  }
};

int main(int, char **) {
  ComInitialize com;

  // CreateDeviceIndependentResources();

  WNDCLASS wc = {};
  wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
  // wc.hInstance = module;
  wc.lpszClassName = L"window";
  wc.style = CS_HREDRAW | CS_VREDRAW;

  wc.lpfnWndProc = [](HWND window, UINT message, WPARAM wparam,
                      LPARAM lparam) -> LRESULT {
    auto game =
        reinterpret_cast<Game *>(GetWindowLongPtr(window, GWLP_USERDATA));

    switch (message) {
    case WM_PAINT:
      PAINTSTRUCT ps;
      VERIFY(BeginPaint(window, &ps));
      EndPaint(window, &ps);
      return 0;

    case WM_DESTROY:
      PostQuitMessage(0);
      return 0;

    case WM_KEYDOWN:
      if(game->HandleInput(wparam)) {
        PostQuitMessage(0);
      }
      return 0;

    default:
      return DefWindowProc(window, message, wparam, lparam);
    }
  };

  RegisterClass(&wc);

  RECT rect = {0, 0, static_cast<LONG>(SCREEN_WIDTH),
               static_cast<LONG>(SCREEN_HEIGHT)};

  AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

  auto width = rect.right - rect.left;
  auto height = rect.bottom - rect.top;

  auto window = CreateWindow(
      wc.lpszClassName, L"Snake written in C++ with dx.h",
      WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, width,
      height, nullptr, nullptr, nullptr, nullptr);

  auto game = std::make_unique<Game>();
  SetWindowLongPtr(window, GWLP_USERDATA,
                   reinterpret_cast<LONG_PTR>(game.get()));

  if (game->Initialize(window)) {
    MSG message = {};
    while (WM_QUIT != message.message) {
      if (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE)) {
        TranslateMessage(&message);
        DispatchMessage(&message);
      } else {
        game->Tick();
      }
    }
  }
  // ReleaseDevice();
  game.reset();
}