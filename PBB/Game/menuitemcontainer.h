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

#include "selectablecontrol.h"
#include "resourcemanager.h"
#include "surface.h"
#include "video.h"
#include "logmanager.h"

using namespace System::Collections::Generic;


/// <summary>
/// MenuItemContainers are in charge of selecting the appropriate MenuItem 
/// each time the user presses the down or up arrow keys. 
/// They also forward the input state to the currently selected control 
/// and handle the rendering of each control.
/// </summary>
public ref class MenuItemContainer : public SelectableControl
{
private:
    // controls contained within the container. Each control depends on the container 
    // to instruct it to render and pass on arguments received from the Scene they 
    // belong to each time the scene is displayed. 
    List<SelectableControl ^> ^items;

    // the cursor to be displayed next to a child control, indicating it's the control that is selected.
    Surface ^menuItemCursor;

    // the arguments passed by the Scene object each time the scene is shown to the user.
    array<Object ^, 1> ^receivedArguments;

    // the index of the currently selected item in the items list. 
    // This is used to determine which control is to be selected next,
    // based on whether the user presses up (select previous control) or down (select the next control).
    int currentSelectedItem;
public:
    /// <summary>
    /// 
    /// </summary>
    MenuItemContainer(Surface ^cursor) : SelectableControl(0, 0, 0, gcnew System::Drawing::Size())
    {
        this->items = gcnew List<SelectableControl ^>();
        this->IsSelected = true;
        this->menuItemCursor = cursor;
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="control"></param>
    void AddControl(SelectableControl ^control);


    /// <summary>
    /// Stores and forwards the arguments passed by the Scene to each control located within the container.
    /// </summary>
    /// <param name="args"></param>
    void ReceiveSceneArgs(array<Object ^, 1> ^args) override
    {
        this->receivedArguments = args;

        for(int i = 0; i < this->items->Count; i++)
        {
            items[i]->ReceiveSceneArgs(args);
        }
    }

    /// <summary>
    /// 
    /// </summary>
    void SelectPreviousControl();

    /// <summary>
    /// 
    /// </summary>
    void SelectNextControl();

    void Render() override
    {
        try
        {
            for(int i = 0; i < this->items->Count; i++)
            {
                if(this->items[i]->IsSelected && items[i]->ShowMenuCursor)
                {
                    Video::Blit(items[i]->CursorPosition->X, items[i]->CursorPosition->Y, this->menuItemCursor);
                }

                this->items[i]->Render();
            }
        }
        catch(...)
        {
            throw;
        }
    }
    /// <summary>
    /// 
    /// </summary>
    /// <param name="keyboardState"></param>
    /// <param name="mouseState"></param>
    void Update(Keys ^keyboardState, Mouse ^mouseState) override
    {
        if(nullptr == keyboardState)
        {
            throw gcnew ArgumentNullException("keyboardState");
        }

        this->items[this->currentSelectedItem]->Update(keyboardState, mouseState);
    }

    property System::Drawing::Size ^CursorSize
    {
        System::Drawing::Size ^get()
        {
            return gcnew System::Drawing::Size(menuItemCursor->Width, menuItemCursor->Height);
        }
    }
};
