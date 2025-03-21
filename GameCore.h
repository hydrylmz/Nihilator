#ifndef GAME_CORE_H
#define GAME_CORE_H

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include "ImageCore.h"
#include "InputCore.h"

class GameCore {
public:
    // Screen dimension constants
    static const int SCREEN_WIDTH = 640;
    static const int SCREEN_HEIGHT = 480;

    // Constructor/Destructor
    GameCore();
    ~GameCore();

    // Starts up SDL and creates window
    bool init();

    // Loads media
    bool loadMedia(const char* imagePath);

    // Frees media and shuts down SDL
    void close();

    // Gets the window's screen surface
    SDL_Surface* getScreenSurface() const;

    // Gets the renderer
    SDL_Renderer* getRenderer();

    // Gets the image core
    ImageCore* getImageCore();

    // Set viewport functions
    void setTopLeftViewport();
    void setTopRightViewport();
    void setBottomViewport();

    // Render all viewports
    void renderViewports();

    // Main game loop
    void run();

private:
    // Initialize viewport rectangles
    void initViewports();

    // The window we'll be rendering to
    SDL_Window* nWindow;

    // The renderer
    SDL_Renderer* nRenderer;

    // Viewport rectangles
    SDL_Rect topLeftViewport;
    SDL_Rect topRightViewport;
    SDL_Rect bottomViewport;

    // The texture
    SDL_Texture* nTexture;

    // The surface contained by the window
    SDL_Surface* nScreenSurface;

    // Image handling component
    ImageCore imageCore;

    // Input handling component
    InputCore inputCore;

    // Path to the current loaded image
    std::string currentImagePath;
};

#endif // GAME_CORE_H