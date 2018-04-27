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
#include "menuitem.h"
#include "scenemanager.h"
#include "game.h"

MenuItem::MenuItem(int x, int y, String ^font, String ^text, int selectedIndex, String ^navigateTo, 
    MenuItemContainer ^parentContainer) : ContainerControl(x, y, selectedIndex, parentContainer)
{
    if(nullptr == font)
    {
        throw gcnew ArgumentNullException("font");
    }
    else if(nullptr == text)
    {
        throw gcnew ArgumentNullException("text");
    }
    else if(nullptr == navigateTo)
    {
        throw gcnew ArgumentNullException("navigateTo");
    }

    if(String::Empty == font)
    {
        throw gcnew ArgumentException("font evaluates to String::Empty.");
    }
    else if(String::Empty == text)
    {
        throw gcnew ArgumentException("text evaluates to String::Empty.");
    }
    else if(String::Empty == navigateTo)
    {
        throw gcnew ArgumentException("navigateTo evaluates to String::Empty.");
    }

    try
    {
        this->label = gcnew Label(x, y, font, text);
    }
    catch(...)
    {
        throw;
    }

    this->navigateTo = navigateTo;
}

void MenuItem::Update(Keys ^keyboardState, Mouse ^mouseState)
{
    if(keyboardState->KeyPressed(DIK_UP))
    {
        ParentContainer->SelectPreviousControl();
    }
    else if(keyboardState->KeyPressed(DIK_DOWN))
    {
        ParentContainer->SelectNextControl();
    }
    else if(keyboardState->KeyPressed(DIK_RETURN))
    {
        if(nullptr != navigateTo)
        {
            if("terminate" == navigateTo)
            {
                Game::IsRunning = false;
                return;
            }

            // "caller" means that when selected, navigate back to the previous scene.
            else if("caller" == navigateTo)
            {
                if(nullptr != receivedArgs)
                {
                    navigateTo = (String ^) receivedArgs[0];
                }
            }

            array<Object ^, 1> ^args = gcnew array<Object ^, 1> { (Object ^)outboundArgs };

            try
            {
                SceneManager::SetActiveScene(navigateTo, args);
            }
            catch(...)
            {
                throw;
            }
        }

    }
}
