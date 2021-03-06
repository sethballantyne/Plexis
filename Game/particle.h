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
#include "vector2.h"
#include "video.h"

///<summary>
/// Represents a square of 4 pixels used in particle effects.
///</summary>
public ref class Particle
{
private:
	// the 32bit colour of the particle; only used when rendering.
	unsigned int Colour;

	// the amount each RGB value will be decremented by, each frame.
	const unsigned char decrementAmount = 5;

	// decrease the specified value, ensuring it doesn't underflow. 
	// for use with the R,G,B values.
	inline void Decrease(unsigned char %value)
	{
		if(value > 0)
		{
			if(value - decrementAmount < 0)
			{
				value -= (0 + value);
			}
			else
			{
				value -= decrementAmount;
			}
		}
	}

	bool IsOffScreen(int x, int y)
	{
		bool expr1 = x >= Video::Width || Position.X < 0;
		bool expr2 = y >= Video::Height || Position.Y < 0;

		return expr1 || expr2;
	}

public:
	Vector2 Velocity;

	// Screen position of the particle
	System::Drawing::Point Position;

	// colour of the pixel.
	// pixels are rendered in 32 bit colour.
	unsigned char R, G, B;

	///<summary>
	/// initialises the particle with the specified coordinates, velocity and colour.
	///</summary>
	///<param name="x">the initial position on the screen, x axis.</param>
	///<param name="y">the initial position on the screen, y axis.</param>
	///<param name="velocity">the particles initial velocity.</param>
	///<param name="R">initial red component of the particles colour.</param>
	///<param name="G">initial green component of the particles colour.</param>
	///<param name="B">initial blue component of the particles colour.</param>
	Particle(int x, int y, Vector2 velocity, unsigned char R, unsigned char G, unsigned char B)
	{
		this->Position.X = x;
		this->Position.Y = y;
		this->Velocity = velocity;
		this->R = R;
		this->G = G;
		this->B = B;
		this->Colour = Video::ARGBTo32Bit(255, R, G, B);
	}

	///<summary>
	/// initialises the particle with the specified coordinates, velocity and colour.
	///</summary>
	///<param name="position">the initial position on the screen.</param>
	///<param name="velocity">the particles initial velocity.</param>
	///<param name="R">initial red component of the particles colour.</param>
	///<param name="G">initial green component of the particles colour.</param>
	///<param name="B">initial blue component of the particles colour.</param>
	Particle(System::Drawing::Point position, Vector2 velocity, unsigned char R, unsigned char G, unsigned char B) 
	{
		this->Position = position;
		this->Velocity = velocity;
		this->R = R;
		this->G = G;
		this->B = B;
		this->Colour = Video::ARGBTo32Bit(255, R, G, B);
	}

	///<summary>
	/// Returns true if the pixel is the specified colour, else returns false.
	///</summary>
	bool IsColour(unsigned char R, unsigned G, unsigned B)
	{
		return ((this->R == R) && (this->G == G) && (this->B == B));
	}

	///<summary>
	/// Returns true if the particle is off screen, else returns false.
	///</summary>
	bool IsOffScreen()
	{
		bool expr1 = Position.X >= Video::Width || Position.X < 0;
		bool expr2 = Position.Y >= Video::Height || Position.Y < 0;
		bool expr3 = Position.X + 1 >= Video::Width || Position.X + 1 < 0;
		bool expr4 = Position.Y + 1 >= Video::Height || Position.Y + 1 < 0;

		return expr1 && expr2 && expr3 && expr4;
	}

	

	///<summary>
	/// Updates the particles position and colour.
	///</summary>
	void Update()
	{
		Position.X += Velocity.X;
		Position.Y += Velocity.Y;

		Decrease(R);
		Decrease(G);
		Decrease(B);

		Colour = Video::ARGBTo32Bit(255, R, G, B);
	}

	///<summary>
	/// Renders the pixels to the screen. 
	/// LOCK THE SCREEN before using.
	///</summary>
	void Render()
	{
		// it's asssumed the surface is being locked elsewhere, since doing it 
		// here would be dumb.
		//Video::DrawPixel(Position.X, Position.Y, Colour);
		for(int i = Position.X; i < Position.X + 2; i++)
		{
			for(int j = Position.Y; j < Position.Y + 2; j++)
			{
				if(!IsOffScreen(i, j))
				{
					Video::DrawPixel(i, j, Colour);
				}
			}
		}
	}
};