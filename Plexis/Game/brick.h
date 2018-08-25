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
#include "entity.h"

/// <summary>
/// Red, Green and Yellow bricks are instances of this class.
/// The other bricks in the game use this as a base class.
/// </summary>
public ref class Brick : public Entity
{
private:
    // the amount of times the brick can be hit by a ball or an explosion
    // before it's destroyed. Each time a brick is hit by a brick or an explosion
    // this will decrease by one.
    unsigned int health = 0;

    // the chance a power up will spawn when this brick dies.
    // this is a percentage within the range of 0 to 100.
    unsigned int chanceOfPowerUpSpawning = 0;

    // the number of points awarded to the player when the brick is destroyed.
    unsigned int pointsAwardedOnDeath = 0;

public:
    /// <summary>
    /// Initialises a new instance of <see cref="Brick"/>.
    /// </summary>
    /// <param name="sprite">The <see cref="Sprite"/> instance that encapsulates this bricks 
    /// <see cref="Surface"/> and frame data.</param>
    /// <param name="health">the number of hits the brick can take from a ball or explosion before
    /// it's destroyed.</param>
    /// <param name="chanceOfPowerUpSpawning">the chance a powerup will spawn when the brick is destroyed. 
    /// This is a percentage between 1 and 100.</param>
    /// <param name="pointsAwarded">the number of added to the players score when this brick is destroyed.</param>
    Brick(::Sprite ^sprite, unsigned int health, unsigned int chanceOfPowerUpSpawning, 
        unsigned int pointsAwarded) : Entity(sprite, Vector2(0))
    {
        this->health = health;
        this->chanceOfPowerUpSpawning = chanceOfPowerUpSpawning;
        pointsAwardedOnDeath = pointsAwarded;
    }

    /// <summary>
    /// Creates a deep copy of this <see cref="Brick"/> instance.
    /// </summary>
    /// <returns></returns>
    Object ^Clone() override
    {
        ::Sprite ^sprite = gcnew ::Sprite(
            this->Position.X, 
            this->Position.Y, 
            this->Sprite->GetFrames(), this->Sprite->Surface);

        return gcnew Brick(sprite, this->health, this->chanceOfPowerUpSpawning, this->pointsAwardedOnDeath);
    }

    /// <summary>
    /// Updates the bricks state.
    /// </summary>
    void Update() override
    {
    }

    /// <summary>
    /// Gets or sets the bricks health. A value of zero infers death.
    /// </summary>
    property unsigned int Health
    {
        unsigned int get()
        {
            return health;
        }

        void set(unsigned int value)
        {
            health = value;
        }
    }

    /// <summary>
    /// Gets the percentage used to determine whether a powerup is generated when 
    /// this brick is destroyed.
    /// </summary>
    property unsigned int PowerUpSpawnPercentage
    {
        unsigned int get()
        {
            return chanceOfPowerUpSpawning;
        }
    }

    /// <summary>
    /// Gets the number points that will be added to the players score when 
    /// this brick is destroyed.
    /// </summary>
    property unsigned int PointValue
    {
        unsigned int get()
        {
            return pointsAwardedOnDeath;
        }
    }
};

