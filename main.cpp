#include "SDL.h"

SDL_Renderer* renderer;

auto const SPEED = 16;

struct World
{
  int pos = 0;
  int vel = 0;

  void update()
  {
    if(pos > 1900)
      vel = -SPEED;
    if(pos < 10)
      vel = SPEED;
    pos += vel;
  }
};

World world;

void drawScreen(int deltaTime)
{
  SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
  SDL_RenderFillRect(renderer, nullptr);

  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_Rect rect {};
  rect.x = 10 + world.pos;
  rect.y = 10;
  rect.w = 100;
  rect.h = 1000;
  SDL_RenderFillRect(renderer, &rect);


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

  auto time = SDL_GetTicks();

  for(int i=0;i < 200;++i)
  {
    auto const deltaTime = SDL_GetTicks() - time;
    time += deltaTime;

    world.update();

    drawScreen(deltaTime);
  }

  SDL_DestroyWindow(wnd);
  SDL_Quit();

  return 0;
}

