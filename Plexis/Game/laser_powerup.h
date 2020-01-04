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
#include "powerup.h"

///<summary>
/// Represents an instance of the laser powerup that can be spawned when a brick is destroyed.
///</summary>
public ref class LaserPowerUp : public PowerUp
{
public:
	// how long the powerup lasts for when caught by the player.
	property unsigned int Duration;

	// how much damage is done by each individual laser when it hits a brick.
	// Damage is translated in hits, so if a laser does two damage, that's the
	// equivalent of two hits.
	property unsigned int LaserDamage;

	// the velocity of each laser beam shot by the paddle.
	property int LaserVelocity;

	///<summary>
	/// <param name="powerupSprite">The sprite instance containing the graphic that'll be rendered
	/// to the screen when the powerup is drawn.</param>
	/// <param name="duration"> how long the powerup's effect lasts for when caught by the player.</param>
	/// <param name="laserVelocity">the velocity of the laser as it fires from the paddle</param>
	/// <param name="laserDamage">the amount of damage the laser does when it hits a brick.</param>
	/// <param name="name">the entities name as specified in its xml "name" attribute.</param>
	///</summary>
	LaserPowerUp(::Sprite ^powerupSprite, unsigned int duration, int laserVelocity, 
				 unsigned int laserDamage, System::String ^name) : PowerUp(powerupSprite, Vector2::Zero, name)
	{
		Duration = duration;
		LaserDamage = laserDamage;
		LaserVelocity = laserVelocity;
	}

	///<summary>
	/// Creates a deep copy of the object.
	///</summary>
	Object ^Clone() override
	{
		::Sprite ^sprite = gcnew ::Sprite(
			this->Sprite->Position.X,
			this->Sprite->Position.Y,
			this->Sprite->GetFrames(), this->Sprite->Surface);

		return gcnew LaserPowerUp(sprite, Duration, LaserVelocity, LaserDamage, this->Name);
	}
};