#include "CTexture.h"

#include <stdio.h>
#include <sstream>

CTexture::CTexture()
{
	mTexture = NULL;
	Angle = 0.0;
	X = 0;
	Y = 0;
	mTexW = 0;
	mTexH = 0;
	mBounds = { 0,0,0,0 };
	mAlpha = 0xFF;
}


CTexture::~CTexture()
{
}

void CTexture::DrawBounds(SDL_Renderer *renderer, int x, int y)
{
	if (mTexture != NULL)
	{
		//Render to screen
		if (mBounds.w > 0 && mBounds.h > 0)
		{

			SDL_Rect r;

			r.x = x + mBounds.x;
			r.y = y + mBounds.y;
			r.w = mBounds.w;
			r.h = mBounds.h;

			SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xff);
			SDL_RenderDrawRect(renderer, &r);
		}
	}
}

void CTexture::SetAlpha(Uint8 alpha)
{
	mAlpha = alpha;
}

void CTexture::Draw(SDL_Renderer *renderer, SDL_Rect *srcRect, SDL_Rect *dstRect, float angle)
{
	if (mTexture != NULL)
	{
		SDL_SetTextureAlphaMod(mTexture, mAlpha);
		//Render to screen
		SDL_RenderCopyEx(renderer, mTexture, srcRect, dstRect, angle, NULL, SDL_FLIP_NONE);
	}
}

void CTexture::Draw(SDL_Renderer *renderer, int x, int y, int w, int h, float angle)
{
	if (mTexture != NULL)
	{
		//Set rendering space and render to screen
		SDL_Rect renderQuad = { x, y, w, h };

		//Set clip rendering dimensions
		//if (clip != NULL)
		//{
		//	renderQuad.w = clip->w;
		//	renderQuad.h = clip->h;
		//}

		//Render to screen
		SDL_SetTextureAlphaMod(mTexture, mAlpha);
		SDL_RenderCopyEx(renderer, mTexture, NULL, &renderQuad, angle, NULL, SDL_FLIP_NONE);
	}
}

void CTexture::Draw(SDL_Renderer *renderer, int x, int y, float angle)
{
	if (mTexture != NULL)
	{
		//Set rendering space and render to screen
		SDL_Rect renderQuad = { x, y, mTexW, mTexH };

		//Set clip rendering dimensions
		//if (clip != NULL)
		//{
		//	renderQuad.w = clip->w;
		//	renderQuad.h = clip->h;
		//}

		//Render to screen
		SDL_SetTextureAlphaMod(mTexture, mAlpha);
		SDL_RenderCopyEx(renderer, mTexture, NULL, &renderQuad, angle, NULL, SDL_FLIP_NONE);
	}
}

void CTexture::Draw(SDL_Renderer *renderer)
{
	if (mTexture != NULL)
	{
		//Set rendering space and render to screen
		SDL_Rect renderQuad = { X, Y, mTexW, mTexH };

		//Set clip rendering dimensions
		//if (clip != NULL)
		//{
		//	renderQuad.w = clip->w;
		//	renderQuad.h = clip->h;
		//}

		//Render to screen
		SDL_SetTextureAlphaMod(mTexture, mAlpha);
		SDL_RenderCopyEx(renderer, mTexture, NULL, &renderQuad, 0.0, NULL, SDL_FLIP_NONE);
	}
}

void CTexture::SetTexture(SDL_Texture *tex)
{
	mTexture = tex;
}

void CTexture::SetPosition(int x, int y)
{
	X = x;
	Y = y;
}

bool CTexture::LoadFromFile(SDL_Renderer *renderer, std::string path)
{
	//Get rid of preexisting texture
	//free();

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
		//SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0xFF, 0x00, 0xFF));

		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}
		else
		{
			//Get image dimensions
			mTexW = loadedSurface->w;
			mTexH = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}

void CTexture::free()
{
	//Free texture if it exists
	if (mTexture != NULL)
	{
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
		mTexW = 0;
		mTexH = 0;
	}
}

bool CTexture::LoadFromRenderedText(SDL_Renderer *renderer, TTF_Font *font, std::string textureText, SDL_Color textColor)
{
	//Get rid of preexisting texture
	free();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Blended(font, textureText.c_str(), textColor);
	if (textSurface == NULL)
	{
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else
	{
		//Create texture from surface pixels
		mTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
		if (mTexture == NULL)
		{
			printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		}
		else
		{
			//Get image dimensions
			mTexW = textSurface->w;
			mTexH = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface(textSurface);
	}

	//Return success
	return mTexture != NULL;
}