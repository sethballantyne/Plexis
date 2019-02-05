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

using namespace System::Diagnostics;
using namespace System::Timers;

// 0 based
#define DEFAULT_NUMBER_OF_LIVES 2

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

	// the amount of points the player has
	NumericField ^score;

	// the number lives remaining
	NumericField ^lives;

	// 
	Timer ^levelLoadDelayTimer = gcnew Timer(5000);

	

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

	
	/// <summary>
	/// Removes a brick from the game, as if it were hit by the ball.
	/// </summary>
	void DebugRemoveBrick();

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
		if(mouseState->ButtonDown(playerFireKey) || keyboardState->KeyDown(playerFireKey))
		{
			player->FirePressed();
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
			this->gameState = GameState::Playing;
		}
	}


	void OnLevelTransitionTimerEvent(Object ^source, ElapsedEventArgs ^e)
	{
		this->levelLoadDelayTimer->Stop();
		this->gameState = GameState::NewLevel;
	}

	/// <summary>
	/// describes what should happen when a brick is destroyed.
	/// </summary>
	void OnDeath(Object ^sender, EventArgs ^e)
	{
		/*if(!ResourceManager::GetSoundBuffer("volume_conf")->IsPlaying)
			ResourceManager::GetSoundBuffer("volume_conf")->Play();*/

		this->score->Value += (safe_cast<Brick ^>(sender))->PointValue;
		this->currentLevel->BrickCount--;
		Debug::Assert(this->currentLevel->BrickCount >= 0);
	}
};