#include "gamelogic.h"
#include "scenemanager.h"

void GameLogic::DebugRemoveBrick()
{
	for(int i = 0; i < currentLevel->Width; i++)
	{
		for(int j = 0; j < currentLevel->Height; j++)
		{
			if(nullptr != currentLevel[i, j] && currentLevel[i, j]->Visible)
			{
				currentLevel[i,j]->Hit(i, j);
				return;
			}
		}
	}
}


GameLogic::GameLogic(String ^gameInProgressMenu)
{
	this->gameInProgressMainMenu = gameInProgressMenu;
	//this->highScorePrompt = highScorePrompt;
	this->debugKeysEnabled = GameOptions::GetValue("debugKeys", false);

	this->player = EntityManager::GetEntity<Paddle ^>("player");
	Debug::Assert(this->player != nullptr);
	this->player->SetPosition(player->Sprite->Position.X, 700);

	this->ball = EntityManager::GetEntity<Ball ^>("defaultBall");
	Debug::Assert(this->ball != nullptr);

	this->livesImage = ResourceManager::GetSurface("heart2");
	this->lives = gcnew NumericField(200, 5, this->livesImage, 2, 2);
	this->score = gcnew NumericField(5, 5, "SCORE", 0, 5);
	this->pauseImage = ResourceManager::GetSurface("paused2");
	this->pauseX = (Video::Width / 2) - (this->pauseImage->Size->Width / 2);
	this->pauseY = 564;

	this->levelCompleteImage = ResourceManager::GetSurface("level_complete");
	this->levelCompleteX = (Video::Width / 2) - (this->levelCompleteImage->Size->Width / 2);
	this->levelCompleteY = (Video::Height / 2) - (this->levelCompleteImage->Size->Height / 2);

	this->levelLoadDelayTimer->Elapsed += gcnew ElapsedEventHandler(this, &GameLogic::OnLevelTransitionTimerEvent);
	this->levelLoadDelayTimer->Enabled = false;
	this->levelLoadDelayTimer->AutoReset = true;

	this->playerResetTimer->Elapsed += gcnew ElapsedEventHandler(this, &GameLogic::OnPlayerResetTimerEvent);
	this->playerResetTimer->Enabled = false;
	this->playerResetTimer->AutoReset = true;
	
}

void GameLogic::HandleGameStateInput(Keys ^keyboardState, Mouse ^mouseState)
{
	Debug::Assert(keyboardState != nullptr && mouseState != nullptr);

	if(gameState == GameState::Playing || gameState == GameState::Paused)
	{
		if(this->debugKeysEnabled)
		{
			if(keyboardState->KeyPressed(DIK_SUBTRACT))
			{
				this->DebugRemoveBrick();
			}

			else if(keyboardState->KeyPressed(DIK_D))
			{
				//this->GameOverTransition();
				gameState = GameState::GameOver;
				this->gameOverScreen->Show(score->Value);
			}
			else if(keyboardState->KeyPressed(DIKEYBOARD_1))
			{
				score->Value += 500;
			}
		}

		if(keyboardState->KeyPressed(DIK_ESCAPE))
		{
			this->gameState = GameState::Paused;
			SceneManager::SetActiveScene(gameInProgressMainMenu, nullptr);
		}
		else if(keyboardState->KeyPressed(pauseKey))
		{
			if(this->gameState != GameState::Paused)
			{
				this->gameState = GameState::Paused;
			}
			else
			{
				// BUG: pressing pause in a state other than GameState::Playing
				// will put the game into this state when pause is deactivated.
				this->gameState = GameState::Playing;
			}
		}
	}
	
}
void GameLogic::HandleBallCollision()
{
	int ballX = ball->BoundingBox.X + ball->Velocity.X;
	int ballY = ball->BoundingBox.Y + ball->Velocity.Y;
	int ballX2 = ballX + ball->BoundingBox.Width;
	int ballY2 = ballY + ball->BoundingBox.Height;

	int correctedX = ballX;
	int correctedY = ballY;
	if(ballX2 >= Video::Width)
	{
		//ResourceManager::GetSoundBuffer("bounce")->Play();

		ball->Velocity.X = -ball->Velocity.X;
		correctedX = (Video::Width - ball->BoundingBox.Width) - 1;
	}

	if(ballX < 0)
	{
		//ResourceManager::GetSoundBuffer("bounce")->Play();

		ball->Velocity.X = -ball->Velocity.X;
		correctedX = 0;
	}

	

	if(ballY < 0)
	{
		ResourceManager::GetSoundBuffer("bounce")->Play();

		//ball->Velocity.X = -ball->Velocity.X;
		ball->Velocity.Y = -ball->Velocity.Y;
		correctedY = 0;
	}

	if(ballY >= Video::Height)
	{
		/*ball->Velocity.Y = -ball->Velocity.Y;
		correctedY = (Video::Height - ball->BoundingBox.Height) - 1;*/

		this->gameState = GameState::PlayerReset;
		ResourceManager::GetSoundBuffer("loselife")->Play();

		if(lives->Value != 0)
		{
			lives->Value--;
			ball->Velocity = Vector2::Zero;
		}
		else
		{
			this->gameState = GameState::GameOver;
		}

		playerResetTimer->Start();
		/*lives->Value--;
		if(lives->Value != -1)
		{
			ball->Velocity = Vector2::Zero;
			this->gameState = GameState::PlayerReset;
			playerResetTimer->Start();
		}
		else
		{
			this->GameOverTransition();
			return;
		}*/
	}

	if(ball->BoundingBox.IntersectsWith(player->BoundingBox))
	{
		/*if(!ResourceManager::GetSoundBuffer("volume_conf")->IsPlaying)
		ResourceManager::GetSoundBuffer("volume_conf")->Play();*/

		ResourceManager::GetSoundBuffer("bounce")->Play();

		ball->Velocity.Y *= -1;
		ball->Velocity.X = ball->Velocity.X + (0.4f * player->Velocity.X);

		if(ball->Velocity.X > 10)
		{
			ball->Velocity.X = 10;
		}

		// bottom of the ball has intersected with the paddle
		if(ball->BoundingBox.Y <= player->BoundingBox.Y)
		{
			correctedY = player->BoundingBox.Y - ball->BoundingBox.Height - 1;
		}
		// top of the ball has intersected with the paddle
		else if(ball->BoundingBox.Y > player->BoundingBox.Y)
		{
			correctedY = player->BoundingBox.Bottom + 1;
		}

		// left of the ball has intersected with the paddle
		if(ball->BoundingBox.Right >= player->BoundingBox.Right)
		{
			correctedX = player->BoundingBox.Right + 1;
		}

		// right side of the ball has intersected with the paddle
		else if(ball->BoundingBox.X <= player->BoundingBox.X)
		{
			correctedX = player->BoundingBox.X - ball->BoundingBox.Width - 1;
		}
	}

	ball->SetPosition(correctedX, correctedY);
}

void GameLogic::HandleBrickCollisions()
{
	for(int i = 0; i < currentLevel->Width; i++)
	{
		for(int j = 0; j < currentLevel->Height; j++)
		{
			if(nullptr != currentLevel[i, j] && currentLevel[i, j]->Visible)
			{
				if(ball->BoundingBox.IntersectsWith(currentLevel[i, j]->BoundingBox))
				{
					currentLevel[i, j]->Hit(i, j);
					ball->Velocity.X = -ball->Velocity.X;
					ball->Velocity.Y *= -1;

					return;
				}
			}
		}
	}
}

void GameLogic::HandlePlayerWallCollision()
{
	// where the left side of the paddle is going to be
	int x1 = player->BoundingBox.X + player->Velocity.X;

	// where the right side of the paddle is going to be
	int x2 = x1 + player->BoundingBox.Width;

	int correctedX = x1;

	if(x1 < 0)
	{
		correctedX = 0;
	}
	else if(x2 >= Video::Width)
	{
		correctedX = (Video::Width - player->Sprite->CurrentFrame->Coordinates.Width) - 1;
	}

	player->SetPosition(correctedX);
}
void GameLogic::Update(Keys ^keyboardState, Mouse ^mouseState)
{
	switch(this->gameState)
	{
		case GameState::NewLevel:
			LogManager::WriteLine(LogType::Debug, "GameState::NewLevel");
			if (!testLevel)
			{
				this->currentLevel = LevelManager::GetNextLevel();
			}
			else
			{
				this->currentLevel = LevelManager::ReadLevel(testLevel, true);
				this->testLevel = nullptr;
			}

			for(int i = 0; i < currentLevel->Width; i++)
			{
				for(int j = 0; j < currentLevel->Height; j++)
				{
					if(nullptr != this->currentLevel[i, j])
					{
						this->currentLevel[i, j]->Death += gcnew BrickDeathEventHandler(this, &GameLogic::OnDeath);
					}
				}
			}

			player->ResetPosition();
			this->player->RemoveAttachments();
			this->player->AttachBall(ball);

			this->gameState = GameState::Playing;
		break;

		case GameState::Playing:
			HandleGameInput(keyboardState, mouseState);
			HandleCollisions();

			if(0 == currentLevel->BrickCount)
			{
				gameState = GameState::LevelComplete;
				this->levelLoadDelayTimer->Start();
			}
		case GameState::GameOver:
			gameOverScreen->Update(keyboardState, mouseState);
			break;
		break;
		default:
			break;
	}

	HandleGameStateInput(keyboardState, mouseState);
}

void GameLogic::Render()
{
	if(nullptr != currentLevel)
	{
		// stops the current level from being rendered before rendering the first level
		// in the game if the player selects "new game" from the main menu.
		// The current level will briefly appear before reverting to the first level, otherwise.
		if(GameState::NewLevel != gameState && !gameOverScreen->Visible)
		{
			currentLevel->Render();
		}

		if(GameState::GameOver == gameState && gameOverScreen->Visible)
		{
			// blank screen with "GAME OVER" rendered in the middle.
			gameOverScreen->Render();
		}
		else
		{
			player->Sprite->Render();
			ball->Sprite->Render();

			if(explosionList->Count > 0)
			{
				LogManager::WriteLine(LogType::Debug, "{0}", explosionList->Count);
				for(int i = explosionList->Count - 1; i >= 0; i--)
				{
					explosionList[i]->Render();
					if(explosionList[i]->Done)
					{
						explosionList->RemoveAt(i);
					}
				}
			}

			score->Render();

			// don't show -1 when the player loses his/her last life.
			lives->Render();

			switch(this->gameState)
			{
				case GameState::Paused:
					Video::Blit(this->pauseX, this->pauseY, this->pauseImage);
					break;

				case GameState::LevelComplete:
					Video::Blit(this->levelCompleteX, this->levelCompleteY, this->levelCompleteImage);
					break;
				default:
					break;
			}
		}
	}
}