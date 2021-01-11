#pragma once
#include "CAnimate.h"

using namespace std;

class CText : public CAnimate
{
private:
		CTexture *DropTex;

public:
	CText();
	bool DropShadow;
	void Draw(SDL_Renderer *renderer);
	void SetText(SDL_Renderer *renderer, TTF_Font *font, std::string textureText, SDL_Color textColor);
};

