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
#include "label.h"
#include "input.h"
#include "highscores.h"
#include "editablelabel.h"
#include "scenemanager.h"

using namespace System::Timers;

/// <summary>
/// 
/// </summary>
public ref class GameOverScreen
{
private:
	//
	Label ^gameOverLabel = gcnew Label(431, 250, "white", "");

	// the x coordinate will change, based on the string length passed
	// to the label when the timer fires.
	Label ^finalScoreLabel = gcnew Label(431, 300, "white", " ");

	//
	Label ^newHighScoreYoApostrophe;

	// x value changes in the constructor
	EditableLabel ^namePrompt = gcnew EditableLabel(431, 400, "green", 12, true);

	// x value changes in the constructor
	Label ^pressAnyKeyToContinuePrompt = gcnew Label(0, 500, "white", "PRESS ANY KEY TO CONTINUE");

	
	//
	Timer ^gameOverPrinterTimer = gcnew Timer(190);

	Surface ^dude;

	int dudex, dudey;
	//
	unsigned int finalScore;

	int scoreRank = -1;
	//
	bool visible = false;

	bool showPressAnyKeyPrompt = false;
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
			tickCount = 0;

			finalScoreLabel->Text = String::Format("FINAL SCORE: {0}", finalScore);

			// center the label on the screen
			finalScoreLabel->Position->X = (Video::Width / 2)  - ((finalScoreLabel->Text->Length * finalScoreLabel->LabelFont->GlyphWidth) /  2);

			scoreRank = HighScores::IsAHighScore(finalScore);
		}
	}
public:
	GameOverScreen()
	{
		gameOverPrinterTimer->Elapsed += gcnew ElapsedEventHandler(this, &GameOverScreen::OnPrintGameOverTimerEvent);
		gameOverPrinterTimer->Enabled = false;
		gameOverPrinterTimer->AutoReset = true;

		String ^newHighScoreText = "BITCHIN'! NEW HIGHSCORE! ENTER YOUR NAME, ROCKIN' DUDE!";
		int videoWidthDiv2 = Video::Width / 2;

		// safe to use the finalScoreFont because it's exactly the same font as this label uses.
		int xposition = videoWidthDiv2 - ((newHighScoreText->Length * finalScoreLabel->LabelFont->GlyphWidth) / 2);
		newHighScoreYoApostrophe = gcnew Label(xposition, 350, "green", newHighScoreText);
		namePrompt->Position->X = videoWidthDiv2 - ((namePrompt->Length * namePrompt->LabelFont->GlyphWidth) / 2);
		pressAnyKeyToContinuePrompt->Position->X = videoWidthDiv2 - ((pressAnyKeyToContinuePrompt->Text->Length * pressAnyKeyToContinuePrompt->LabelFont->GlyphWidth) / 2);

		dude = ResourceManager::GetSurface("dude");
		dudex = (Video::Width / 2) - (dude->Size->Width / 2);
		dudey = 450;
	}

	/// <summary>
	/// 
	/// </summary>
	/// <param name="keyboardInput"></param>
	/// <param name="mouseInput"></param>
	void Update(Keys ^keyboardInput, Mouse ^mouseInput)
	{
		if(showPressAnyKeyPrompt && keyboardInput->PressedKey != -1)
		{
			array<String ^> ^params = gcnew array<String ^>(1);
			params[0] = "updateScores";

			Hide();
			SceneManager::SetActiveScene("game_over_view_high_scores", params);
		}
		else if(scoreRank != -1)
		{
			namePrompt->Update(keyboardInput, mouseInput);

			// pressing return will have "submitted" the players name in namePrompt->Update(),
			// deactivating the control. Display the "press any key" prompt so we can get out of here.
			if(keyboardInput->KeyPressed(DIK_RETURN))
			{
				showPressAnyKeyPrompt = true;
				HighScores::Update(scoreRank, namePrompt->Bytes, finalScore);
			}
		}
	}

	/// <summary>
	/// 
	/// </summary>
	void Show(unsigned int score)
	{
		gameOverPrinterTimer->Start();
		visible = true;
		
		finalScore = score;

	}

	void Hide()
	{
		visible = false;
		scoreRank = -1;
		showPressAnyKeyPrompt = false;
		gameOverLabel->Text = "";
		finalScoreLabel->Text = "";
		namePrompt->Clear();
	}
	/// <summary>
	/// 
	/// </summary>
	void Render()
	{
		gameOverLabel->Render();
		finalScoreLabel->Render();

		if(scoreRank != -1)
		{
			newHighScoreYoApostrophe->Render();
			namePrompt->Render();
			
			if(showPressAnyKeyPrompt)
			{
				pressAnyKeyToContinuePrompt->Render();
			}
			//Video::Blit(dudex, dudey, dude);
		}
	}

	/// <summary>
	/// 
	/// </summary>
	property bool Visible
	{
		bool get() {return this->visible;}
		void set(bool value) { this->visible = value; }
	}
};