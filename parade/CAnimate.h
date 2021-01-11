#pragma once
#include "CTexture.h"

#include "CRectangle.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

class CAnimate
{


	//AudioClip audio;
protected:
	CTexture *mTexture;
	int     FrameInc;
	int     FrameRate; //Milliseconds
	long    OldTime;
	int mCurAnim;
	int mDelay, mTicks;
	int mXPos, mYPos;
	int W, H;
	int mStartFrame, mEndFrame, mCurFrame;

	bool mAnimate;

	int mVelocity;
	

	int mFloor;
	int mMaxFrames[10];
	int mMaxRows;

	Mix_Chunk *mAudio[5];
	int mNumAudio;
	bool mShowHitBox;
	bool mFade;

public:
	float mXProgress;
	int mState;
	bool mView;
	bool mActive;
	bool    Oscillate;
	bool mLoop;
	bool mWrapAnimation;

	void SetFloor(int f);


	 int GetFloor();

	 //void SetAudio(AudioClip a)
	//void PlayAudio();
	 //void StopAudio();
	 //void LoopAudio();

	 void SetMaxFrames(int r0, int r1, int r2, int r3, int r4, int r5, int r6, int r7, int r8, int r9);
	 void SetMaxFrames(int row, int max);
	 virtual int GetState();
	 virtual void Setup();
	 virtual void SetState(int s);
	 void SetActive(bool a);
	 void MoveOffScreen();
	 virtual void Reset();
	 virtual bool Collide(CRectangle r);
	 CRectangle GetBounds();
	 int GetWidth();
	 int GetHeight();
	 CAnimate();
	 virtual int GetVelocity();
	 virtual void SetVelocity(int v);
	 virtual void SetMoverState(bool b);

	 void SetTexture(CTexture *tex, int rows, int frames);
	 virtual void SetPos(int x, int y);
	 int GetPosX();
	 int GetPosY();
	 virtual void SetPosX(int x);
	 virtual void SetPosY(int y);
	 void SetFrame(int n);
	 virtual void Start(int anim, bool loop);
	 void Stop();
	 virtual void Transition();
	 virtual  bool Tick();
	 void OnAnimate();
	 virtual void Move();
	 virtual void Draw(SDL_Renderer *renderer);
	 void SetFrameRate(int ms);

	 void AddAudio(Mix_Chunk *audio);
	 void PlayAudio(int i);
	 void ShowHitBox(bool b);
	 void SetAlpha(Uint8 alpha);
	 Uint8 GetAlpha();
	 void Fade(bool b) { mFade = b; }
};

