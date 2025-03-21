#pragma once
#include <string>
#include <SDL.h>
#include <vector>
class NTexture
{
public:
    //Initializes variables
    NTexture(SDL_Renderer* renderer);

    //Deallocates memory
    ~NTexture();

    //Loads image at specified path
    bool loadFromFile(std::string path);

    //Deallocates texture
    void free();

    //Renders texture at given point
    void render(int x, int y, SDL_Rect* clip = nullptr);

    void setClips(int count, int clipWidth, int clipHeight);
    //Gets image dimensions
    int getWidth();
    int getHeight();

    std::vector<SDL_Rect>& getClips();

private:
    //The actual hardware texture
    SDL_Texture* nTexture;
    std::vector<SDL_Rect> nSpriteClips;

    SDL_Renderer* nRenderer; 

    //Image dimensions
    int nWidth;
    int nHeight;
};

