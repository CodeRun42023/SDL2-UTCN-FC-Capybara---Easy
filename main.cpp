#include <iostream>
#include <SDL.h>
#include <glm/glm.hpp>
#include <vector>

//define window dimensions
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

//define SDL Window related variables
SDL_Window* window = NULL;
SDL_Renderer* windowRenderer = NULL;
SDL_Event currentEvent;

bool quit = false;

int mouseX, mouseY;

const float GLOBAL_ANIMATION_SPEED = 1.0f;

Uint64 elapsedTime, startTime, endTime;

struct Rectangle {
    glm::vec2 center;
    int left_margin;
    int right_margin;
    int width;
    int height;
    float speed = 20.0f;
    glm::vec2 direction;
    glm::vec3 color = glm::vec3(0.0f, 255.0f, 0.0f);

    SDL_Rect getSDLRect() {
        SDL_Rect r;

        r.x = center.x - width / 2.0f;
        r.y = center.y - height / 2.0f;
        r.w = width;
        r.h = height;

        return r;
    }
};

Rectangle rect;

std::vector<Rectangle> enemies;

bool squareIsBlue = false;

void initRectangle() {
    //cannon
    rect.center.x = 250.0f;
    rect.center.y = 590.0f;
    rect.width = 60;
    rect.height = 30;
    rect.color = glm::vec3(0.0f, 0.0f, 0.0f);

    enemies.push_back(rect);

    //big square
    rect.center.x = 80.0f;//stanga dreapta
    rect.center.y = 60.0f;//sus jos
    rect.width = 60;
    rect.height = 60;
    rect.speed = 0.5f;
    rect.color = glm::vec3(0.0f, 255.0f, 0.0f);

    enemies.push_back(rect);
}

/*void animate() {
        enemies.at(1).direction = glm::normalize(glm::vec2(mouseX, mouseY) - enemies.at(1).center);
        enemies.at(1).center += GLOBAL_ANIMATION_SPEED * elapsedTime * enemies.at(1).speed * enemies.at(1).direction;
}
*/

bool initWindow() {

    bool success = true;

    //Try to initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0 ) {

        std::cout << "SDL initialization failed" << std::endl;
        success = false;

    }
    else {

        //Try to create the window
        window = SDL_CreateWindow(
            "Hello SDL2 Window!",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            WINDOW_WIDTH,
            WINDOW_HEIGHT,
            SDL_WINDOW_SHOWN);

        if (window == NULL) {

            std::cout << "Failed to create window: " << SDL_GetError() << std::endl;
            success = false;

        } else {

            // Create a renderer for the current window
            windowRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

            if (windowRenderer == NULL) {

                std::cout << "Failed to create the renderer: " << SDL_GetError() << std::endl;
                success = false;

            } else {

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
    }

    //Mouse event -> mouse movement
    if (currentEvent.type == SDL_MOUSEMOTION) {

        if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON_LMASK) {

            SDL_GetMouseState(&mouseX, &mouseY);
            std::cout << "Mouse move while left clicking => " << "x: " << mouseX << ", y: " << mouseY << std::endl;
        }
    }

    //Keyboard event
    if (currentEvent.type == SDL_KEYDOWN) {

        switch (currentEvent.key.keysym.sym) {

        case SDLK_UP:
            break;

        case SDLK_DOWN:
            break;

        case SDLK_LEFT:
                //enemies.at(0).center.x -= 6.0f;
            break;

        case SDLK_RIGHT:
            //enemies.at(0).center.x += 6.0f;
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

void drawRectangle() {
    for (auto enemy : enemies) {
        SDL_SetRenderDrawColor(windowRenderer, enemy.color.r, enemy.color.g, enemy.color.b, 255);
        SDL_Rect tmpr = enemy.getSDLRect();
        SDL_RenderFillRect(windowRenderer, &tmpr);
    }
}

void drawFrame() {
    //Clear the background
    SDL_SetRenderDrawColor(windowRenderer, 255, 255, 255, 255);
    SDL_RenderClear(windowRenderer);

    //Draw scene
    drawRectangle();
    
    //Update window
    SDL_RenderPresent(windowRenderer);
}

void processMovement() {
    const Uint8* state = SDL_GetKeyboardState(nullptr);
    if (state[SDL_SCANCODE_RIGHT] && enemies.at(0).center.x + enemies.at(0).width / 2.0f < WINDOW_WIDTH) {
        enemies.at(0).center.x += 0.01f;
    }

    if (state[SDL_SCANCODE_LEFT] && enemies.at(0).center.x - enemies.at(0).width / 2.0f > 0) {
        enemies.at(0).center.x -= 0.01f;
    }
    /*if (state[SDL_SCANCODE_RIGHT]) {
        rect.center.x += 0.3f;
    }

    if (state[SDL_SCANCODE_LEFT]) {
        rect.center.x -= 0.3f;
    }

    if (state[SDL_SCANCODE_UP]) {
        rect.center.y -= 0.3f;
    }

    if (state[SDL_SCANCODE_DOWN]) {
        rect.center.y += 0.3f;
    }*/
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

int ok=0;

Uint32 callback(Uint32 interval, void* name) {
    if (enemies.at(1).center.x + enemies.at(1).width / 2.0f < WINDOW_WIDTH - 60 && ok == 0) {
        enemies.at(1).center.x += GLOBAL_ANIMATION_SPEED * enemies.at(1).speed * 100.0f; // Move right
    }
    else if (enemies.at(1).center.y < WINDOW_HEIGHT - 60 && ok == 0) {
        enemies.at(1).center.y += 5.0f; // Move down
    }
    else if (enemies.at(1).center.x - enemies.at(1).width / 2.0f > 60 && ok == 1) {
        enemies.at(1).center.x -= GLOBAL_ANIMATION_SPEED * enemies.at(1).speed * 100.0f; // Move left
    }
    else if (enemies.at(1).center.y < WINDOW_HEIGHT - 60 && ok == 1) {
        enemies.at(1).center.y += 5.0f; // Move down
    }
    else {
        // Reset to the starting position
        ok = 0;
        enemies.at(1).center.x = 80.0f;
        enemies.at(1).center.y = 60.0f;
    }

    return 1000;
}




Uint32 callback2(Uint32 interval, void* name) {

    /*if (squareIsBlue) {
        enemies.at(0).color = glm::vec3(0.0f, 255.0f, 0.0f);
        squareIsBlue = false;
    }
    else {
        enemies.at(0).color = glm::vec3(0.0f, 0.0f, 255.0f);
        squareIsBlue = true;
    }
    return 1000;
    */
    if (enemies.at(1).center.x + enemies.at(1).width / 2.0f < WINDOW_WIDTH-60 && ok==0) {
        enemies.at(1).center.x += GLOBAL_ANIMATION_SPEED * enemies.at(1).speed* 100.0f; //la dreapta
    }

    if (enemies.at(1).center.x + enemies.at(1).width / 2.0f == WINDOW_WIDTH-60) {
        ok = 1;
        enemies.at(1).center.x -= GLOBAL_ANIMATION_SPEED * enemies.at(1).speed * 100.0f; //la dreapta   
        enemies.at(1).center.y -= 5.0f;
    }

    //if (enemies.at(1).center.x - enemies.at(1).width / 2.0f > 0 && ok==1) {
       
       //enemies.at(1).center.x -= GLOBAL_ANIMATION_SPEED * enemies.at(1).speed * 100.0f;
    //}


    //enemies.at(1).direction = glm::normalize(glm::vec2(enemies.at(1).center.x+20.0f, enemies.at(1).center.y) - enemies.at(1).center);
    //enemies.at(1).center += GLOBAL_ANIMATION_SPEED * enemies.at(1).speed * enemies.at(1).direction;

    return 1000;
}


int main(int argc, char* argv[]) {

    //Initialize window
    if (!initWindow()) {

        std::cout << "Failed to initialize" << std::endl;
        return -1;
    }

    initRectangle();
    
    SDL_TimerID timerID = SDL_AddTimer(1000, callback, nullptr);

    //Game loop
    while (!quit) {
        startTime = SDL_GetTicks64();
        processMovement();
        processEvents();
        drawFrame();
        //animate();

        endTime = SDL_GetTicks64();
        elapsedTime = endTime - startTime;
    }
    SDL_RemoveTimer(timerID);

    cleanup();
    return 0;
}
