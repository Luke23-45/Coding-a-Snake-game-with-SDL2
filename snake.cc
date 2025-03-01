#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <deque>
#include <cstdlib>
#include <string>
#include <algorithm>

// Constants for window and grid.
const int WINDOW_WIDTH  = 580;
const int WINDOW_HEIGHT = 500;
const int TILE_SIZE     = 20;
const int UPDATE_INTERVAL_MS = 100; 

// Structure for snake segment.
struct Segment {
    int x;
    int y;
    Segment(int a, int b) : x(a), y(b) {}
};

// Structure for food.
struct Food {
    int x;
    int y;
    Food(int a, int b) : x(a), y(b) {}
};

// Check collision between a rectangle and a circle.
bool Collision(Segment* seg, int centerX, int centerY, int radius, int size) {
    int closeX = std::max(seg->x, std::min(centerX, seg->x + size));
    int closeY = std::max(seg->y, std::min(centerY, seg->y + size));
    int diffX = centerX - closeX;
    int diffY = centerY - closeY;
    return (diffX * diffX + diffY * diffY <= radius * radius);
}


void SDL_RenderFillCircle(SDL_Renderer* renderer, int centerX, int centerY, int radius) {
    for (int w = 0; w < radius * 2; w++) {
        for (int h = 0; h < radius * 2; h++) {
            int dx = radius - w;
            int dy = radius - h;
            if ((dx * dx + dy * dy) <= (radius * radius)) {
                SDL_RenderDrawPoint(renderer, centerX + dx, centerY + dy);
            }
        }
    }
}

// Generate a new food location.
Food GenerateFood(const std::deque<Segment*>& snake) {
    Food f(0, 0);
    bool isValid;
    do {
        // Choose a random position on the grid.
        f.x = (std::rand() % (WINDOW_WIDTH - TILE_SIZE));
        f.y = (std::rand() % (WINDOW_HEIGHT - TILE_SIZE));
        f.x = ((f.x + TILE_SIZE / 2) / TILE_SIZE) * TILE_SIZE;
        f.y = ((f.y + TILE_SIZE / 2) / TILE_SIZE) * TILE_SIZE;
        isValid = true;
        for (const auto& segment : snake) {
            if (segment->x == f.x && segment->y == f.y) {
                isValid = false;
                break;
            }
        }
    } while (!isValid);
    return f;
}

int main() {
    // Initialize SDL.
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL_Init Error: " << SDL_GetError() << "\n";
        return 1;
    }
    
    // Initialize SDL_ttf.
    if (TTF_Init() < 0) {
        std::cout << "TTF_Init Error: " << TTF_GetError() << "\n";
        SDL_Quit();
        return 1;
    }
    // Create window and renderer.
    SDL_Window* window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << "\n";
        TTF_Quit();
        SDL_Quit();
        return 1;
    }
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << "\n";
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }
    // Load font for rendering the score.
    TTF_Font* font = TTF_OpenFont("arial.ttf", 24);
    if (!font) {
        std::cout << "TTF_OpenFont Error: " << TTF_GetError() << "\n";
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }
    // Initialize snake.
    std::deque<Segment*> snake;
    // Starting with 4 segments positioned horizontally.
    snake.push_back(new Segment(80, 240));
    snake.push_back(new Segment(60, 240));
    snake.push_back(new Segment(40, 240));
    snake.push_back(new Segment(20, 240));
    
    // Direction: 0 = up, 1 = right, 2 = down, 3 = left.
    int dir = 1;          // current direction (initially right)
    int nextDir = dir;    // next direction to apply on update

    // Food variables.
    int foodX = 0;
    int foodY = 0;
    bool spawnFood = true; // trigger initial food spawn
    
    int score = 0;
    
    bool quit = false;
    SDL_Event e;
    
    // Timing for fixed update.
    Uint32 previousTime = SDL_GetTicks();
    int lag = 0; // in ms
    
    while (!quit) {
        // Calculate elapsed time.
        Uint32 currentTime = SDL_GetTicks();
        int elapsed = currentTime - previousTime;
        previousTime = currentTime;
        lag += elapsed;
        

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                quit = true;
            else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        quit = true;
                        break;
                    case SDLK_UP:
                        if (dir != 2) nextDir = 0;
                        break;
                    case SDLK_RIGHT:
                        if (dir != 3) nextDir = 1;
                        break;
                    case SDLK_DOWN:
                        if (dir != 0) nextDir = 2;
                        break;
                    case SDLK_LEFT:
                        if (dir != 1) nextDir = 3;
                        break;
                }
            }
        }
        

        while (lag >= UPDATE_INTERVAL_MS) {
            lag -= UPDATE_INTERVAL_MS;
            dir = nextDir;
            
            // Determine new head position based on current direction.
            Segment* head = snake.front();
            int newX = head->x;
            int newY = head->y;
            switch (dir) {
                case 0: newY -= TILE_SIZE; break; // up
                case 1: newX += TILE_SIZE; break; // right
                case 2: newY += TILE_SIZE; break; // down
                case 3: newX -= TILE_SIZE; break; // left
            }
            
            // Check collision with walls.
            if (newX < 0 || newX >= WINDOW_WIDTH || newY < 0 || newY >= WINDOW_HEIGHT) {
                std::cout << "Collision with wall! Final score: " << score << "\n";
                quit = true;
                break;
            }
            
            // Create new head segment.
            Segment* newHead = new Segment(newX, newY);
            snake.push_front(newHead);
            
            // Check self-collision (start checking from second segment).
            for (auto it = snake.begin() + 1; it != snake.end(); ++it) {
                if ((*it)->x == newHead->x && (*it)->y == newHead->y) {
                    std::cout << "Collision with self! Final score: " << score << "\n";
                    quit = true;
                    break;
                }
            }
            if (quit) break;
            
            // Spawn food if needed.
            if (spawnFood) {
                Food f = GenerateFood(snake);
                foodX = f.x + TILE_SIZE / 2;
                foodY = f.y + TILE_SIZE / 2;
                spawnFood = false;
            }
            
            // Check collision with food.
            if (Collision(newHead, foodX, foodY, TILE_SIZE/2 - 2, TILE_SIZE)) {
                score += 10;
                spawnFood = true; // next update will spawn new food
            } else {
                // If food not eaten, remove tail.
                delete snake.back();
                snake.pop_back();
            }
        }
        

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        

        SDL_SetRenderDrawColor(renderer, 25, 25, 25, 10);
        for (int i = 0; i <= WINDOW_WIDTH; i += TILE_SIZE) {
            SDL_RenderDrawLine(renderer, i, 0, i, WINDOW_HEIGHT);
        }
        for (int i = 0; i <= WINDOW_HEIGHT; i += TILE_SIZE) {
            SDL_RenderDrawLine(renderer, 0, i, WINDOW_WIDTH, i);
        }
        

        for (const auto& segment : snake) {
            SDL_Rect rect = { segment->x, segment->y, TILE_SIZE, TILE_SIZE };
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // green
            SDL_RenderFillRect(renderer, &rect);
        }
        
        // Draw food as a filled circle.
        SDL_SetRenderDrawColor(renderer, 34, 21, 100, 255);
        SDL_RenderFillCircle(renderer, foodX, foodY, TILE_SIZE / 2);
        
        // Render score text.
        std::string scoreText = "Score: " + std::to_string(score);
        SDL_Color textColor = {255, 255, 255, 255};
        SDL_Surface* textSurface = TTF_RenderText_Solid(font, scoreText.c_str(), textColor);
        if (textSurface) {
            SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            SDL_Rect textRect = {10, 10, textSurface->w, textSurface->h};
            SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);
            SDL_FreeSurface(textSurface);
            SDL_DestroyTexture(textTexture);
        }
        

        SDL_RenderPresent(renderer);
        
        SDL_Delay(1);
    }
    
    // free snake segments.
    for (auto segment : snake)
        delete segment;
    
    // Cleanup SDL_ttf and SDL.
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    
    return 0;
}
