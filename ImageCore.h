// ImageCore.h - Enhanced image loading system
#ifndef IMAGE_CORE_H
#define IMAGE_CORE_H

#include <SDL.h>
#include <SDL_image.h>
#include <string>

class ImageCore {
public:
    // Constructor/Destructor
    ImageCore();
    ~ImageCore();

    SDL_Texture* loadTexture(std::string path, SDL_Renderer* renderer);



    // Initialize image subsystem
    bool init();

    // Load media from specified path (supports both BMP and PNG)
    bool loadMedia(const char* path);

    // Render loaded image to target surface
    bool renderToSurface(SDL_Surface* targetSurface);

    // Render loaded image stretched to fit screen dimensions
    bool renderStretched(SDL_Surface* targetSurface, int width, int height);

    // Free resources
    void freeResources();

private:
    // The image we will load and show on the screen
    SDL_Surface* loadedImage;

    // Format of the target surface for optimization
    SDL_PixelFormat* targetFormat;

    // Helper function to load and optimize a surface
    SDL_Surface* loadSurface(const char* path);

    SDL_Texture* loadTexture(std::string path);

    // Set the target format for optimization
    void setTargetFormat(SDL_PixelFormat* format);
};

#endif // IMAGE_CORE_H