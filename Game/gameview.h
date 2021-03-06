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

/// <summary>
/// 
/// </summary>
public ref class GameView : SelectableControl
{
private:
	GameLogic ^gameLogic;
public:
    GameView(String ^gameInProgressMainMenu) : SelectableControl(0, 0, 0, gcnew System::Drawing::Size(0, 0))
    {
       IsSelected = true;
	   gameLogic = gcnew GameLogic(gameInProgressMainMenu);
    }

    void ReceiveSceneArgs(array<Object ^, 1> ^sceneArgs) override
    {
        if(sceneArgs != nullptr && sceneArgs[0] != nullptr)
        {
			if ("new" == safe_cast<String ^>(sceneArgs[0]))
			{
				gameLogic->NewGame();
			}
			else if ("/map" == safe_cast<String ^>(sceneArgs[0]))
			{
				Debug::Assert(sceneArgs->Length >= 2);
				gameLogic->TestLevel = safe_cast<String ^>(sceneArgs[1]);
			}

			gameLogic->UpdateKeys();
        }
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