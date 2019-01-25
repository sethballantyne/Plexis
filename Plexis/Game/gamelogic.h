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

using namespace System::Diagnostics;
using namespace System::Timers;

#define DEFAULT_NUMBER_OF_LIVES 3

/// <summary>
/// 
/// </summary>
public ref class GameLogic
{
private:
	//
	Level ^currentLevel;

	//
	Paddle ^player;

	//
	Ball ^ball;

	//
	Surface ^pauseImage = nullptr;

	//
	Surface ^levelCompleteImage = nullptr;

	//
	Surface ^livesPaddleImage = nullptr;

	Label ^gameOverLabel;

	//
	NumericField ^score;

	//
	NumericField ^lives;

	// 
	Timer ^levelLoadDelayTimer = gcnew Timer(5000);

	//
	Timer ^gameOverPrinterTimer = gcnew Timer(190);

	// used for the delay when the ball goes off the screen
	Timer ^playerResetTimer = gcnew Timer(2000);

	//
	String ^gameInProgressMainMenu;
	String ^highScorePrompt;

	//
	GameState gameState = GameState::NewLevel;

	//
	int pauseKey;
	int playerLeftKey;
	int playerRightKey;
	int playerFireKey;

	int pauseX;
	int pauseY;

	int levelCompleteX;
	int levelCompleteY;

	int numberOfBricksRemaining;
	int numberOfLives;

	bool debugKeysEnabled = false;

	bool gameOverScreenVisible = false;

	//-----------------------------------------------------------
	// DEBUG COMMANDS
	//-----------------------------------------------------------
	void DebugRemoveBrick();

	void GameOverTransition()
	{
		this->gameOverLabel->Text = String::Empty;
		//this->gameState = GameState::GameOver;
	    gameOverScreenVisible = true;
		this->gameOverPrinterTimer->Start();
	}

	void ResetPlayerAndBall()
	{
		player->ResetPosition();
		this->player->RemoveAttachments();
		this->player->AttachBall(ball);
	}

	/// <summary>
	/// 
	/// </summary>
	/// <param name="keyboardState"></param>
	/// <param name="mouseState"></param>
	void HandleGameStateInput(Keys ^keyboardState, Mouse ^mouseState);
	
	/// <summary>
	/// 
	/// </summary>
	void HandleBallCollision();

	/// <summary>
	/// 
	/// </summary>
	void HandlePlayerWallCollision();
	
	/// <summary>
	/// 
	/// </summary>
	void HandleBrickCollisions();

	/// <summary>
	/// 
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
	/// 
	/// </summary>
	/// <param name="keyboardState"></param>
	/// <param name="mouseState"></param>
	void HandleGameInput(Keys ^keyboardState, Mouse ^mouseState)
	{
		player->Velocity.X = mouseState->X;
		
		if(keyboardState->KeyDown(playerFireKey))
		{
			player->FirePressed();
		}
	}

public:
	/// <summary>
	/// 
	/// </summary>
	/// <param name="gameInProgressMenu"></param>
	/// <param name="highScorePrompt"></param>
	GameLogic(String ^gameInProgressMenu, String ^highScorePrompt);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="keyboardState"></param>
	/// <param name="mouseState"></param>
	void Update(Keys ^keyboardState, Mouse ^mouseState);

	/// <summary>
	/// 
	/// </summary>
	void UpdateKeys()
	{
		this->playerLeftKey = GameOptions::GetValue("movePaddleLeftKey", DIK_LEFT);
		this->playerRightKey = GameOptions::GetValue("movePaddleRightKey", DIK_RIGHT);
		this->playerFireKey = GameOptions::GetValue("fireKey", DIK_SPACE);
		this->pauseKey = GameOptions::GetValue("", DIK_P);
	}

	/// <summary>
	/// 
	/// </summary>
	void Render();
	/// <summary>
	/// 
	/// </summary>
	void NewGame()
	{
		this->gameState = GameState::NewLevel;
		this->numberOfLives = DEFAULT_NUMBER_OF_LIVES;
		this->score->Value = 0;
		this->lives->Value = this->numberOfLives;
		gameOverScreenVisible = false;

		LevelManager::ResetLevelCounter();
	}

	void OnPlayerResetTimerEvent(Object ^source, ElapsedEventArgs ^e)
	{
		this->playerResetTimer->Stop();

		if(gameState == GameState::GameOver)
		{
			this->GameOverTransition();
		}
		else
		{
			this->ResetPlayerAndBall();
			this->gameState = GameState::Playing;
		}
	}

	/// <summary>
	/// 
	/// </summary>
	/// <param name="source"></param>
	/// <param name="e"></param>
	void OnPrintGameOverTimerEvent(Object ^source, ElapsedEventArgs ^e)
	{
		static wchar_t text[] = {'G', 'A', 'M', 'E', ' ', 'O', 'V', 'E', 'R'};
		static int tickCount = 0;

		gameOverLabel->Text += text[tickCount];
		tickCount++;

		if(wcslen(text) == tickCount)
		{
			this->gameOverPrinterTimer->Stop();
			tickCount =  0;
		}
	}

	/// <summary>
	/// 
	/// </summary>
	/// <param name="source"></param>
	/// <param name="e"></param>
	 void OnLevelTransitionTimerEvent(Object ^source, ElapsedEventArgs ^e)
	{
		this->levelLoadDelayTimer->Stop();
		this->gameState = GameState::NewLevel;
	}

	/// <summary>
	/// 
	/// </summary>
	/// <param name="sender"></param>
	/// <param name="e"></param>
	void OnDeath(Object ^sender, EventArgs ^e)
	{
		/*if(!ResourceManager::GetSoundBuffer("volume_conf")->IsPlaying)
			ResourceManager::GetSoundBuffer("volume_conf")->Play();*/

		this->score->Value += (safe_cast<Brick ^>(sender))->PointValue;
		this->currentLevel->BrickCount--;
		LogManager::WriteLine(LogType::Debug, "{0}", currentLevel->BrickCount);
		Debug::Assert(this->currentLevel->BrickCount >= 0);
	}
};