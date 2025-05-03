#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <stdio.h>
#include <filesystem>
#pragma comment(lib, "SDL2.lib")

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480; 
const int TILE_SIZE = 32;
const int MAP_WIDTH = 50;
const int MAP_HEIGHT = 30;
const int SPEED = 5;

int tileMap1[MAP_HEIGHT][MAP_WIDTH] = {
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};

int tileMap2[MAP_HEIGHT][MAP_WIDTH] = { 
    {0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0},
    {0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,0},
    {0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,0},
    {0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,0},
    {0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,0},
    {0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,0},
    {0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,0},
    {0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,0},
    {0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,0},
    {0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,0},
    {0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,0},
    {0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,0},
    {0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,0},
    {0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,2}
}; //타일 배열 0 : 이동가능 , 1: 벽 , 2: 포탈

int cameraX = 0; //카메라 x
int cameraY = 0; //카메라 y
int (*currentMap)[MAP_WIDTH] = tileMap1; // 초기 타일 

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
SDL_Texture* mapTexture = nullptr;
SDL_Texture* playerTexture = nullptr;
SDL_Rect playerRect = { 0 * TILE_SIZE, 0 * TILE_SIZE, TILE_SIZE, TILE_SIZE };

bool Init();
void HandleInput(bool& isRunning);
void Update();
void Render();
void Cleanup();
void RenderMap(SDL_Texture* tileTexture);
void TryMovePlayer(int dx, int dy);

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

    //맵 타일 이미지 불러오기 
    SDL_Surface* mapSurface = SDL_LoadBMP("assets/map_tiles.bmp");
    mapTexture = SDL_CreateTextureFromSurface(renderer, mapSurface);
    SDL_FreeSurface(mapSurface);

    //캐릭터 이미지 불러오기 
    SDL_Surface* playerSurface = SDL_LoadBMP("assets/player.bmp"); // bmp파일을 읽어 SDL_Surface로 반환 
    playerTexture = SDL_CreateTextureFromSurface(renderer, playerSurface); // SDL_SURFACE를 GPU 텍스처로 변환
    SDL_FreeSurface(playerSurface); // 쓸일이 없는 Surface 메모리 해제

    return true;

}

void HandleInput(bool& isRunning) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT)
            isRunning = false;
    }

    const Uint8* keys = SDL_GetKeyboardState(NULL);

    if (keys[SDL_SCANCODE_LEFT]) TryMovePlayer(-SPEED, 0);
    if (keys[SDL_SCANCODE_RIGHT]) TryMovePlayer(SPEED, 0);
    if (keys[SDL_SCANCODE_UP]) TryMovePlayer(0, -SPEED);
    if (keys[SDL_SCANCODE_DOWN]) TryMovePlayer(0, SPEED);

}

void Render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    //타일맵 렌더링 
    RenderMap(mapTexture);

    //플레이어 랜더링
    //플레이어 위치 보정 
    SDL_Rect screenPlayer = playerRect;
    screenPlayer.x -= cameraX;
    screenPlayer.y -= cameraY;

    SDL_RenderCopy(renderer, playerTexture, NULL, &screenPlayer);
    SDL_RenderPresent(renderer);

}

void Cleanup() {
    SDL_DestroyTexture(mapTexture);
    SDL_DestroyTexture(playerTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

//맵 랜더링
void RenderMap(SDL_Texture* tileTexture) {
    SDL_Rect src, dest;
    src.w = dest.w = TILE_SIZE; //타일셋 이미지 자르기
    src.h = dest.h = TILE_SIZE;

    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            int tile = currentMap[y][x];
            src.x = tile * TILE_SIZE;
            src.y = 0;

            dest.x = x * TILE_SIZE - cameraX; // 화면에 그릴 위치
            dest.y = y * TILE_SIZE - cameraY;

            SDL_RenderCopy(renderer, tileTexture, &src, &dest);
        }
    }
}

bool IsWalkable(int x, int y) {
    int tileX = x / TILE_SIZE;
    int tileY = y / TILE_SIZE;

    if (tileX < 0 || tileY < 0 || tileX >= MAP_WIDTH || tileY >= MAP_HEIGHT) return false;

    return currentMap[tileY][tileX] == 0 || currentMap[tileY][tileX] == 2;
}

//충돌 검사 후 이동 허용
void TryMovePlayer(int dx, int dy) {
    int nextX = playerRect.x + dx;
    int nextY = playerRect.y + dy;

    //캐릭터 사각형 4개 꼭짓점 검사 
    if (
        IsWalkable(nextX, nextY) &&
        IsWalkable(nextX + TILE_SIZE - 1, nextY) &&
        IsWalkable(nextX, nextY + TILE_SIZE - 1) &&
        IsWalkable(nextX + TILE_SIZE - 1, nextY + TILE_SIZE - 1)
        ) {
        playerRect.x = nextX;
        playerRect.y = nextY;
    }
}

void Update() {
    //카메라 위치 계산 
    cameraX = playerRect.x - WINDOW_WIDTH / 2 + TILE_SIZE / 2;
    cameraY = playerRect.y - WINDOW_HEIGHT / 2 + TILE_SIZE / 2;

    //카메라가 맨 바깥으로 못나가게 고정 
    if (cameraX < 0) cameraX = 0;
    if (cameraY < 0) cameraY = 0;

    int maxCameraX = MAP_WIDTH * TILE_SIZE - WINDOW_WIDTH;
    int maxCameraY = MAP_HEIGHT * TILE_SIZE - WINDOW_HEIGHT;

    if (cameraX > maxCameraX) cameraX = maxCameraX;
    if (cameraY > maxCameraY) cameraY = maxCameraY;

    //맵 전환 조건 
    int tileX = playerRect.x / TILE_SIZE;
    int tileY = playerRect.y / TILE_SIZE;

    if (currentMap == tileMap1 && currentMap[tileY][tileX] == 2) {
        currentMap = tileMap2;
        playerRect.x = 0 * TILE_SIZE;
        playerRect.y = 0 * TILE_SIZE;
        printf("스테이지 2 이동");
    }
    else if (currentMap == tileMap2 && currentMap[tileY][tileX] == 2) {
        currentMap = tileMap1;
        playerRect.x = 0 * TILE_SIZE;
        playerRect.y = 0 * TILE_SIZE;
        printf("스테이지 1 이동");
    }
}