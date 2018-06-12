///////////////////////////////////////////////////////////////////////////////
// Game logic: runs at fixed tick rate (independent from display)
///////////////////////////////////////////////////////////////////////////////

auto const GAME_HERTZ = 10;
auto const GAME_PERIOD_IN_US = (1000 * 1000) / GAME_HERTZ;

auto const BAR_SPEED = 30;

struct World
{
  double pos = 0;
  double vel = 0;

  void tick()
  {
    if(pos > 1900)
      vel = -BAR_SPEED;
    if(pos < 10)
      vel = BAR_SPEED;
    pos += vel;
  }
};

World world;

///////////////////////////////////////////////////////////////////////////////
// Display: draw frames at a fixed rate (screen rate)
///////////////////////////////////////////////////////////////////////////////

#include "SDL.h"
SDL_Renderer* renderer;
SDL_Window* wnd;

// bar screen position, as last drawn (i.e potentially extrapolated)
int g_x = 0;

void initRenderer()
{
  SDL_InitSubSystem(SDL_INIT_VIDEO);

  uint32_t flags = 0;
  flags |= SDL_RENDERER_ACCELERATED;
  flags |= SDL_RENDERER_PRESENTVSYNC;
  wnd = SDL_CreateWindow("Smooth", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP);
  renderer = SDL_CreateRenderer(wnd, -1, flags);
}

void destroyRenderer()
{
  SDL_DestroyWindow(wnd);
  SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

void drawScreen(int remainder)
{
  SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
  SDL_RenderFillRect(renderer, nullptr);

  {
    auto extrapolatedPos = (world.pos + (world.vel * remainder) / GAME_PERIOD_IN_US);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect rect {};
    if(1)
      rect.x = 10 + extrapolatedPos;
    else
      rect.x = 10 + world.pos;
    g_x = rect.x;
    rect.y = 10;
    rect.w = 100;
    rect.h = 1000;
    SDL_RenderFillRect(renderer, &rect);
  }


  SDL_RenderPresent(renderer);
}

///////////////////////////////////////////////////////////////////////////////
// Main loop
///////////////////////////////////////////////////////////////////////////////

#include <chrono>
#include <cstdio>

using namespace std;

auto timeNow()
{
  return chrono::system_clock::now();
}

int main()
{
  initRenderer();

  auto const firstTime = timeNow();
  auto lastTime = firstTime;
  int remainder = 100 * 1000;

  for(int i=0;;++i)
  {
    auto const now = timeNow();

    if(chrono::duration<double>(now - firstTime).count() > 3)
      break;

    auto const deltaTimeInUs = int(chrono::duration<double>(now - lastTime).count() * 1000 * 1000);
    bool dirty = false;

    remainder += deltaTimeInUs;

    while(remainder > GAME_PERIOD_IN_US)
    {
      world.tick();
      remainder -= GAME_PERIOD_IN_US;
      dirty = true;
    }

    auto const prev_x = g_x;
    drawScreen(remainder);
    printf("%d, %d, %d, %d, %.2f ms\n", i, g_x, g_x - prev_x, dirty, deltaTimeInUs/1000.0);

    lastTime = now;
  }

  destroyRenderer();

  return 0;
}

