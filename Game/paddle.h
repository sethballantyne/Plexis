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
#include "entity.h"
#include "vector2.h"
#include "ball.h"

public ref class Paddle : public Entity
{
public:
	property bool IsDead;

	Paddle(::Sprite ^sprite, String ^name) : Entity(sprite, Vector2::Zero, name)
	{
		
	}

	void AttachBall(Ball ^ball)
	{
		int ballWidth = ball->Sprite->CurrentFrame->Coordinates.Width;
		int middleOfPaddle = this->Sprite->CurrentFrame->Coordinates.Width / 2;

		// ballX and ballY are local coordinates (coordinates on the paddle), not screen coordinates.
		int ballX = middleOfPaddle - (ballWidth / 2);
		int ballY = this->Sprite->CurrentFrame->Coordinates.Y - ball->BoundingBox.Height;

		this->Attach(ball, ballX, ballY);
	}

	/// <summary>
	/// Creates a deep copy of this <see cref="Paddle"/> instance.
	/// </summary>
	/// <returns></returns>
	Object ^Clone() override
	{
		::Sprite ^sprite = gcnew ::Sprite(
			this->Sprite->Position.X,
			this->Sprite->Position.Y,
			this->Sprite->GetFrames(), this->Sprite->Surface);

		return gcnew Paddle(sprite, this->Name);
	}

	/// <summary>
	/// Puts the paddle in the middle of the screens X axis.
	/// </summary>
	inline void ResetPosition()
	{
		int x = (Video::Width / 2) - (this->Sprite->CurrentFrame->Coordinates.Width / 2);
		this->SetPosition(x);
	}

	inline void SetPosition(int x)
	{
		this->SetPosition(x, this->Sprite->Position.Y);
	}

	void FirePressed();

	/// <summary>
	/// Updates the bricks state.
	/// </summary>
	void Update() override
	{
	}
};