#include "NTexture.h"
#include <SDL_image.h>
#include "GameCore.h"

NTexture::NTexture(SDL_Renderer* renderer) {
	nTexture = nullptr;
	nWidth = 0;
	nHeight = 0;
	nRenderer = renderer; // GameCore tarafýndan gelen renderer
}

NTexture::~NTexture()
{
    //Deallocate

    free();
}

bool NTexture::loadFromFile(std::string path)
{
	//Get rid of preexisting texture
	free();

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
		//Color key image
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(nRenderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}
		else
		{
			//Get image dimensions
			nWidth = loadedSurface->w;
			nHeight = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	//Return success
	nTexture = newTexture;
	return nTexture != NULL;
}

void NTexture::free()
{
	//Free texture if it exists
	if (nTexture != NULL)
	{
		SDL_DestroyTexture(nTexture);
		nTexture = NULL;
		nWidth = 0;
		nHeight = 0;
	}
}

std::vector<SDL_Rect>& NTexture::getClips() {
	return nSpriteClips;
}

void NTexture::setClips(int count, int clipWidth, int clipHeight) {
	nSpriteClips.clear(); 
	nSpriteClips.resize(count); 

	for (int i = 0; i < count; i++) {
		nSpriteClips[i].x = (i % 2) * clipWidth;
		nSpriteClips[i].y = (i / 2) * clipHeight;
		nSpriteClips[i].w = clipWidth;
		nSpriteClips[i].h = clipHeight;
	}
}

void NTexture::render(int x, int y, SDL_Rect* clip)
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, nWidth, nHeight };

	//Set clip rendering dimensions
	if (clip != NULL)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopy(nRenderer, nTexture, clip, &renderQuad);
}

int NTexture::getWidth()
{
	return nWidth;
}

int NTexture::getHeight()
{
	return nHeight;
}
