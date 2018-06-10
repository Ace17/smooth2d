#include <chrono>
#include <cstdio>
#include "SDL.h"

using namespace std;

SDL_Renderer* renderer;

auto const GAME_PERIOD_IN_US = 16000; // 60 Hz

auto const BAR_SPEED = 6.9;

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
int g_x = 0;

void drawScreen(int remainder)
{
  SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
  SDL_RenderFillRect(renderer, nullptr);

  {
    auto extrapolatedPos = (world.pos + (world.vel * remainder) / GAME_PERIOD_IN_US);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect rect {};
    rect.x = 10 + extrapolatedPos;
    g_x = rect.x;
    rect.y = 10;
    rect.w = 100;
    rect.h = 1000;
    SDL_RenderFillRect(renderer, &rect);
  }


  SDL_RenderPresent(renderer);
}

int main()
{
  SDL_InitSubSystem(SDL_INIT_VIDEO);

  uint32_t flags = 0;
  flags |= SDL_RENDERER_ACCELERATED;
  flags |= SDL_RENDERER_PRESENTVSYNC;
  auto wnd = SDL_CreateWindow("Smooth", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP);
  renderer = SDL_CreateRenderer(wnd, -1, flags);

  auto lastTime = chrono::system_clock::now();
  int remainder = 0;

  for(int i=0;i < 200;++i)
  {
    auto const now = chrono::system_clock::now();
    auto const deltaTimeInUs = int(chrono::duration<double>(now - lastTime).count() * 1000 * 1000);

    remainder += deltaTimeInUs;

    while(remainder > GAME_PERIOD_IN_US)
    {
      world.tick();
      remainder -= GAME_PERIOD_IN_US;
    }

    drawScreen(remainder);
    printf("%d, %d, %.2f ms\n", i, g_x, deltaTimeInUs/1000.0);

    lastTime = now;
  }

  SDL_DestroyWindow(wnd);
  SDL_Quit();

  return 0;
}

