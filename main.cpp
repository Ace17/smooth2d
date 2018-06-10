#include <chrono>
#include <cstdio>
#include "SDL.h"

using namespace std;

SDL_Renderer* renderer;

auto const GAME_PERIOD_IN_US = 16000; // 60 Hz

auto const BAR_SPEED = 16;

struct World
{
  int pos = 0;
  int vel = 0;

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

void drawScreen(int deltaTime)
{
  SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
  SDL_RenderFillRect(renderer, nullptr);

  {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect rect {};
    rect.x = 10 + world.pos;
    rect.y = 10;
    rect.w = 100;
    rect.h = 1000;
    SDL_RenderFillRect(renderer, &rect);
  }


  {
    SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
    SDL_Rect rect {};
    rect.x = 10;
    rect.y = 10;
    rect.w = 10;
    rect.h = 10 + deltaTime * 10;
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

    int updateCount = (deltaTimeInUs + remainder) / (GAME_PERIOD_IN_US);
    remainder = (deltaTimeInUs + remainder) % (GAME_PERIOD_IN_US);

    for(int k=0;k < updateCount;++k)
      world.tick();

    drawScreen(deltaTimeInUs/1000);
    printf("%.2f ms\n", deltaTimeInUs/1000.0);

    lastTime = now;
  }

  SDL_DestroyWindow(wnd);
  SDL_Quit();

  return 0;
}

