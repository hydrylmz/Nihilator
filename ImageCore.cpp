// ImageCore.cpp - Enhanced image loading system
#include "ImageCore.h"
#include <stdio.h>
#include "GameCore.h"

ImageCore::ImageCore() : loadedImage(NULL), targetFormat(NULL) {
}

ImageCore::~ImageCore() {
    freeResources();
}

bool ImageCore::init() {
    // Initialize PNG loading
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        return false;
    }
    return true;
}

bool ImageCore::loadMedia(const char* path) {
    // Free previous image if it exists
    if (loadedImage != NULL) {
        SDL_FreeSurface(loadedImage);
        loadedImage = NULL;
    }

    // Load new image
    loadedImage = loadSurface(path);
    return loadedImage != NULL;
}

bool ImageCore::renderToSurface(SDL_Surface* targetSurface) {
    if (loadedImage == NULL || targetSurface == NULL) {
        return false;
    }

    // Save target format for future optimization
    if (targetFormat == NULL) {
        setTargetFormat(targetSurface->format);
    }

    // Apply the image to the target surface
    SDL_BlitSurface(loadedImage, NULL, targetSurface, NULL);
    return true;
}

bool ImageCore::renderStretched(SDL_Surface* targetSurface, int width, int height) {
    if (loadedImage == NULL || targetSurface == NULL) {
        return false;
    }

    // Save target format for future optimization
    if (targetFormat == NULL) {
        setTargetFormat(targetSurface->format);
    }

    // Create a rectangle to stretch the image to
    SDL_Rect stretchRect;
    stretchRect.x = 0;
    stretchRect.y = 0;
    stretchRect.w = width;
    stretchRect.h = height;

    // Apply the image stretched to the target surface
    SDL_BlitScaled(loadedImage, NULL, targetSurface, &stretchRect);
    return true;
}

SDL_Texture* ImageCore::loadTexture(std::string path, SDL_Renderer* renderer)
{
    //The final texture
    SDL_Texture* newTexture = NULL;

    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL)
    {
        printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
    }
    else
    {
        newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        if (newTexture == NULL)
        {
            printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        }

        //Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }

    return newTexture;
}

void ImageCore::freeResources() {
    // Free loaded image
    if (loadedImage != NULL) {
        SDL_FreeSurface(loadedImage);
        loadedImage = NULL;
    }

    // Don't free targetFormat as it's owned by SDL
    targetFormat = NULL;
}

SDL_Surface* ImageCore::loadSurface(const char* path) {
    // The final optimized image
    SDL_Surface* optimizedSurface = NULL;

    // Determine file type by extension
    bool isPNG = false;
    const char* extension = strrchr(path, '.');
    if (extension != NULL) {
        if (SDL_strcasecmp(extension, ".png") == 0) {
            isPNG = true;
        }
    }

    // Load image based on file type
    SDL_Surface* loadedSurface = NULL;

    if (isPNG) {
        // Load PNG using SDL_image
        loadedSurface = IMG_Load(path);
        if (loadedSurface == NULL) {
            printf("Unable to load PNG image %s! SDL_image Error: %s\n", path, IMG_GetError());
        }
    }
    else {
        // Load BMP using SDL's built-in function
        loadedSurface = SDL_LoadBMP(path);
        if (loadedSurface == NULL) {
            printf("Unable to load BMP image %s! SDL Error: %s\n", path, SDL_GetError());
        }
    }

    // If loading succeeded and we have a target format
    if (loadedSurface != NULL && targetFormat != NULL) {
        // Convert surface to target format
        optimizedSurface = SDL_ConvertSurface(loadedSurface, targetFormat, 0);
        if (optimizedSurface == NULL) {
            printf("Unable to optimize image %s! SDL Error: %s\n", path, SDL_GetError());
            return loadedSurface; // Return original if optimization fails
        }

        // Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
        return optimizedSurface;
    }

    // Return original surface if no optimization was done
    return loadedSurface;
}

void ImageCore::setTargetFormat(SDL_PixelFormat* format) {
    targetFormat = format;
}