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
#include "particle.h"


///<summary>
/// Object for creating particle effects representing explosions.
///</summary>
public ref class ExplosionParticleEffect
{
	System::Collections::Generic::List<Particle ^>^ particles;

public:
	///<summary>
	/// initialises the explosion by creating the particles and assigning an initial velocity.
	///</summary>
	///<param name="x">the initial x position of each particle.</param>
	///<param name="y">the initial y postion of each particle</param>
	///<param name="numberOfParticles">the amount of particles the explosion consists of.
	/// depending on what you're aiming for, 25 is a good starting point.</param>
	///<param name="maxXVelocity">the initial velocity on the X axis that the particle shouldn't
	/// exceed.</param>
	///<param name="maxYVelocity">the initial velocity on the Y axis that the particle shouldn't
	/// exceed.</param>
	///<param name="R">The red component of the particles colour.</param>
	///<param name="G">The green component of the particles colour.</param>
	///<param name="B">the blue component of the particles colour.</param>
	ExplosionParticleEffect(int x, int y, unsigned int numberOfParticles, int maxXVelocity, int maxYVelocity,
							unsigned char R, unsigned char G, unsigned char B)
	{
		const float PI_RADIANS = 3.1459 / 180.0f;
		Random ^rng = gcnew Random();
		particles = gcnew System::Collections::Generic::List<Particle ^>(numberOfParticles);
		Vector2 velocity;
		
		for(unsigned int i = 0; i < numberOfParticles; i++)
		{
			float angle = rng->Next(1, 359) * PI_RADIANS;
			velocity.X = rng->Next(1, maxXVelocity) / cos(angle);
			velocity.Y = rng->Next(1, maxYVelocity) / sin(angle);

			particles->Add(gcnew Particle(x, y, velocity, R, G, B));
		}
	}

	///<summary>
	/// Gets the number of the particles remaining in the list
	///</summary>
	property int ParticleCount
	{
		int get()
		{
			return particles->Count;
		}
	}

	///<summary>
	/// Causes each particle in the explosion to update their position and colour.
	///</summary>
	void Update()
	{
		for(int i = particles->Count - 1; i >= 0; i--)
		{
			particles[i]->Update();
			if(particles[i]->IsOffScreen() || particles[i]->IsColour(0, 0, 0))
			{
				particles->RemoveAt(i);
			}
		}
	}

	/// <summary>
	/// Renders the particle effect to the back buffer.
	/// NOTE: the video surface MUST be locked before calling this and unlocked
	/// after its completed. 
	/// </summary>
	void Render()
	{
		for(int i = 0; i < particles->Count; i++)
		{
			particles[i]->Render();
		}
	}
};