#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <stdio.h>

#pragma comment(lib, "SDL2.lib")

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;
const int TILE_SIZE = 32;
const int SPEED = 5;

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
SDL_Texture* mapTexture = nullptr;
SDL_Texture* playerTexture = nullptr;
SDL_Rect playerRect = { 100, 100, TILE_SIZE, TILE_SIZE };

bool Init();
void HandleInput(bool& isRunning);
void Update();
void Render();
void Cleanup();

int main(int argc, char* argv[])
{
    if (!Init()) return -1;

    bool isRunning = true;
    while (isRunning) {
        HandleInput(isRunning);
        Update();
        Render();
        SDL_Delay(16); // 60 FPS
    }

    Cleanup();
    return 0;
}

bool Init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return false;
    }

    window = SDL_CreateWindow("Walking town",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);

    if (!window) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        return false;
    }

    // 맵 타일 이미지 불러오기 (나중에)
    SDL_Surface* mapSurface = SDL_LoadBMP("assets/map.bmp");
    mapTexture = SDL_CreateTextureFromSurface(renderer, mapSurface);
    SDL_FreeSurface(mapSurface);

    // 캐릭터 이미지 불러오기
    SDL_Surface* playerSurface = SDL_LoadBMP("assets/player.bmp");
    playerTexture = SDL_CreateTextureFromSurface(renderer, playerSurface);
    SDL_FreeSurface(playerSurface);

    return true;
}
void HandleInput(bool& isRunning) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT)
            isRunning = false;
    }

    const Uint8* keys = SDL_GetKeyboardState(NULL);

    if (keys[SDL_SCANCODE_LEFT])  playerRect.x -= SPEED;
    if (keys[SDL_SCANCODE_RIGHT]) playerRect.x += SPEED;
    if (keys[SDL_SCANCODE_UP])    playerRect.y -= SPEED;
    if (keys[SDL_SCANCODE_DOWN])  playerRect.y += SPEED;
}

void Render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // 배경 검정
    SDL_RenderClear(renderer);

    // 맵 그리기
    SDL_RenderCopy(renderer, mapTexture, NULL, NULL);

    // 플레이어 그리기
    SDL_RenderCopy(renderer, playerTexture, NULL, &playerRect);

    SDL_RenderPresent(renderer);
}

void Cleanup() {
    SDL_DestroyTexture(mapTexture);
    SDL_DestroyTexture(playerTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

