#include "CAnimate.h"

	CAnimate::CAnimate()
	{
		mView = true;
		Setup();
	}

	void CAnimate::Setup()
	{
		int i;

		mXProgress = 1.0; // Percentage of teh sprite to draw horizontally. From 0.0 to 1.0. Allows for progress bars.

		for (i=0; i<10; i++)
		{
			mMaxFrames[i] = 0;
		}

		for (i = 0; i < 5; i++)
		{
			mAudio[i] = NULL;
		}

		mNumAudio = 0;

		mTexture = NULL;

		mFloor = 600;
		mDelay = 0;
		mTicks = 0;
		mXPos = 0;
		mYPos = 0;
		W=0; H=0;
		mStartFrame = 0;
		mEndFrame = 0;
		mCurFrame = 0;
		mState = 0;
		mAnimate = false;
		mVelocity = 1;
		mCurAnim = 0;
		mActive = false;
		mLoop = true;
		mWrapAnimation = false;
		mView = true;
		Oscillate = false;
		FrameInc = 1;
		FrameRate = 50; //Milliseconds
		OldTime = 0;
		mShowHitBox = false;
		mFade = false;
	}

	void CAnimate::ShowHitBox(bool b)
	{
		mShowHitBox = b;

	}

	 void CAnimate::SetFloor(int f)
	{
		mFloor = f; // This is the lowest on the screen that you want the Move object to allow the sprite to go to.
	}

	 int CAnimate::GetFloor()
	{
		return(mFloor);
	}


	 int CAnimate::GetState()
	{
		return(mState);
	}


	 //
	 // Set the current state of the sprite. The Transition() mehtod uses the state to determine 
	 // what to do next.
	 //
	void CAnimate::SetState(int s)
	{
		mState = s;
	}

	void CAnimate::SetActive(bool a)
	{
		mActive = a;
	}

	 void CAnimate::MoveOffScreen()
	{
		SetPosX(-100 - GetWidth());

		SetMoverState(false);
		Stop();
	}

	 void CAnimate::Reset()
	{
		Stop();
		mCurAnim = 0;
		mCurFrame = 0;
		mState = 0;
		MoveOffScreen();

		mActive = false;
	}

	 //
	 // Returns true if the sprite's hitbox is touching the specified rectangle.
	 //
	 bool CAnimate::Collide(CRectangle r)
	{
		 CRectangle r2 = GetBounds();

		if (r2.intersects(r))
			return(true);

		return(false);
	}

	 //
	 // Return the hotbox for the sprite.
	 //
	 CRectangle CAnimate::GetBounds()
	{
		CRectangle r = {0,0,0,0};

		if (mTexture != NULL)
		{
			r = mTexture->GetBounds();
			r.x = r.x + mXPos;
			r.y = r.y + mYPos;
		}
		return(r);
	}

	int CAnimate::GetWidth()
	{
		return(W);
	}

	 int CAnimate::GetHeight()
	{
		 return(H);
	}


	 int CAnimate::GetVelocity()
	{
		return(mVelocity);
	}

	 void CAnimate::SetVelocity(int v)
	{
		mVelocity = v;

	}

	 //
	 // Turn teh sprite's mover object on and off.
	 //
	 void CAnimate::SetMoverState(bool b)
	{

	}




	 void CAnimate::SetTexture(CTexture *tex, int rows, int frames)
	 {
		 int i;
		 mMaxRows = rows;
		 for (i = 0; i < 10; i++)
		 {
			 mMaxFrames[i] = frames;
		 }

		 mTexture = tex;
		 if (tex != NULL)
		 {
			W = mTexture->GetWidth() / frames;
			H = mTexture->GetHeight() / rows;
		 }
	 }

	 //
	 // Set the number of frames for each row of the sprite's texture.
	 // Some animations will have the same number of frames.
	 //
	 void CAnimate::SetMaxFrames(int r0, int r1, int r2, int r3, int r4, int r5, int r6, int r7, int r8, int r9)
	 {
		 mMaxFrames[0] = r0;
		 mMaxFrames[1] = r1;
		 mMaxFrames[2] = r2;
		 mMaxFrames[3] = r3;
		 mMaxFrames[4] = r4;
		 mMaxFrames[5] = r5;
		 mMaxFrames[6] = r6;
		 mMaxFrames[7] = r7;
		 mMaxFrames[8] = r8;
		 mMaxFrames[9] = r9;
	 }

	 void CAnimate::SetMaxFrames(int row, int max)
	 {
		mMaxFrames[row] = max;
	 }

	 void CAnimate::SetPos(int x, int y)
	{
		mXPos = x;
		mYPos = y;
;
	}
	 int CAnimate::GetPosX()
	{
		return(mXPos);
	}
	 int CAnimate::GetPosY()
	{
		return(mYPos);
	}
	 void CAnimate::SetPosX(int x)
	{
		mXPos = x;

	}
	 void CAnimate::SetPosY(int y)
	{

		mYPos = y;
;
	}

	 //
	 // Set the current visible frame for the animation.
	 //
	 void CAnimate::SetFrame(int n)
	{
		mCurFrame = n;
	}


	 //
	 // Start the specified sprite animation.
	 //
	void CAnimate::Start(int anim, bool loop)
	{
		mCurAnim = anim;
		mStartFrame = 0;
		mEndFrame = mMaxFrames[anim]; 
		mCurFrame = 0;
		mTicks = mDelay;
		mAnimate = true;
		mLoop = loop;
		mActive = true;
		mTexture->SetAlpha(0xFF);
		PlayAudio(anim);
	}

	 void CAnimate::Stop()
	{
		mAnimate = false;
	}


	 //
	 // Default Transition() method. This jsut moves the sprote off teh screen at the end of the animation.
	 // A class can override this and transition to different animations based on what state they are in.
	 //
	void CAnimate::Transition()
	{
		Reset();
		MoveOffScreen();
	}

	//
	// Performs thh animation by incrementing teh frame counter is the fame timer has elapsed.
	// If the animation isn't repeating then the Transition() method is called when the last frame is reached.
	//
	 bool CAnimate::Tick()
	 {
		 if (OldTime + FrameRate > SDL_GetTicks())
		 {
			 return(false);
		 }

		 OldTime = SDL_GetTicks();


		 if (mAnimate)
		 {
			 mCurFrame += FrameInc;
			 if (mFade)
			 {
				 Uint8 f = 0XFF - 128 * ((float)mCurFrame / (float)mMaxFrames[mCurAnim]);
				 if (mTexture!=NULL)
				 {
					 mTexture->SetAlpha(f);
				 }
			 }

			 if (Oscillate)
			 {
				 if (FrameInc > 0)
				 {
					 if (mCurFrame >= mMaxFrames[mCurAnim])
					 {
						 mCurFrame = mMaxFrames[mCurAnim] - 1;
						 FrameInc = -FrameInc;
					 }
				 }
				 else
				 {
					 if (mCurFrame <= 0)
					 {
						 mCurFrame = 0;
						 FrameInc = -FrameInc;
						 mAnimate = mLoop;
					 }
				 }
			 }
			 else
			 {
				 if (mCurFrame >= mMaxFrames[mCurAnim])
				 {
					 if (mWrapAnimation == true)
					 {
						 mCurFrame = 0;
						 mCurAnim++;

						 if (mCurAnim >= mMaxRows)
						 {
							 mCurAnim=0;
							 mAnimate = mLoop;
							 if (mAnimate == false)
							 {
								 Transition();
								 return(true);
							 }
						 }
					 }
					 else
					 {
						 mCurFrame = 0;
						 mAnimate = mLoop;
						 if (mAnimate == false)
						 {
							 Transition();
							 return(true);
						 }
					 }
				 }
			 }
		 }
		 return(false);
	 }

	 void CAnimate::OnAnimate()
	{
		//if (mAnimate == true)
		//{
		//	mTicks--;
		//	if (mTicks > 0)
		//		return(false);
		//	else
		//		mTicks = mDelay;


		//	mCurFrame++;
		//	if (mCurFrame > mEndFrame)
		//	{
		//		if (mLoop)
		//		{
		//			mCurFrame = mStartFrame;
		//		}
		//		else
		//		{
		//			mCurFrame = mStartFrame;
		//			Stop();
		//			Transition();
		//			return(true);
		//		}
		//	}
		//}

		//return(false);
	}

	 void CAnimate::SetFrameRate(int ms)
	 {
		 FrameRate = ms; // Set the number id milliseconds between frames.
	 }

	 void CAnimate::Move()
	{

	}

	 void CAnimate::Draw(SDL_Renderer *renderer)
	{
		// Draw the sprite to the screen.

		if (mView == true)
		{
			if (mTexture != NULL)
			{
				SDL_Rect src;

				// Determine if only part of the sprite needs to be draw. Used for progress bars.
				int scaledW = W;
				if (mXProgress != 1.0)
				{
					scaledW = (int)((float)W * mXProgress);
					if (scaledW == 0)
					{
						return;
					}
				}

				// Calculate the pixels to pull out of the texture.
				src = { mCurFrame*W, mCurAnim*H, scaledW, H };

				// SPecify where the pixels will go on the screen.
				SDL_Rect dst = { mXPos, mYPos, scaledW, H };

				// Send the pixels to the screen.
				mTexture->Draw(renderer, &src, &dst, 0.0);
				if (mShowHitBox)
				{
					// Draw the hitbox for debugging.
					mTexture->DrawBounds(renderer, mXPos, mYPos);
				}
			}
		}
	}



	 void CAnimate::AddAudio(Mix_Chunk *audio)
	 {
		 if (mNumAudio < 5)
		 {

			 mAudio[mNumAudio] = audio;
			 mNumAudio++;
		 }
	 }

	 void CAnimate::PlayAudio(int i)
	 {
		 if (i < 5)
		 {
			 if (mAudio[i] != NULL)
			 {
				 Mix_PlayChannel(-1, mAudio[i], 0);
			 }
		 }
	 }

	 void CAnimate::SetAlpha(Uint8 alpha)
	 {
		 // Set the transparency level of the sprite from 0 - 255.
		 if (mTexture != NULL)
		 {
			 mTexture->SetAlpha(alpha);
		 }
	 }

	 Uint8 CAnimate::GetAlpha()
	 {
		 
		 if (mTexture != NULL)
			 return(mTexture->GetAlpha());

		 return(0);
	 }