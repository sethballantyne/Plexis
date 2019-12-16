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
#include "laserpowerup.h"
#include "laser.h"

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


	Surface ^timerImage = nullptr;

	// the amount of points the player has
	NumericField ^score;

	// the number lives remaining
	NumericField ^lives;

	// number of seconds remaining when the laser power up is in use.
	NumericField ^powerUpTimerValue;

	// 
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

	PowerUp ^powerUpInEffect = nullptr;

	// the name of the level if /map was passed via command line.
	String ^testLevel = nullptr;

	Random ^randomNumberGen = gcnew Random();
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
		
		if(mouseState->ButtonPressed(playerFireKey))
		{
			if(nullptr != powerUpInEffect)
			{
				/*ResourceManager::GetSoundBuffer("laser")->Stop();
				ResourceManager::GetSoundBuffer("laser")->Play();*/
				powerUpInEffect->Fired();
				
			}
			else
			{
				player->FirePressed();
			}
		}
		/*if(mouseState->ButtonDown(playerFireKey) || keyboardState->KeyDown(playerFireKey))
		{
			if(nullptr != powerUpInEffect)
			{
				powerUpInEffect->Fired();
			}
			else
			{
				player->FirePressed();
			}
		}*/
		/*if((keyboardState->KeyPressed(playerFireKey) || mouseState->ButtonPressed(playerFire
		{
		}*/
	}

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
						currentLevel[i, j]->Hit(i, j);
						laserList->Remove(laser);
					}
				}
			}
		}
	}

public:
	GameLogic(String ^gameInProgressMenu);

	void Update(Keys ^keyboardState, Mouse ^mouseState);

	/// <summary>
	/// Gets the key configuration from GameOptions.
	/// </summary>
	void UpdateKeys()
	{
		this->playerFireKey = GameOptions::GetValue("fireKey", DIK_SPACE);
		this->pauseKey = GameOptions::GetValue("", DIK_P);
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

	/// <summary>
	/// controls whether the game over screen is displayed or if the player and ball
	/// are reset when the player loses a life.
	/// </summary>
	void OnPlayerResetTimerEvent(Object ^source, ElapsedEventArgs ^e)
	{
		this->playerResetTimer->Stop();

		if(gameState == GameState::GameOver)
		{
			this->gameOverScreen->Show(score->Value);
		}
		else
		{
			this->ResetPlayerAndBall();
		    DisablePowerUps(true);
			this->gameState = GameState::Playing;
		}
	}


	void OnLevelTransitionTimerEvent(Object ^source, ElapsedEventArgs ^e)
	{
		this->levelLoadDelayTimer->Stop();
		this->gameState = GameState::NewLevel;
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

	// Handles firing the lasers when the laser powerup is active and the
	// player has pressed by the fire button.
	void OnFirePressed_LaserPowerUp(Object ^sender, EventArgs ^args)
	{
		Laser ^leftLaser = EntityManager::GetEntity<Laser ^>("laser");
		Laser ^rightLaser = EntityManager::GetEntity<Laser ^>("laser");

		int spawnX = player->Sprite->Position.X + 8;
		int spawnY = (player->BoundingBox.Y - leftLaser->BoundingBox.Height) - 1;
		leftLaser->SetPosition(spawnX, spawnY);
		leftLaser->Velocity = safe_cast<LaserPowerUp ^>(powerUpInEffect)->LaserVelocity;
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

	// Event handlder for when the laser powerup collides with the players paddle.
	void OnCollisionWithPaddle_LaserPowerUp(Object ^sender, EventArgs ^e)
	{
		ResourceManager::GetSoundBuffer("powerup2")->Play();
		powerUpInEffect = safe_cast<PowerUp ^>(sender);

		// when the laser powerup is caught, the timer is reset is if the power-up
		// is already active.
		if(laserActiveTimer->Enabled)
		{
			powerUpTimerValue->Value = 30;
		}
		else
		{
			laserActiveTimer->Start();
		}
	}

	/// <summary>
	/// describes what should happen when a brick is destroyed.
	/// </summary>
	void OnDeath(Object ^sender, BrickHitEventArgs ^e)
	{
		/*if(!ResourceManager::GetSoundBuffer("volume_conf")->IsPlaying)
			ResourceManager::GetSoundBuffer("volume_conf")->Play();*/

		Brick ^destroyedBrick = safe_cast<Brick ^>(sender);

		this->score->Value += destroyedBrick->PointValue;
		if(destroyedBrick->Name == "explosiveBrick")
		{
			// make the exploding brick look like it's exploding.
			CreateExplosion(e->TileCoordinates.X, e->TileCoordinates.Y);

			ExplodeSurroundingBricks(e->TileCoordinates);
		}
		else if(true == e->Explode) // test to see if we've been told to explode.
		{
			CreateExplosion(e->TileCoordinates.X, e->TileCoordinates.Y);
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
					currentLevel[xCoord, yCoord]->Die(xCoord, yCoord, true);
				}
			}
		}
	}

	/// <summary>
	/// Creates a random powerup at the specified coordinates.
	///</summary>
	void SpawnPowerUp(int x, int y)
	{
		static int id = 0;
		int value = randomNumberGen->Next(0, EntityManager::NumberOfPowerUps);
		float angle = randomNumberGen->Next(220, 340) * PI_RADIANS;

		switch(value)
		{
			case 0:
				LaserPowerUp ^laser = EntityManager::GetEntity<LaserPowerUp ^>("laser_powerup");
				
				laser->FirePressed += gcnew PowerUpEffectHandler(this, &GameLogic::OnFirePressed_LaserPowerUp);
				laser->CollisionWithPaddle += gcnew PowerUpEffectHandler(this, &GameLogic::OnCollisionWithPaddle_LaserPowerUp);
				laser->Spawn(x, y, angle);

				powerUpList->Add(safe_cast<PowerUp ^>(laser));
			break;
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
			else if(powerUpList[i]->BoundingBox.IntersectsWith(player->BoundingBox))
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
};