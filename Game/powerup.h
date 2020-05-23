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


delegate void PowerUpEffectHandler(Object ^sender, EventArgs ^args);

///<summary>
/// Base class for all powerups within the game.
///</summary>
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

	///<summary>
	///<param name="x">the initial x position on the screen.</param>
	///<param name="y">the initial y position on the screen.</param>
	///<param name="angle">the powerups inital angle of movement.</param>
	///</summary>
	void Spawn(int x, int y, float angle)
	{
		Angle = angle;
		Velocity.X = Speed * cos(angle);
		Velocity.Y = Speed * sin(angle);
		Sprite->Position.X = x;
		Sprite->Position.Y = y;
	}

	///<summary>
	/// User has pressed the fire button while the power up is active. 
	/// Triggers the FirePressed event.
	///</summary>
	virtual void Fired()
	{
		EventArgs ^eventArgs = gcnew EventArgs();
		FirePressed(this, eventArgs);
	}

	///<summary>
	/// Triggers the CollisionWithPaddle event.
	/// Should be called when the powerup collides with the paddle.
	///</summary>
	virtual void PlayerCollision()
	{
		EventArgs ^eventArgs = gcnew EventArgs();
		CollisionWithPaddle(this, eventArgs);
	}


	void Update() override
	{
	}

	///<summary>
	/// Updates the postion of the power up.
	///</summary>
	///<param name="x">the top left coordinate of the viewport, x axis.</param>
	///<param name="y">the top left coordinate of the viewport, y axis.</param>
	///<param name="width">the width of the view port in pixels.</param>
	///<param name="height">the height of the view port in pixels.</param>
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

	Object ^Clone() override
	{
		::Sprite ^sprite = gcnew ::Sprite(
			this->Sprite->Position.X,
			this->Sprite->Position.Y,
			this->Sprite->GetFrames(), this->Sprite->Surface);

		return gcnew PowerUp(sprite, Vector2::Zero, this->Name);
	}

	// triggers when PlayerCollision() is called.
	// represents when the powerup is collected by the paddle.
	event PowerUpEffectHandler^ CollisionWithPaddle;

	// triggers when Fired() is called.
	// Used for when the player presses the fire button when the powerup is active.
	event PowerUpEffectHandler^ FirePressed;
};