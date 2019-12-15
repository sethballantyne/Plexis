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
#include <math.h>
#include "entity.h"
#include "powerupeffectargs.h"

delegate void PowerUpEffectHandler(Object ^sender, EventArgs ^args);

public ref class PowerUp : public Entity
{
	/// <summary>
	/// Causes the powerup to bounce off the edges of the viewport.
	/// </summary>
	/// <param name="x">the screen coordinates of the viewports top left point on the X axis.</param>
	/// <param name="y">the screen coordinates of the viewports top left point on the Y axis.</param>
	/// <param name="width">How wide the viewport is, in pixels.</param>
	/// <param name="height">How high the viewport is, in pixels.</param>
	/// <remarks>
	/// Within plexis, the viewport is the entire screen.
	/// </remarks>
	void HandleEdgeCollisions(int x, int y, int width, int height)
	{
		int frameWidth = Sprite->CurrentFrame->Coordinates.Width;
		int frameHeight = Sprite->CurrentFrame->Coordinates.Height;

		//---------------------------------------------------------------------
		// check to see if the sprite has moved outside the viewport at all
		//---------------------------------------------------------------------
		if(Sprite->Position.X <= x)	// left edge of the viewport
		{
			SetPosition(x, Sprite->Position.Y);
			Velocity.X = -Velocity.X;
		}

		else if(Sprite->Position.X + frameWidth >= width) // right edge of the viewport
		{
			// move it so the right edge of the powerup is aligned with the right edge of the viewport
			SetPosition(width - frameWidth, Sprite->Position.Y);
			Velocity.X = -Velocity.X;
		}

		if(Sprite->Position.Y <= y)
		{
			SetPosition(Sprite->Position.X, y);

			Velocity.Y = -Velocity.Y;
			if(Velocity.Y < 0)
			{
				Velocity.Y = 1.0f;
			}

		}
	}

public:
	// the rate at which the object will fall to the "ground"
	// the greater the value, the heavier it is, which affects how far it'll travel
	// before falling and the speed at which it falls. 
	property float Gravity;

	property float Angle;

	PowerUp(::Sprite ^powerupSprite, Vector2 velocity, System::String ^name) : Entity(powerupSprite, Vector2::Zero, name)
	{
		Gravity = 0.1f;
		Speed = 7.0f;
	}

	void Spawn(int x, int y, float angle)
	{
		Angle = angle;
		Velocity.X = Speed * cos(angle);
		Velocity.Y = Speed * sin(angle);
		Sprite->Position.X = x;
		Sprite->Position.Y = y;
	}

	virtual void Fired()
	{
		EventArgs ^eventArgs = gcnew EventArgs();
		FirePressed(this, eventArgs);
	}

	virtual void PlayerCollision()
	{
		EventArgs ^eventArgs = gcnew EventArgs();
		CollisionWithPaddle(this, eventArgs);
	}


	void Update() override
	{
	}

	void Update(int x, int y, int width, int height)
	{
		int xPos = this->Sprite->Position.X;
		int yPos = this->Sprite->Position.Y;

		xPos += Velocity.X;
		yPos += Velocity.Y;
		Velocity.Y += Gravity;

		SetPosition(xPos, yPos);

		HandleEdgeCollisions(x, y, width, height);
	}

	event PowerUpEffectHandler^ CollisionWithPaddle;
	event PowerUpEffectHandler^ FirePressed;
};