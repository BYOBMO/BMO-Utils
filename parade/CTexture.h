#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <sstream>
#include "CRectangle.h"

using namespace std;

class CTexture
{
public:
	CTexture();
	~CTexture();
	void DrawBounds(SDL_Renderer *renderer, int x, int y);
	void Draw(SDL_Renderer *renderer, SDL_Rect *srcRect, SDL_Rect *dstRect, float angle);
	void Draw(SDL_Renderer *renderer, int x, int y, int w, int h, float angle);
	void Draw(SDL_Renderer *renderer, int x, int y, float angle);
	void Draw(SDL_Renderer *renderer);
	void SetTexture(SDL_Texture *tex);
	void SetPosition(int x, int y);
	bool LoadFromFile(SDL_Renderer *renderer, std::string path);
	int GetWidth() { return(mTexW); }
	int GetHeight() { return(mTexH); }
	void setAngle(float a) { Angle = a; }
	float getAngle() { return(Angle); }
	void SetBounds(CRectangle bounds) { mBounds = bounds; }
	CRectangle GetBounds() { return(mBounds);}
	void free();
	bool LoadFromRenderedText(SDL_Renderer *renderer, TTF_Font *font, std::string textureText, SDL_Color textColor);
	void SetAlpha(Uint8 alpha);
	Uint8 GetAlpha() { return(mAlpha); }
private:
	int X, Y;
	SDL_Texture *mTexture;
	int mTexW;
	int mTexH;
	float Angle;
	CRectangle mBounds;
	Uint8 mAlpha;
};

