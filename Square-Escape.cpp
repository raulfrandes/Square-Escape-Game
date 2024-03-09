#include <iostream>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <vector>
#include <random>

#define RAND_MAX 550;

using std::rand;
using std::vector;

#undef main

#if defined (__APPLE__)
#define RENDERER_TYPE SDL_RENDERER_SOFTWARE
#else
#define RENDERER_TYPE SDL_RENDERER_ACCELERATED
#endif

//define window dimensions
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

const float ANIMATION_SPEED = 1.0f;

//define SDL Window related variables
SDL_Window* window = NULL;
SDL_Renderer* windowRenderer = NULL;
SDL_Event currentEvent;

bool quit = false;
bool finish = false;

int mouseX, mouseY;

//SDL_Rect rectangle = {100, 100, 200, 200};

struct Rectangle {

    glm::vec2 pos;
    int width;
    int height;
    glm::vec3 color;
    float speed = 0.1f;

    SDL_Rect getSDLrect() {

        SDL_Rect r;
        r.x = pos.x - width / 2.0f;
        r.y = pos.y - height / 2.0f;
        r.w = width;
        r.h = height;
        return r;
    }
};

struct Obstacol {

    glm::vec2 pos;
    int width;
    int height;
    glm::vec3 color;
    float speed = 0.05f;
    Rectangle alb;

    SDL_Rect getSDLrect() {

        SDL_Rect r;
        r.x = pos.x - width / 2.0f;
        r.y = pos.y - height / 2.0f;
        r.w = width;
        r.h = height;
        return r;
    }
};

Rectangle r1;

void initRectangle() {

    r1.pos.x = 25.0f;
    r1.pos.y = 300.0f;
    r1.width = 50;
    r1.height = 50;
    r1.color.r = 110.0f;
    r1.color.g = 0.0f;
    r1.color.b = 200.0f;
}

vector<Obstacol> obstacles(50);


void initObst() {
    float pos = 812.5f;
    float random;
    srand(time(0));
    for (Obstacol& o : obstacles) {

        random = static_cast<float>(rand() % 550);
        o.pos.x = pos;
        o.pos.y = 300.0f;
        o.width = 25;
        o.height = 600;
        o.color.r = 0.0f;
        o.color.g = 0.0f;
        o.color.b = 255.0f;
        pos = pos + 150.0f;


        o.alb.pos.x = o.pos.x;
        o.alb.pos.y = random;

        o.alb.color.r = 255.0f;
        o.alb.color.b = 255.0f;
        o.alb.color.g = 255.0f;

        o.alb.width = o.width;
        o.alb.height = 150;
    }
}

bool movingUp = false;
bool movingDown = false;
bool movingLeft = false;
bool movingRight = false;

Uint32 startTime;
Uint32 endTime;
float elapsedTime;

bool initWindow() {

    bool success = true;

    //Try to initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {

        std::cout << "SDL initialization failed" << std::endl;
        success = false;

    }
    else {

        //Try to create the window
        window = SDL_CreateWindow(
            "SDL Hello Window Example",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            WINDOW_WIDTH,
            WINDOW_HEIGHT,
            SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);

        if (window == NULL) {

            std::cout << "Failed to create window: " << SDL_GetError() << std::endl;
            success = false;

        }
        else {

            // Create a renderer for the current window
            windowRenderer = SDL_CreateRenderer(window, -1, RENDERER_TYPE);

            if (windowRenderer == NULL) {

                std::cout << "Failed to create the renderer: " << SDL_GetError() << std::endl;
                success = false;

            }
            else {

                //Set background color
                SDL_SetRenderDrawColor(windowRenderer, 255, 255, 255, 255);

                //Apply background color
                SDL_RenderClear(windowRenderer);
            }
        }
    }

    return success;
}

void processEvents() {

    //Check for events in queue
    SDL_PollEvent(&currentEvent);

    //User requests quit
    if (currentEvent.type == SDL_QUIT) {

        quit = true;
    }



    //Mouse event -> pressed button
    if (currentEvent.type == SDL_MOUSEBUTTONDOWN) {

        if (currentEvent.button.button == SDL_BUTTON_LEFT) {

            SDL_GetMouseState(&mouseX, &mouseY);
            std::cout << "Mouse left click => " << "x: " << mouseX << ", y: " << mouseY << std::endl;
        }

        if (currentEvent.button.button == SDL_BUTTON_RIGHT) {

            SDL_GetMouseState(&mouseX, &mouseY);
            std::cout << "Mouse right click => " << "x: " << mouseX << ", y: " << mouseY << std::endl;
        }
    }

    //Mouse event -> mouse movement
    if (currentEvent.type == SDL_MOUSEMOTION) {

        SDL_GetMouseState(&mouseX, &mouseY);

        if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON_LMASK) {

            SDL_GetMouseState(&mouseX, &mouseY);
            std::cout << "Mouse move while left clicking => " << "x: " << mouseX << ", y: " << mouseY << std::endl;
        }

        if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON_RMASK) {

            SDL_GetMouseState(&mouseX, &mouseY);
            std::cout << "Mouse move while right clicking => " << "x: " << mouseX << ", y: " << mouseY << std::endl;
        }
    }

    //Keyboard event keydown
    if (currentEvent.type == SDL_KEYDOWN) {

        switch (currentEvent.key.keysym.sym) {

        case SDLK_UP:
            movingUp = true;
            break;

        case SDLK_DOWN:
            movingDown = true;
            break;

        case SDLK_LEFT:
            movingLeft = true;
            break;

        case SDLK_RIGHT:
            movingRight = true;
            break;

        case SDLK_r:
            break;

        case SDLK_s:
            break;

        case SDLK_ESCAPE:

            quit = true;
            break;

        default:
            break;
        }
    }

    //Keyboard event keyup
    if (currentEvent.type == SDL_KEYUP) {

        switch (currentEvent.key.keysym.sym) {

        case SDLK_UP:
            movingUp = false;
            break;

        case SDLK_DOWN:
            movingDown = false;
            break;

        case SDLK_LEFT:
            movingLeft = false;
            break;

        case SDLK_RIGHT:
            movingRight = false;
            break;

        case SDLK_r:
            break;

        case SDLK_s:
            break;

        case SDLK_ESCAPE:

            quit = true;
            break;

        default:
            break;
        }
    }
}

bool checkY() {
    /*bool ok = false;
    for (Obstacol& o : obstacles) {
        if (r1.pos.y + static_cast<float>(r1.height / 2) > o.alb.pos.y + static_cast<float>(o.alb.height / 2) && r1.pos.y - static_cast<float>(r1.height / 2) < o.alb.pos.y - static_cast<float>(o.alb.height / 2)) {
            true;
        }
    }
    return ok;*/
    /*if (r1.pos.y + static_cast<float>(r1.height / 2) < obstacles[0].alb.pos.y + static_cast<float>(obstacles[0].alb.height / 2) && r1.pos.y - static_cast<float>(r1.height / 2) > obstacles[0].alb.pos.y - static_cast<float>(obstacles[0].alb.height / 2)) {
        return true;
    }
    return false;*/
    if (r1.pos.x == 300.0f) {
        return true;
    }
    return false;
}


void updateRectanglePos() {

    if (movingUp) {

        r1.pos.y -= r1.speed;
    }

    if (movingDown) {

        r1.pos.y += r1.speed;
    }

    if (movingRight) {

        r1.pos.x += r1.speed;
    }

    if (movingLeft) {

        r1.pos.x -= r1.speed;
    }

    //for (int i = 0; i < 100000000; i++);

    //glm::vec2 cursorPos{ mouseX, mouseY };
    //glm::vec2 r1Dir = cursorPos - r1.pos;
    //r1Dir = glm::normalize(r1Dir);
    //r1.pos = r1.pos + r1Dir * r1.speed * elapsedTime * ANIMATION_SPEED;
}

void updateObstPos() {
    for (Obstacol& o : obstacles) {
        o.pos.x -= o.speed;
        o.alb.pos.x -= o.speed;
    }
}

void winGame() {
    if ((r1.pos.x + static_cast<float>(r1.width) / 2) == 800) {
        finish = true;
        SDL_SetRenderDrawColor(windowRenderer, static_cast<Uint8>(r1.color.r), static_cast<Uint8>(r1.color.g), static_cast<Uint8>(r1.color.b), 255);
        SDL_Rect r = r1.getSDLrect();

        SDL_RenderFillRect(windowRenderer, &r);

    }
}

void drawFrame() {

    //Clear the background
    SDL_SetRenderDrawColor(windowRenderer, 255, 255, 255, 255);
    SDL_RenderClear(windowRenderer);


    //fill obstacol
    for (Obstacol& o : obstacles) {

        SDL_SetRenderDrawColor(windowRenderer, static_cast<Uint8>(o.color.r), static_cast<Uint8>(o.color.g), static_cast<Uint8>(o.color.b), 255);

        SDL_Rect o1 = o.getSDLrect();
        SDL_RenderFillRect(windowRenderer, &o1);


        SDL_SetRenderDrawColor(windowRenderer, static_cast<Uint8>(o.alb.color.r), static_cast<Uint8>(o.alb.color.g), static_cast<Uint8>(o.alb.color.b), 255);

        SDL_Rect o2 = o.alb.getSDLrect();
        SDL_RenderFillRect(windowRenderer, &o2);
    }

    SDL_SetRenderDrawColor(windowRenderer, static_cast<Uint8>(r1.color.r), static_cast<Uint8>(r1.color.g), static_cast<Uint8>(r1.color.b), 255);

    SDL_Rect r = r1.getSDLrect();
    SDL_RenderFillRect(windowRenderer, &r);




    //Draw scene
    //TODO
    if (r1.pos.x + static_cast<float>(r1.width) / 2 <= 800.0f && r1.pos.y + static_cast<float>(r1.width) / 2 <= 600.0f && r1.pos.x - static_cast<float>(r1.width) / 2 >= 0.0f && r1.pos.y - static_cast<float>(r1.width) / 2 >= 0.0f) {
        if (checkY) {
            updateObstPos();
            updateRectanglePos();
        }
        else quit = true;
    }
    //winGame();


    //Update window
    SDL_RenderPresent(windowRenderer);
}

void cleanup() {

    //Destroy renderer
    if (windowRenderer) {

        SDL_DestroyRenderer(windowRenderer);
        windowRenderer = NULL;
    }

    //Destroy window
    if (window) {

        SDL_DestroyWindow(window);
        window = NULL;
    }

    //Quit SDL
    SDL_Quit();
}

int main(int argc, char* argv[]) {

    //Initialize window
    if (!initWindow()) {

        std::cout << "Failed to initialize" << std::endl;
        return -1;
    }

    initObst();
    initRectangle();


    //Game loop
    while (!quit) {
        startTime = SDL_GetTicks();




        processEvents();
        drawFrame();

        endTime = SDL_GetTicks();
        elapsedTime = endTime - startTime;
    }

    cleanup();
    return 0;
}
