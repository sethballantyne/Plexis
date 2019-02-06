// Copyright(c) 2018 Seth Ballantyne <seth.ballantyne@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this
// software and associated documentation files(the "Software"), to deal in the Software
// without restriction, including without limitation the rights to use, copy, modify, merge,
// publish, distribute, sublicense, and / or sell copies of the Software, and to permit persons
// to whom the Software is furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in all copies or
// substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
// PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
// FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
// OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
#pragma once
#include "sprite.h"

/// <summary>
/// the explosions that are displayed when the explosive brick explodes.
/// </summary>
public ref class Explosion
{
private:
	Sprite ^sprite;

	// number frames in the explosion
	int frameCount;
	
	// true if the sprite has finished rendering all the frames, otherwise false.
	bool done = false;
public:
	/// <summary>
	/// Instantiates a new explosion at the specified tile coordinates.
	/// </summary>
	/// <param name="x">X tile coordinate</param>
	/// <param name="y">Y tile coordinate</param>
	Explosion(int x, int y)
	{
		int frameWidth = 41;
		int frameHeight = 20;
	
		Surface ^explosionSprite = ResourceManager::GetSurface("explosion");
		frameCount = explosionSprite->Size->Width / frameWidth;
		//System::Diagnostics::Debug::Assert(nullptr != spriteSheet && String::Empty != spriteSheet);

		array<Frame ^, 1> ^frames = gcnew array<Frame ^, 1>(frameCount);
		for(unsigned int i = 0; i < frameCount; i++)
		{
			System::Drawing::Rectangle frame(i * frameWidth, 0, frameWidth, frameHeight);
			frames[i] = gcnew Frame(frame, System::Drawing::Rectangle());
		}

		sprite = gcnew Sprite(x * frameWidth, y * frameHeight + 25, frames, explosionSprite);
	}

	/// <summary>
	/// Begins the EXPLOSION!!!! You have to call this before you render.
	/// </summary>
	void Start()
	{
		try
		{
			done = false;
			ResourceManager::GetSoundBuffer("explosion")->Play();
		}
		catch(...)
		{
			throw;
		}
	}

	/// <summary>
	/// Renders the current frame to the backbuffer.
	/// </summary>
	void Render()
	{
		if(!done)
		{
			sprite->Render();
			if(sprite->CurrentFrameIndex < frameCount)
			{
				if(sprite->CurrentFrameIndex + 1 == frameCount)
				{
					done = true;
				}
				else
				{
					sprite->CurrentFrameIndex++;
				}
			}
		}
	}

	/// <summary>
	/// Gets a value indicating whether the sprite has finished rendering all the frames.
	/// </summary>
	property bool Done
	{
		bool get() { return this->done; }
	}
};