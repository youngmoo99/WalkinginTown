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
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

int tileMap2[MAP_HEIGHT][MAP_WIDTH] = { 
    {0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,1},
    {0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,1,0,0,0,1},
    {0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,0,1},
    {0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,0,1},
    {0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,0,1},
    {0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,0,1},
    {0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,0,1},
    {0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,0,1},
    {0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,0,1},
    {0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,0,1},
    {0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,0,1},
    {0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,0,1},
    {0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,0,1},
    {0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,2,0,1},
    {0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
}; //Ÿ�� �迭 0 : �̵����� , 1: �� , 2: ��Ż

int cameraX = 0; //ī�޶� x
int cameraY = 0; //ī�޶� y
int (*currentMap)[MAP_WIDTH] = tileMap1; // �ʱ� Ÿ�� 

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
SDL_Texture* mapTexture = nullptr;
SDL_Texture* playerTexture = nullptr;
SDL_Rect playerRect = { 0 * TILE_SIZE, 0 * TILE_SIZE, TILE_SIZE, TILE_SIZE };

struct Enemy {
    SDL_Rect rect;
    int speed = 5;
};

Enemy enemy = { {0,0, TILE_SIZE, TILE_SIZE} };

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

    //�� Ÿ�� �̹��� �ҷ����� 
    SDL_Surface* mapSurface = SDL_LoadBMP("assets/map_tiles.bmp");
    mapTexture = SDL_CreateTextureFromSurface(renderer, mapSurface);
    SDL_FreeSurface(mapSurface);

    //ĳ���� �̹��� �ҷ����� 
    SDL_Surface* playerSurface = SDL_LoadBMP("assets/player.bmp"); // bmp������ �о� SDL_Surface�� ��ȯ 
    playerTexture = SDL_CreateTextureFromSurface(renderer, playerSurface); // SDL_SURFACE�� GPU �ؽ�ó�� ��ȯ
    SDL_FreeSurface(playerSurface); // ������ ���� Surface �޸� ����

    return true;

}

bool IsCollding(SDL_Rect a, SDL_Rect b) {
    return SDL_HasIntersection(&a, &b);
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


    //Ÿ�ϸ� ������ 
    RenderMap(mapTexture);

    //�� ������ 
    SDL_SetRenderDrawColor(renderer, 150, 75, 0, 0);
    SDL_Rect screenEnemy = enemy.rect;
    screenEnemy.x -= cameraX;
    screenEnemy.y -= cameraY;
    SDL_RenderFillRect(renderer, &screenEnemy);

    //�÷��̾� ������
    //�÷��̾� ��ġ ���� 
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

//�� ������
void RenderMap(SDL_Texture* tileTexture) {
    SDL_Rect src, dest;
    src.w = dest.w = TILE_SIZE; //Ÿ�ϼ� �̹��� �ڸ���
    src.h = dest.h = TILE_SIZE;

    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            int tile = currentMap[y][x];
            src.x = tile * TILE_SIZE;
            src.y = 0;

            dest.x = x * TILE_SIZE - cameraX; // ȭ�鿡 �׸� ��ġ
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

bool CanMoveTo(int x, int y) {
    //���� �簢�� �� ������ ��� �˻� 
    return
        IsWalkable(x, y) &&
        IsWalkable(x + TILE_SIZE - 1, y) &&
        IsWalkable(x, y + TILE_SIZE - 1) &&
        IsWalkable(x + TILE_SIZE - 1, y + TILE_SIZE - 1);
}

//�浹 �˻� �� �̵� ���
void TryMovePlayer(int dx, int dy) {
    int nextX = playerRect.x + dx;
    int nextY = playerRect.y + dy;

    //ĳ���� �簢�� 4�� ������ �˻� 
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

void MoveEnemyTowardsPlayer() {
    int dx = playerRect.x - enemy.rect.x;
    int dy = playerRect.y - enemy.rect.y;

    int moveX = 0, moveY = 0;

    //x�Ǵ� y�� �� �չ��� �켱
    if (abs(dx) > abs(dy)) {
        moveX = (dx > 0) ? enemy.speed : -enemy.speed;
        if (!CanMoveTo(enemy.rect.x + moveX, enemy.rect.y)) moveX = 0;

        if (moveX == 0) { //x�� ������ y�� �õ�
            moveY = (dy > 0) ? enemy.speed : -enemy.speed;
            if (!CanMoveTo(enemy.rect.x, enemy.rect.y + moveY)) moveY = 0;

        }
    }
    else {
        moveY = (dy > 0) ? enemy.speed : -enemy.speed;
        if (!CanMoveTo(enemy.rect.x, enemy.rect.y +moveY)) moveY = 0;

        if (moveY == 0) {
            moveX = (dx > 0) ? enemy.speed : -enemy.speed;
            if (!CanMoveTo(enemy.rect.x + moveX, enemy.rect.y)) moveX = 0;
        }
    }

    enemy.rect.x += moveX;
    enemy.rect.y += moveY;
}

void Update() {
    MoveEnemyTowardsPlayer();

    //�÷��̾� �� �浹 �� ���� 
    if (IsCollding(playerRect, enemy.rect)) {
        currentMap = tileMap1; //1����� ���� 
        playerRect.x = 7 * TILE_SIZE;
        playerRect.y = 0 * TILE_SIZE;
        enemy.rect.x = 0;
        enemy.rect.y = 0;
        printf("���� �浹! ������ ���µ˴ϴ�.");
    }

    //ī�޶� ��ġ ��� 
    cameraX = playerRect.x - WINDOW_WIDTH / 2 + TILE_SIZE / 2;
    cameraY = playerRect.y - WINDOW_HEIGHT / 2 + TILE_SIZE / 2;

    //ī�޶� �� �ٱ����� �������� ���� 
    if (cameraX < 0) cameraX = 0;
    if (cameraY < 0) cameraY = 0;

    int maxCameraX = MAP_WIDTH * TILE_SIZE - WINDOW_WIDTH;
    int maxCameraY = MAP_HEIGHT * TILE_SIZE - WINDOW_HEIGHT;

    if (cameraX > maxCameraX) cameraX = maxCameraX;
    if (cameraY > maxCameraY) cameraY = maxCameraY;

    //�� ��ȯ ���� 
    int tileX = playerRect.x / TILE_SIZE;
    int tileY = playerRect.y / TILE_SIZE;

    if (currentMap == tileMap1 && currentMap[tileY][tileX] == 2) {
        currentMap = tileMap2;
        playerRect.x = 0 * TILE_SIZE;
        playerRect.y = 7 * TILE_SIZE;
        enemy.rect.x = 0;
        enemy.rect.y = 0;
        printf("�������� 2 �̵�");
    }
    else if (currentMap == tileMap2 && currentMap[tileY][tileX] == 2) {
        currentMap = tileMap1;
        playerRect.x = 7 * TILE_SIZE;
        playerRect.y = 0 * TILE_SIZE;
        enemy.rect.x = 0;
        enemy.rect.y = 0;
        printf("�������� 1 �̵�");
    }
}