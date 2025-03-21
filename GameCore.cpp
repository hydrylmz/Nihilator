#include "GameCore.h"
#include "NTexture.h"

GameCore::GameCore()
    : nWindow(nullptr), nRenderer(nullptr), nScreenSurface(nullptr),
    nTexture(nullptr), currentImagePath("") {
}

GameCore::~GameCore() {
    close();
}



bool GameCore::init() {
    // Initialization flag
    bool success = true;


    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    // Create window
    nWindow = SDL_CreateWindow("Nihilator", SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (nWindow == nullptr) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    // Create renderer
    nRenderer = SDL_CreateRenderer(nWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (nRenderer == nullptr) {
        printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
        return false;
    }

    // Initialize renderer color
    SDL_SetRenderDrawColor(nRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

    // Let ImageCore handle SDL_image initialization
    if (!imageCore.init()) {
        printf("Failed to initialize image system!\n");
        return false;
    }

    // Initialize viewports once
    initViewports();

    return true;
}

void GameCore::initViewports() {
    // Top left viewport
    topLeftViewport.x = 0;
    topLeftViewport.y = 0;
    topLeftViewport.w = SCREEN_WIDTH / 2;
    topLeftViewport.h = SCREEN_HEIGHT / 2;

    // Top right viewport
    topRightViewport.x = SCREEN_WIDTH / 2;
    topRightViewport.y = 0;
    topRightViewport.w = SCREEN_WIDTH / 2;
    topRightViewport.h = SCREEN_HEIGHT / 2;

    // Bottom viewport
    bottomViewport.x = 0;
    bottomViewport.y = SCREEN_HEIGHT / 2;
    bottomViewport.w = SCREEN_WIDTH;
    bottomViewport.h = SCREEN_HEIGHT / 2;
}

bool GameCore::loadMedia(const char* imagePath) {
    // Loading success flag
    bool success = true;

    // Store the image path
    currentImagePath = imagePath;

    // Free previous texture if it exists
    if (nTexture != nullptr) {
        SDL_DestroyTexture(nTexture);
        nTexture = nullptr;
    }

    // Load PNG texture
    nTexture = imageCore.loadTexture(imagePath, nRenderer);
    if (nTexture == nullptr) {
        printf("Failed to load texture image: %s\n", imagePath);
        return false;
    }

    return true;
}

void GameCore::close() {
    // Free loaded texture - only once
    if (nTexture != nullptr) {
        SDL_DestroyTexture(nTexture);
        nTexture = nullptr;
    }

    // Let ImageCore free its resources
    imageCore.freeResources();

    // Destroy renderer
    if (nRenderer != nullptr) {
        SDL_DestroyRenderer(nRenderer);
        nRenderer = nullptr;
    }

    // Destroy window
    if (nWindow != nullptr) {
        SDL_DestroyWindow(nWindow);
        nWindow = nullptr;
    }

    // Quit SDL subsystems - let IMG_Quit be called by ImageCore
    SDL_Quit();
}

SDL_Surface* GameCore::getScreenSurface() const {
    return nScreenSurface;
}

ImageCore* GameCore::getImageCore() {
    return &imageCore;
}

SDL_Renderer* GameCore::getRenderer() {
    return nRenderer;
}

void GameCore::setTopLeftViewport() 
{
    topLeftViewport.x = 0;
    topLeftViewport.y = 0;
    topLeftViewport.w = SCREEN_WIDTH / 2;
    topLeftViewport.h = SCREEN_HEIGHT / 2;
    SDL_RenderSetViewport(nRenderer, &topLeftViewport);

    //Render texture to screen
    SDL_RenderCopy(nRenderer, nTexture, NULL, NULL);
}

void GameCore::setTopRightViewport() 
{
    SDL_Rect topRightViewport;
    topRightViewport.x = SCREEN_WIDTH / 2;
    topRightViewport.y = 0;
    topRightViewport.w = SCREEN_WIDTH / 2;
    topRightViewport.h = SCREEN_HEIGHT / 2;
    SDL_RenderSetViewport(nRenderer, &topRightViewport);

    //Render texture to screen
    SDL_RenderCopy(nRenderer, nTexture, NULL, NULL);
}

void GameCore::renderViewports() {
    // Clear screen
    SDL_SetRenderDrawColor(nRenderer, 0, 0, 0, 255);
    SDL_RenderClear(nRenderer);

    if (nTexture == nullptr) {
        return;
    }

    // Render to top left viewport
    SDL_RenderSetViewport(nRenderer, &topLeftViewport);
    SDL_RenderCopy(nRenderer, nTexture, nullptr, nullptr);

    // Render to top right viewport
    SDL_RenderSetViewport(nRenderer, &topRightViewport);
    SDL_RenderCopy(nRenderer, nTexture, nullptr, nullptr);

    // Render to bottom viewport
    SDL_RenderSetViewport(nRenderer, &bottomViewport);
    SDL_RenderCopy(nRenderer, nTexture, nullptr, nullptr);

    // Update screen once after all viewports are rendered
    SDL_RenderPresent(nRenderer);
}
void GameCore::setBottomViewport() 
{
    SDL_Rect bottomViewport;
    bottomViewport.x = 0;
    bottomViewport.y = SCREEN_HEIGHT / 2;
    bottomViewport.w = SCREEN_WIDTH;
    bottomViewport.h = SCREEN_HEIGHT / 2;
    SDL_RenderSetViewport(nRenderer, &bottomViewport);

    //Render texture to screen
    SDL_RenderCopy(nRenderer, nTexture, NULL, NULL);
}

void GameCore::run() {
    // Start up SDL and create window
    if (!init()) {
        printf("Failed to initialize!\n");
        return;
    }

    // Load default image
    if (!loadMedia("images/ataturk.bmp")) {
        printf("Failed to load default image!\n");
        return;
    }   
    
    NTexture nSpriteSheetTexture(nRenderer);
    if (!nSpriteSheetTexture.loadFromFile("images/sprites.png")) {
        printf("Failed to load sprite sheet texture!\n");
    }
    else {
        nSpriteSheetTexture.setClips(4, 100, 100);
    }

    std::vector<SDL_Rect>& clips = nSpriteSheetTexture.getClips();

    // Main loop flag
    bool quit = false;

    // Event handler  
    SDL_Event e;

    SDL_SetRenderDrawColor(nRenderer, 0, 0, 0, 255);
    SDL_RenderClear(nRenderer);
    renderViewports();
    /*nSpriteSheetTexture.render(0, 0, &clips[0]);

    //Render top right sprite
    nSpriteSheetTexture.render(SCREEN_WIDTH - clips[1].w, 0, &clips[1]);

    //Render bottom left sprite
    nSpriteSheetTexture.render(0, SCREEN_HEIGHT - clips[2].h, &clips[2]);

    //Render bottom right sprite
    nSpriteSheetTexture.render(SCREEN_WIDTH - clips[3].w, SCREEN_HEIGHT - clips[3].h, &clips[3]);*/

    // Main game loop
    while (!quit) {
        // Handle events on queue
        while (SDL_PollEvent(&e) != 0) {
            // User requests quit
            if (e.type == SDL_QUIT) {
                quit = true;
            }

            // Handle input events
            inputCore.handleEvent(e);
        }


        SDL_RenderPresent(nRenderer);
    }

}