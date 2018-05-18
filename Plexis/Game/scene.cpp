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
#include "scene.h"
#include "logmanager.h"

void Scene::Update(Keys ^keyboardState, Mouse ^mouseState)
{
    // mouse state isn't checked because it's not required
    // to play the game and navigate the menus.
    if(nullptr == keyboardState)
    {
        throw gcnew ArgumentNullException("keyboardState");
    }

    try
    {
        for(int i = 0; i < controls->Count; i++)
        {
            if(controls[i]->GetType()->IsSubclassOf(SelectableControl::typeid))
            {
                SelectableControl ^temp = dynamic_cast<SelectableControl ^>(controls[i]);
                if(temp->IsSelected)
                {
                    temp->Update(keyboardState, mouseState);
                }
            }
        }
    }
    catch(...)
    {
        throw;
    }
}