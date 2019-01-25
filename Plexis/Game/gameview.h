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
#include "selectablecontrol.h"
#include "scenemanager.h"
#include "entitymanager.h"
#include "levelmanager.h"
#include "gamelogic.h"

public ref class GameView : SelectableControl
{
private:
    Label ^text;
    //String ^gameInProgressMainMenuScene;
    //String ^highScorePrompt;
    //Level ^currentLevel = nullptr;
	GameLogic ^gameLogic;
public:
    GameView(String ^gameInProgressMainMenu, String ^highScorePrompt) : SelectableControl(0, 0, 0, gcnew System::Drawing::Size(0, 0))
    {
       text = gcnew Label(0, 0, "green", "game view");
       // gameInProgressMainMenuScene = gameInProgressMainMenu;
       IsSelected = true;
	   gameLogic = gcnew GameLogic(gameInProgressMainMenu, highScorePrompt);
    }

    void ReceiveSceneArgs(array<Object ^, 1> ^sceneArgs) override
    {
        if(sceneArgs != nullptr && sceneArgs[0] != nullptr)
        {
			if("new" == (String ^)sceneArgs[0])
			{
				gameLogic->NewGame();
			}

			gameLogic->UpdateKeys();
            //text->Text = (String ^) sceneArgs[0];
        }

        //currentLevel = LevelManager::GetNextLevel();
    }

    void Update(Keys ^keyboardState, Mouse ^mouseState) override
    {
		gameLogic->Update(keyboardState, mouseState);
        /*if(keyboardState->KeyPressed(DIK_ESCAPE))
        {
            SceneManager::SetActiveScene(gameInProgressMainMenuScene, nullptr);
        }*/
		
    }

    void Render() override
    {
		gameLogic->Render();
        //currentLevel->Render();
        //text->Render();
    }
};