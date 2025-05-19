#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <stdio.h>
#include <filesystem>
#include <queue>
#include <unordered_map>
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

enum GameState {
    MAIN_MENU,
    IN_GAME
};
GameState gameState = MAIN_MENU;

SDL_Texture* titleTexture = nullptr;
SDL_Texture* startButtonTexture = nullptr;
SDL_Rect startButtonRect = { 220, 300, 200, 100 }; // ��ư ��ġ�� ũ��


SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
SDL_Texture* mapTexture = nullptr;
SDL_Texture* playerTexture = nullptr;
SDL_Rect playerRect = { 0 * TILE_SIZE, 0 * TILE_SIZE, TILE_SIZE, TILE_SIZE };

struct Enemy {
    SDL_Rect rect;
    int speed = 5;
    int targetX;
    int targetY;
};

Enemy enemy = { {0,0, TILE_SIZE, TILE_SIZE}, 5, 0, 0};

bool Init();
void HandleInput(bool& isRunning);
void Update();
void Render();
void Cleanup();
void RenderMap(SDL_Texture* tileTexture);
void TryMovePlayer(int dx, int dy);

struct Node
{
    int x, y;
    int gCost, hCost;
    Node* parent;

    Node(int x, int y, int gCost, int hCost, Node* parent = nullptr)
        : x(x), y(y), gCost(gCost), hCost(hCost), parent(parent) {}

    int getFCost() const {
        return gCost + hCost;
    }

    //��� �񱳸� ���� ������ �����ε� 
    bool operator<(const Node& other) const {
        return getFCost() > other.getFCost();
    }

};
bool IsWalkable(int x, int y) {
    int tileX = x / TILE_SIZE;
    int tileY = y / TILE_SIZE;

    if (tileX < 0 || tileY < 0 || tileX >= MAP_WIDTH || tileY >= MAP_HEIGHT) return false;

    int tileType = currentMap[tileY][tileX];
    return tileType == 0 || tileType == 2;
}

std::vector<Node> FindPath(int startX, int startY, int endX, int endY) {
    std::priority_queue<Node> openList;
    std::unordered_map<int, Node*> closedList;

    Node* startNode = new Node(startX, startY, 0, abs(endX - startX) + abs(endY - startY));
    openList.push(*startNode);

    std::vector<Node> path;

    while (!openList.empty()) {
        Node current = openList.top();
        openList.pop();

        // �������� �������� ��
        if (current.x == endX && current.y == endY) {
            Node* node = &current;
            while (node != nullptr) {
                path.push_back(*node);
                node = node->parent;
            }
            std::reverse(path.begin(), path.end());

            // �޸� ����
            for (auto& pair : closedList) {
                delete pair.second;
            }

            return path;
        }

        // �̹� �湮�� ����̸� ����
        int nodeKey = current.y * MAP_WIDTH + current.x;
        if (closedList.count(nodeKey)) continue;
        closedList[nodeKey] = new Node(current);

        // �� �������� Ȯ��
        int dx[] = { 1, -1, 0, 0 };
        int dy[] = { 0, 0, 1, -1 };

        for (int i = 0; i < 4; i++) {
            int nx = current.x + dx[i];
            int ny = current.y + dy[i];

            if (IsWalkable(nx * TILE_SIZE, ny * TILE_SIZE)) {
                int gCost = current.gCost + 1;
                int hCost = abs(endX - nx) + abs(endY - ny);
                Node* parentNode = closedList[nodeKey];
                Node neighbor(nx, ny, gCost, hCost, parentNode);

                openList.push(neighbor);
            }
        }
    }

    // ��θ� ã�� ���� ��� �� ���� ��ȯ
    return path;
}

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

    window = SDL_CreateWindow("Run Away",
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

    // Ÿ��Ʋ �̹��� �ε�
    SDL_Surface* titleSurface = SDL_LoadBMP("assets/title.bmp");
    titleTexture = SDL_CreateTextureFromSurface(renderer, titleSurface);
    SDL_FreeSurface(titleSurface);

    // ���� ��ư �̹��� �ε�
    SDL_Surface* startButtonSurface = SDL_LoadBMP("assets/start_button.bmp");
    startButtonTexture = SDL_CreateTextureFromSurface(renderer, startButtonSurface);
    SDL_FreeSurface(startButtonSurface);



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

        // ���� �޴����� ��ư Ŭ�� ó��
        if (gameState == MAIN_MENU && event.type == SDL_MOUSEBUTTONDOWN) {
            int mouseX = event.button.x;
            int mouseY = event.button.y;

            // START ��ư Ŭ�� ����
            if (mouseX >= startButtonRect.x && mouseX <= startButtonRect.x + startButtonRect.w &&
                mouseY >= startButtonRect.y && mouseY <= startButtonRect.y + startButtonRect.h) {
                gameState = IN_GAME;
                printf("���� ����!\n");
            }
        }
    }

    // ���� �� �÷��̾� �̵�
    if (gameState == IN_GAME) {
        const Uint8* keys = SDL_GetKeyboardState(NULL);
        if (keys[SDL_SCANCODE_LEFT]) TryMovePlayer(-SPEED, 0);
        if (keys[SDL_SCANCODE_RIGHT]) TryMovePlayer(SPEED, 0);
        if (keys[SDL_SCANCODE_UP]) TryMovePlayer(0, -SPEED);
        if (keys[SDL_SCANCODE_DOWN]) TryMovePlayer(0, SPEED);
    }
}


void RenderMainMenu() {
    // ��� �� ����
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Ÿ��Ʋ ������
    SDL_Rect titleRect = { 120, 100, 400, 150 };
    SDL_RenderCopy(renderer, titleTexture, NULL, &titleRect);

    // START ��ư ������
    SDL_RenderCopy(renderer, startButtonTexture, NULL, &startButtonRect);

    SDL_RenderPresent(renderer);
}


void Render() {
    if (gameState == MAIN_MENU) {
        RenderMainMenu();
    }
    else if (gameState == IN_GAME) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Ÿ�ϸ� ������
        RenderMap(mapTexture);

        // �� ������
        SDL_SetRenderDrawColor(renderer, 150, 75, 0, 255);
        SDL_Rect screenEnemy = enemy.rect;
        screenEnemy.x -= cameraX;
        screenEnemy.y -= cameraY;
        SDL_RenderFillRect(renderer, &screenEnemy);

        // �÷��̾� ������
        SDL_Rect screenPlayer = playerRect;
        screenPlayer.x -= cameraX;
        screenPlayer.y -= cameraY;
        SDL_RenderCopy(renderer, playerTexture, NULL, &screenPlayer);

        SDL_RenderPresent(renderer);
    }

}

void Cleanup() {
    SDL_DestroyTexture(mapTexture);
    SDL_DestroyTexture(playerTexture);
    SDL_DestroyTexture(titleTexture);
    SDL_DestroyTexture(startButtonTexture);
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
    //���� ���� Ÿ�� ��ǥ
    int enemyTileX = enemy.rect.x / TILE_SIZE;
    int enemyTileY = enemy.rect.y / TILE_SIZE;
    int playerTileX = playerRect.x / TILE_SIZE;
    int playerTileY = playerRect.y / TILE_SIZE;

    // ��θ� ���� ã�� �ʾҰų� ��ǥ�� ������ ��� ���ο� ��� ã��
    if (enemy.rect.x == enemy.targetX && enemy.rect.y == enemy.targetY) {
        std::vector<Node> path = FindPath(enemyTileX, enemyTileY, playerTileX, playerTileY);

        // ��ȿ�� ��ΰ� ������ ���� ��ǥ ��带 ����
        if (path.size() > 1) {
            Node nextStep = path[1];
            enemy.targetX = nextStep.x * TILE_SIZE;
            enemy.targetY = nextStep.y * TILE_SIZE;
        }
    }

    // ���� ��ǥ �������� �����̵��� ���� (�ӵ� ����)
    if (enemy.rect.x < enemy.targetX) enemy.rect.x += std::min(enemy.speed, enemy.targetX - enemy.rect.x);
    if (enemy.rect.x > enemy.targetX) enemy.rect.x -= std::min(enemy.speed, enemy.rect.x - enemy.targetX);
    if (enemy.rect.y < enemy.targetY) enemy.rect.y += std::min(enemy.speed, enemy.targetY - enemy.rect.y);
    if (enemy.rect.y > enemy.targetY) enemy.rect.y -= std::min(enemy.speed, enemy.rect.y - enemy.targetY);
}

void Update() {
    if (gameState == IN_GAME) {
        MoveEnemyTowardsPlayer();

        // �÷��̾�� �� �浹 �� ����
        if (IsCollding(playerRect, enemy.rect)) {
            currentMap = tileMap1; // 1����� ����
            playerRect.x = 7 * TILE_SIZE;
            playerRect.y = 0 * TILE_SIZE;
            enemy.rect.x = 0;
            enemy.rect.y = 0;
            enemy.targetX = 0;
            enemy.targetY = 0;
            printf("���� �浹! ������ ���µ˴ϴ�.\n");
        }

        // �� ��ȯ ����
        int tileX = playerRect.x / TILE_SIZE;
        int tileY = playerRect.y / TILE_SIZE;

        if (currentMap == tileMap1 && currentMap[tileY][tileX] == 2) {
            currentMap = tileMap2;
            playerRect.x = 0 * TILE_SIZE;
            playerRect.y = 7 * TILE_SIZE;
            enemy.rect.x = 0;
            enemy.rect.y = 0;
            enemy.targetX = 0;
            enemy.targetY = 0;
            printf("�������� 2�� �̵�\n");
        }
        else if (currentMap == tileMap2 && currentMap[tileY][tileX] == 2) {
            currentMap = tileMap1;
            playerRect.x = 7 * TILE_SIZE;
            playerRect.y = 0 * TILE_SIZE;
            enemy.rect.x = 0;
            enemy.rect.y = 0;
            enemy.targetX = 0;
            enemy.targetY = 0;
            printf("�������� 1�� �̵�\n");
        }

        // ī�޶� ��ġ ���
        cameraX = playerRect.x - WINDOW_WIDTH / 2 + TILE_SIZE / 2;
        cameraY = playerRect.y - WINDOW_HEIGHT / 2 + TILE_SIZE / 2;

        if (cameraX < 0) cameraX = 0;
        if (cameraY < 0) cameraY = 0;

        int maxCameraX = MAP_WIDTH * TILE_SIZE - WINDOW_WIDTH;
        int maxCameraY = MAP_HEIGHT * TILE_SIZE - WINDOW_HEIGHT;

        if (cameraX > maxCameraX) cameraX = maxCameraX;
        if (cameraY > maxCameraY) cameraY = maxCameraY;
    }
}
