#include "CText.h"



CText::CText() : CAnimate()
{
	mTexture = new CTexture();
	DropShadow = false;
	DropTex = new CTexture();
}



void CText::SetText(SDL_Renderer *renderer, TTF_Font *font, std::string textureText, SDL_Color textColor)
{
	if (mTexture != NULL)
	{
		mTexture->LoadFromRenderedText(renderer, font, textureText, textColor);
		if (mTexture != NULL)
		{
			W = mTexture->GetWidth();
			H = mTexture->GetHeight();
		}

		if (DropShadow)
		{
			SDL_Color clr = { 0, 0, 0 };

			DropTex->LoadFromRenderedText(renderer, font, textureText, clr);
		}
	}
}

void CText::Draw(SDL_Renderer *renderer)
{

	if (mView == true)
	{
		if (DropTex != NULL)
		{
			SDL_Rect src;


			src = { 0, 0, W, H };


			SDL_Rect dst = { mXPos-2, mYPos+2, W, H };

			DropTex->Draw(renderer, &src, &dst, 0.0);
		}
	}
	CAnimate::Draw(renderer);
}