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
#include "level.h"
#include "paddle.h"
#include "ball.h"
#include "gamestate.h"
#include "input.h"
#include "levelmanager.h"
#include "gameoptions.h"
#include "numericfield.h"
#include "gameover.h"
#include "explosion.h"
#include "laser_powerup.h"
#include "laser.h"
#include "explosion_particle_effect.h"
#include "instadeath_powerup.h"
#include "bonuspoints_powerup.h"
#include "extralife_powerup.h"

using namespace System::Diagnostics;
using namespace System::Timers;

// 0 based
#define DEFAULT_NUMBER_OF_LIVES 2

const float PI_RADIANS = 3.1459 / 180.0f;

/// <summary>
/// Game play logic.
/// </summary>
public ref class GameLogic
{
private:
	// the current level that's being played
	Level ^currentLevel;

	// the player controlled paddle
	Paddle ^player;

	// The ball the player beats about the screen
	Ball ^ball;

	// the image that's displayed when the user presses the pause key during gameplay.
	Surface ^pauseImage = nullptr;

	// the image that's displayed when the user has destroyed all the bricks in the level.
	Surface ^levelCompleteImage = nullptr;

	// the image next to the lives counter.
	// indicates that the value we're gawking at corresponds to lives.
	Surface ^livesImage = nullptr;

	// the timer image used when a powerup is in effect
	Surface ^timerImage = nullptr;

	// the amount of points the player has
	NumericField ^score;

	// the number lives remaining
	NumericField ^lives;

	// number of seconds remaining when the laser power up is in use.
	NumericField ^powerUpTimerValue;

	NumericField ^ammoCount;

	// the delay that's present when transitioning between levels.
	Timer ^levelLoadDelayTimer = gcnew Timer(5000);

	// timer used to countdown the number of seconds remaining
	// while the laser powerup is in use.
	Timer ^laserActiveTimer = gcnew Timer(1000);

	// used for the delay when the ball goes off the screen
	Timer ^playerResetTimer = gcnew Timer(2000);

	// the scene that should be displayed when the user hits escape during gameplay.
	String ^gameInProgressMainMenu;

	// 
	//String ^highScorePrompt;

	// the screen that's displayed when the user runs out of lives.
	GameOverScreen ^gameOverScreen = gcnew GameOverScreen();

	// controls game flow and what's displayed on the screen. 
	GameState gameState = GameState::NewLevel;

	// list of explosions that need to be rendered when an explosive brick
	// is hit by the ball or destroyed in another explosion.
	List<Explosion ^> ^explosionList = gcnew List<Explosion ^>();

	// list of powerups moving around on the screen
	List<PowerUp ^> ^powerUpList = gcnew List<PowerUp ^>();

	// lasers fired when the user has the laser powerup
	List<Laser ^> ^laserList = gcnew List<Laser ^>();

	// particle effects created by powerups
	List<ExplosionParticleEffect ^> ^particleEffectsList = gcnew List<ExplosionParticleEffect ^>();

	// used for powerups that are caught by the play and have an effect for a specific
	// duration. Insta-death, the bonus points powerups and extra life powerup don't use this.
	PowerUp ^powerUpInEffect = nullptr;

	// the name of the level if /map was passed via command line.
	String ^testLevel = nullptr;

	Random ^randomNumberGen = gcnew Random();

	LaserPowerUp ^laser = nullptr;

	// keys read from options.xml
	int pauseKey;
	int playerFireKey;

	// x and y positions for the image that's displayed when the player
	// presses pause.
	int pauseX;
	int pauseY;

	// x and y coordinates for the image that's displayed when the player
	// completes a level.
	int levelCompleteX;
	int levelCompleteY;

	// the number of bricks the user has to hit in order to complete the level
	int numberOfBricksRemaining;

	/*const int initialAmmo = 90;
	const int ammoPacks = initialAmmo / 3;*/

	bool debugKeysEnabled = false;

	bool gameOverScreenVisible = false;

	//bool powerUpInEffect = false;

	/// <summary>
	/// Removes a brick from the game, as if it were hit by the ball.
	/// </summary>
	void DebugRemoveBrick();

	///<summary>
	/// Disables any active powerups and clears the lists containing powerup effects or
	/// powerups moving around the screen.
	///</summary>
	void DisablePowerUps(bool clearLists)
	{
		powerUpInEffect = nullptr;
		powerUpTimerValue->Value = 30;
		laserActiveTimer->Enabled = false;

		if(clearLists)
		{
			powerUpList->Clear();
			laserList->Clear();
		}
	}

	/// <summary>
	/// Resets the paddle and ball to their default positions.
	/// </summary>
	void ResetPlayerAndBall()
	{
		player->ResetPosition();
		this->player->RemoveAttachments();
		this->player->AttachBall(ball);
		this->player->IsDead = false;
	}

	/// <summary>
	/// Handles mouse and keyboard input during gameplay
	/// </summary>
	/// <param name="keyboardState"></param>
	/// <param name="mouseState"></param>
	void HandleGameStateInput(Keys ^keyboardState, Mouse ^mouseState);

	/// <summary>
	/// Ball collision detection with walls and the player
	/// </summary>
	void HandleBallCollision();

	/// <summary>
	/// Player collision detection
	/// </summary>
	void HandlePlayerWallCollision();

	/// <summary>
	/// Ball collision detection with bricks
	/// </summary>
	void HandleBrickCollisions();


	/// <summary>
	/// Handles all the collision detection within the game
	/// </summary>
	void HandleCollisions()
	{
		HandlePlayerWallCollision();

		// don't perform collision detection for the ball if the 
		// ball has transitioned off the screen and we're currently within
		// the delay that's present before the paddle and ball are reset.
		if(this->gameState != GameState::PlayerReset)
		{
			HandleBallCollision();
			HandleBrickCollisions();
		}
	}

	/// <summary>
	/// Input that controls or affects gameplay.
	/// </summary>
	/// <param name="keyboardState"></param>
	/// <param name="mouseState"></param>
	void HandleGameInput(Keys ^keyboardState, Mouse ^mouseState)
	{
		player->Velocity.X = mouseState->X;
		if(keyboardState->KeyPressed(DIK_SPACE))
		{
			// test shit here
			SpawnPowerUp(640, 480);
		}

		if(keyboardState->KeyPressed(DIK_L))
		{
			SpawnLaserPowerUp(300, 300, 45);
		}

		if(keyboardState->KeyPressed(DIK_H))
		{
			lives->Value += 100;
		}

		if(keyboardState->KeyPressed(DIK_S))
		{
			score->Value += 100000;
		}

		if(mouseState->ButtonPressed(playerFireKey) || keyboardState->KeyPressed(playerFireKey))
		{
			if(ball->Attached)
			{
				player->FirePressed();
			}
			
			else if(ammoCount->Value > 0)
			{
				laser->Fired();
				ammoCount->Value--;
			}
		}
	}

	///<summary>
	/// Collision detection for when lasers hit bricks.
	///</summary>
	void HandleLaserBrickCollisions(Laser ^laser)
	{
		for(int i = 0; i < currentLevel->Width; i++)
		{
			for(int j = 0; j < currentLevel->Height; j++)
			{
				if(nullptr != currentLevel[i, j] && currentLevel[i, j]->Visible)
				{
					if(laser->BoundingBox.IntersectsWith(currentLevel[i, j]->BoundingBox))
					{
						currentLevel[i, j]->Hit(i, j, BRICK_HIT_BY_LASER);
						laserList->Remove(laser);
						//ExplodeBrickWithLaser(currentLevel[i, j], 255, 255, 255);
					}
				}
			}
		}
	}

	///<summary>
	/// Resets the paddle so it's visible and in the middle of the screen, with the ball attached.
	/// Any previously active powerups are disabled.
	///</summary>
	void SpawnPlayer()
	{
		player->ResetPosition();
		this->player->RemoveAttachments(); // this was supposed to be used when the player
		// had a laser gun attached to the paddle or there
		// was a powerup active that caused the ball(s) to stick
		// (FEAR MY STICKY BALLS!!!) to the paddle. Probably redundant now.
		this->player->AttachBall(ball);

		this->player->IsDead = false;

		// disable any powerups currently in use.
		DisablePowerUps(true);
	}

public:
	GameLogic(String ^gameInProgressMenu);

	void Update(Keys ^keyboardState, Mouse ^mouseState);

	/// <summary>
	/// Gets the key configuration from GameOptions.
	/// </summary>
	void UpdateKeys()
	{
		// defaults should never be generated unless it's due to programmer error.
		// GameOptions is searched at start up for the below keys and defaults
		// provided if they don't exist (see Game::Initialise()).
		// if the below defaults do come into play, they won't be reflected in the 
		// options screen; but again, shouldn't happen.
		this->playerFireKey = GameOptions::GetValue("fireKey", 0);
		this->pauseKey = GameOptions::GetValue("pauseKey", DIK_P);
	}

	///<summary>
	/// Moves any particle effect currently on the screen.
	///</summary>
	void UpdateParticleEffects()
	{
		for(int i = particleEffectsList->Count - 1; i >= 0; i--)
		{
			particleEffectsList[i]->Update();
			if(0 == particleEffectsList[i]->ParticleCount)
			{
				particleEffectsList->RemoveAt(i);
			}
		}
	}

	/// <summary>
	/// Renders the gameplay to the screen
	/// </summary>
	void Render();

	/// <summary>
	/// Puts the game into its default state.
	/// </summary>
	void NewGame()
	{
		this->gameState = GameState::NewLevel;
		this->score->Value = 0;
		this->lives->Value = DEFAULT_NUMBER_OF_LIVES;
		gameOverScreen->Visible = false;

		LevelManager::ResetLevelCounter();
	}

	/// <summary>
	/// Creates an explosion at the specified tile coordinates.
	/// </summary>
	/// <param name="x"></param>
	/// <param name="y"></param>
	void CreateExplosion(int x, int y)
	{
		Explosion ^booomMotherFuckerApostrophe = gcnew Explosion(x, y);
		booomMotherFuckerApostrophe->Start();
		explosionList->Add(booomMotherFuckerApostrophe);
	}

	/// <summary>
	/// Causes all bricks within a 1 brick radius of the specified coords to explode.
	/// </summary>
	/// <param name="coords">the center of the explosion, in tile coordinates.</param>
	void ExplodeSurroundingBricks(System::Drawing::Point coords)
	{
		// now make any other bricks around it explode!
		for(int x = -1; x < 2; x++)
		{
			for(int y = -1; y < 2; y++)
			{
				int xCoord = coords.X + x;
				int yCoord = coords.Y + y;

				if(xCoord >= 0 &&
				   xCoord < currentLevel->Width &&
				   yCoord >= 0 &&
				   yCoord < currentLevel->Height &&
				   nullptr != currentLevel[xCoord, yCoord] &&
				   currentLevel[xCoord, yCoord]->Visible)
				{
					// KNOCK KNOCK, YOU'RE DEAD!
					currentLevel[xCoord, yCoord]->Die(xCoord, yCoord, BRICK_EXPLODE);
				}
			}
		}
	}

	///<summary>
	/// Creates an explosion particle effect at the specified brick.
	///</summary>
	void ExplodeBrick(Brick^ b, unsigned char R, unsigned char G, unsigned char B)
	{
		int brickHalfWidth = b->BoundingBox.Width / 2;
		int brickHalfHeight = b->BoundingBox.Height / 2;
		int explosionX = b->BoundingBox.Right - brickHalfWidth;
		int explosionY = b->BoundingBox.Bottom - brickHalfHeight;

		int x = randomNumberGen->Next(10, 25);
		int y = randomNumberGen->Next(10, 25);
		particleEffectsList->Add(gcnew ExplosionParticleEffect(explosionX, explosionY, 35, x, y, R, G, B));

		ResourceManager::GetSoundBuffer("explosion")->Stop();
		ResourceManager::GetSoundBuffer("explosion")->Play();
	}

	///<summary>
	///Creates the effect that makes the particle look like it has exploded.
	///</summary>
	void ExplodePaddle()
	{
		player->IsDead = true;

		int paddleHalfWidth = player->BoundingBox.Width / 2;
		int paddleHalfHeight = player->BoundingBox.Height / 2;
		int explosionX = player->BoundingBox.Right - paddleHalfWidth;
		int explosionY = player->BoundingBox.Bottom - paddleHalfHeight;

		particleEffectsList->Add(gcnew ExplosionParticleEffect(explosionX, explosionY, 75, 25, 25, 255, 255, 255));

		// a different sound effect plays if the ball goes off screen.
		/*if(!ballWentOffScreen)
		{
		ResourceManager::GetSoundBuffer("paddle_explosion")->Play();
		}*/
		ResourceManager::GetSoundBuffer("loselife")->Play();
		/*else
		{
		this->gameState = GameState::GameOver;
		}*/

		playerResetTimer->Start();
		laserActiveTimer->Stop();

	}

	/// <summary>
	/// Creates a random powerup at the specified coordinates.
	///</summary>
	void SpawnPowerUp(int x, int y)
	{
		int value = randomNumberGen->Next(0, EntityManager::NumberOfPowerUps);
		float angle = randomNumberGen->Next(220, 340) * PI_RADIANS;

		switch(value)
		{
			case 0:
				SpawnLaserPowerUp(x, y, angle);
				break;

			case 1:
				SpawnInstaDeathPowerUp(x, y, angle);
				break;

			case 2:
				SpawnBonusPointPowerUp("bp50_powerup", x, y, angle);
				break;

			case 3:
				SpawnBonusPointPowerUp("bp100_powerup", x, y, angle);
				break;

			case 4:
				SpawnBonusPointPowerUp("bp200_powerup", x, y, angle);
				break;

			case 5:
				SpawnBonusPointPowerUp("bp500_powerup", x, y, angle);
				break;

			case 6:
				SpawnExtraLifePowerUp(x, y, angle);
			break;
		}
	}

	///<summary>
	/// Creates a laser power up at the specified screen coordinates.
	/// The angle determines the angle of the powerups projection as it flies upwards before
	/// falling towards the player.
	///</summary>
	void SpawnLaserPowerUp(int x, int y, float angle)
	{
		LaserPowerUp ^laser = EntityManager::GetEntity<LaserPowerUp ^>("laser_powerup");

		laser->FirePressed += gcnew PowerUpEffectHandler(this, &GameLogic::OnFirePressed_LaserPowerUp);
		laser->CollisionWithPaddle += gcnew PowerUpEffectHandler(this, &GameLogic::OnCollisionWithPaddle_LaserPowerUp);
		laser->Spawn(x, y, angle);

		powerUpList->Add(safe_cast<PowerUp ^>(laser));
	}

	/// <summary>
	/// Creates an instadeath powerup at the specified screen coordinates. 
	/// The angle determines the angle of the powerups projection as it flies upwards before
	/// falling towards the player.
	/// </summary>
	void SpawnInstaDeathPowerUp(int x, int y, float angle)
	{
		InstaDeathPowerUp ^instaDeath = EntityManager::GetEntity<InstaDeathPowerUp ^>("instadeath_powerup");

		instaDeath->CollisionWithPaddle += gcnew PowerUpEffectHandler(this, &GameLogic::OnCollisionWithPaddle_InstaDeathPowerUp);
		instaDeath->Spawn(x, y, angle);

		powerUpList->Add(safe_cast<PowerUp ^>(instaDeath));
	}

	///<summary>
	/// Creates a bonus point powerup at the specified screen coordinates. 
	/// The angle determines the angle of the powerups projection as it flies upwards before
	/// falling towards the player.
	///</summary>
	void SpawnBonusPointPowerUp(String ^pickupName, int x, int y, float angle)
	{
		BonusPointsPowerUp ^bonusPointsPowerUp = EntityManager::GetEntity<BonusPointsPowerUp ^>(pickupName);

		bonusPointsPowerUp->CollisionWithPaddle += gcnew PowerUpEffectHandler(this, &GameLogic::OnCollisionWithPaddle_BonusPointsPowerUp);
		bonusPointsPowerUp->Spawn(x, y, angle);

		powerUpList->Add(safe_cast<PowerUp ^>(bonusPointsPowerUp));
	}

	///<summary>
	/// Creates an extra life powerup at the specified screen coordinates. 
	/// The angle determines the angle of the powerups projection as it flies upwards before
	/// falling towards the player.
	///</summary>
	void SpawnExtraLifePowerUp(int x, int y, float angle)
	{
		ExtraLifePowerUp ^extraLifePowerUp = EntityManager::GetEntity<ExtraLifePowerUp ^>("extralife_powerup");

		extraLifePowerUp->CollisionWithPaddle += gcnew PowerUpEffectHandler(this, &GameLogic::OnCollisionWithPaddle_ExtraLifePowerUp);
		extraLifePowerUp->Spawn(x, y, angle);

		powerUpList->Add(safe_cast<PowerUp ^>(extraLifePowerUp));
	}

	///<summary>
	/// Renders all existing particle effects within the particle effects list to the back buffer.
	///</summary>
	/// <exception cref="DirectDrawInvalidObjectException">DirectDraw received a pointer that was an invalid DirectDraw object.</exception>
	/// <exception cref="DirectDrawInvalidParametersException">one or more of the parameters passed to the method are incorrect.</exception>
	/// <exception cref="OutOfMemoryException">Not enough memory available to complete the operation.</exception>
	/// <exception cref="DirectDrawSurfaceBusyException">access to the surface is refused because the surface is locked by another thread.</exception>
	/// <exception cref="DirectDrawSurfaceLostException">access to the surface is refused because the surface memory is gone.</exception>
	/// <exception cref="DirectDrawWasStillDrawingException">the previous blit operation is incomplete.</exception>
	/// <exception cref="DirectDrawInvalidRectException">the rectangle coordinates used by the surface were invalid.</exception>
	/// <exception cref="DirectDrawNotLockedException">Attempted to unlock a surface that wasn't locked.</exception>
	/// <exception cref="System::Runtime::InteropServices::COMException">Generic error returned by DirectDraw when a more specific error isn't provided.</exception>
	void RenderParticleEffects()
	{
		try
		{
			Video::LockSurface();

			// render particle effects
			for(int i = 0; i < particleEffectsList->Count; i++)
			{
				particleEffectsList[i]->Render();
			}

			Video::UnlockSurface();
		}
		catch(...)
		{
			throw;
		}
	}

	///<summary>
	/// Renders any powerups bouncing about the screen, along with any power up effects (such as lasers)
	/// that are currently within the relevant list.
	///</summary>
	void GameLogic::RenderPowerUps()
	{
		for(int i = 0; i < powerUpList->Count; i++)
		{
			powerUpList[i]->Sprite->Render();
		}

		// power up effects
		for(int i = 0; i < laserList->Count; i++)
		{
			laserList[i]->Sprite->Render();
		}

		// timer shit
		if(laserActiveTimer->Enabled)
		{
			powerUpTimerValue->Render();
		}
	}

	///<summar>
	/// Updates the state of any powerups currently visible on the screen, along with any effects
	/// that happen to be active.
	///</summary>
	void GameLogic::UpdatePowerUps()
	{
		for(int i = powerUpList->Count - 1; i >= 0; i--)
		{
			powerUpList[i]->Update(0, 0, Video::Width, Video::Height);
			if(powerUpList[i]->Sprite->Position.Y > Video::Height)
			{
				// powerup wasn't collected by the player and has disappeared
				// off the bottom of the screen
				powerUpList->RemoveAt(i);
				//LogManager::WriteLine(LogType::Debug, "Removing powerup from list. Items remaining: {0}", powerUpList->Count);
			}
			else if(powerUpList[i]->BoundingBox.IntersectsWith(player->BoundingBox) && !player->IsDead)
			{
				powerUpList[i]->PlayerCollision();
				powerUpList->RemoveAt(i);
				//LogManager::WriteLine(LogType::Debug, "ATTACHING LASERS, MOTHERFUCKER!!!");
			}
		}

		// update powerup effects
		for(int i = laserList->Count - 1; i >= 0; i--)
		{
			laserList[i]->Update();

			if(laserList[i]->BoundingBox.Bottom < 0)
			{
				laserList->RemoveAt(i);
			}
			else
			{
				HandleLaserBrickCollisions(laserList[i]);
			}
		}
	}

	/// <summary>
	/// Gets or sets the name of the test level, if any.
	/// </summary>
	/// <remarks>This is only relevant if /map was passed via command line.</remarks>
	property String ^TestLevel
	{
		String ^get() { return this->testLevel; }
		void set(String ^value) { this->testLevel = value; }
	}


	//---------------------------------------------------------------------------------
	// 
	// EVENT HANDLERS UP IN THIS MA FUCKER!
	//
	//---------------------------------------------------------------------------------
	/// <summary>
	/// controls whether the game over screen is displayed or if the player and ball
	/// are reset when the player loses a life.
	/// </summary>
	void OnPlayerResetTimerEvent(Object ^source, ElapsedEventArgs ^e)
	{
		this->playerResetTimer->Stop();

		lives->Value--;

		if(lives->Value == -1)
		{
			gameState = GameState::GameOver;
			this->gameOverScreen->Show(score->Value);
		}
		else
		{
			this->ResetPlayerAndBall();
			DisablePowerUps(true);
			this->gameState = GameState::Playing;
		}
	}

	/// <summary>
	/// describes what should happen when a brick is destroyed.
	/// </summary>
	void Brick_OnDeath(Object ^sender, BrickHitEventArgs ^e)
	{
		/*if(!ResourceManager::GetSoundBuffer("volume_conf")->IsPlaying)
		ResourceManager::GetSoundBuffer("volume_conf")->Play();*/

		Brick ^destroyedBrick = safe_cast<Brick ^>(sender);

		this->score->Value += destroyedBrick->PointValue;
		if(destroyedBrick->Name == "explosiveBrick")
		{
			// make the exploding brick look like it's exploding.
			//CreateExplosion(e->TileCoordinates.X, e->TileCoordinates.Y);
			ExplodeBrick(destroyedBrick, 255, 215, 0);
			ExplodeSurroundingBricks(e->TileCoordinates);
		}
		else if(BRICK_EXPLODE == (e->Flags & BRICK_EXPLODE)) // test to see if we've been told to explode.
		{
			ExplodeBrick(destroyedBrick, 255, 215, 0);
			//CreateExplosion(e->TileCoordinates.X, e->TileCoordinates.Y);
		}

		// decide whether a powerup should be spawned
		int value = randomNumberGen->Next(1, 101);
		if(value <= destroyedBrick->PowerUpSpawnPercentage)
		{
			SpawnPowerUp(e->ScreenCoordinates.X, e->ScreenCoordinates.Y);
		}

		this->currentLevel->BrickCount--;
		Debug::Assert(this->currentLevel->BrickCount >= 0);
	}

	void OnLevelTransitionTimerEvent(Object ^source, ElapsedEventArgs ^e)
	{
		this->levelLoadDelayTimer->Stop();
		this->gameState = GameState::NewLevel;
	}

	//------------------------------------------------------
	// POWERUP EVENT HANDLERS
	//------------------------------------------------------

	//// LASER ////////////////////////////////////////////////
	void OnLaserTimerEvent(Object ^source, ElapsedEventArgs ^e)
	{
		if(powerUpTimerValue->Value != 0)
		{
			powerUpTimerValue->Value--;
		}
		else
		{
			// no more time left on the clock for this powerup.
			// remove the power up effects and reset the timer.
			DisablePowerUps(false);
		}
	}

	// Event handlder for when the laser powerup collides with the players paddle.
	void OnCollisionWithPaddle_LaserPowerUp(Object ^sender, EventArgs ^e)
	{
		ResourceManager::GetSoundBuffer("powerup2")->Play();
		//powerUpInEffect = safe_cast<PowerUp ^>(sender);
		ammoCount->Value += safe_cast<LaserPowerUp ^>(sender)->PickupAmmo;

		// when the laser powerup is caught, the timer is reset is if the power-up
		// is already active.
		/*if(laserActiveTimer->Enabled)
		{
			powerUpTimerValue->Value = 30;
		}
		else
		{
			laserActiveTimer->Start();
		}*/
	}

	// Handles firing the lasers when the laser powerup is active and the
	// player has pressed by the fire button.
	void OnFirePressed_LaserPowerUp(Object ^sender, EventArgs ^args)
	{
		Laser ^leftLaser = EntityManager::GetEntity<Laser ^>("laser");
		Laser ^rightLaser = EntityManager::GetEntity<Laser ^>("laser");

		int spawnX = player->Sprite->Position.X + 8;
		int spawnY = (player->BoundingBox.Y - leftLaser->BoundingBox.Height) - 1;
		leftLaser->SetPosition(spawnX, spawnY);
		leftLaser->Velocity = this->laser->LaserVelocity;
		laserList->Add(leftLaser);


		spawnX = player->BoundingBox.Right - 8;
		rightLaser->SetPosition(spawnX, spawnY);
		rightLaser->Velocity = leftLaser->Velocity;
		laserList->Add(rightLaser);

		// you have to stop the sound effect otherwise playing does nothing
		// if it's already playing and you don't get that rapid fire effect.
		ResourceManager::GetSoundBuffer("laser")->Stop();
		ResourceManager::GetSoundBuffer("laser")->Play();
	}

	/// INSTADEATH ////////////////////////////////////////////
	void OnCollisionWithPaddle_InstaDeathPowerUp(System::Object ^sender, System::EventArgs ^args)
	{
		//player->IsDead = true;

		// generate the explosion in the middle of the paddle
		ExplodePaddle();
	}

	void OnCollisionWithPaddle_BonusPointsPowerUp(System::Object ^sender, System::EventArgs ^args)
	{
		BonusPointsPowerUp ^temp = safe_cast<BonusPointsPowerUp ^>(sender);
		score->Value += temp->Points;

		ResourceManager::GetSoundBuffer("powerup2")->Play();
	}

	void OnCollisionWithPaddle_ExtraLifePowerUp(System::Object ^sender, System::EventArgs ^args)
	{
		ExtraLifePowerUp ^temp = safe_cast<ExtraLifePowerUp ^>(sender);
		lives->Value += temp->LivesAwarded;

		ResourceManager::GetSoundBuffer("powerup2")->Play();
	}

};
