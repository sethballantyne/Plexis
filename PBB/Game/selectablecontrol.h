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
#include "control.h"
#include "keys.h"
#include "mouse.h"

using namespace System;

/// <summary>
/// parent class for all interactive controls and those which receive
/// input from their parent Scene.
/// </summary>
public ref class SelectableControl abstract : Control, IComparable
{
protected:
    // determines the controls position in the control list.
    int selectedIndex;

    // specifies whether the control is enabled or not. Controls that aren't enabled
    // cannot be selected.
    bool enabled;

    // specifies whether the control has the menu cursor adjacent to it.
    bool selected;

    bool showMenuCursor;
public:
    SelectableControl(int x, int y, int selectedIndex) : Control(x, y)
    {
        this->selectedIndex = selectedIndex;
        this->enabled = true;
        this->showMenuCursor = true;
    }

    /// <summary>
    /// compares the current instance of SelectableControl against another.
    /// </summary>
    /// <param name="obj"></param>
    /// <returns></returns>
    virtual int CompareTo(Object ^obj);
 
    /// <summary>
    /// abstract method for handling arguments passed to the control via its parent scene.
    /// </summary>
    /// <param name="sceneArgs">the arguments the control should interpret.</param>
    virtual void ReceiveSceneArgs(array<Object ^, 1> ^sceneArgs) = 0;

    /// <summary>
    /// abstract method for handling user input and updating the state of the control.
    /// </summary>
    /// <param name="keyboardState">current state of the keyboard.</param>
    /// <param name="mouseState">current state of the mouse.</param>
    virtual void Update(Keys ^keyboardState, Mouse ^mouseState) = 0;

    /// <summary>
    /// specifies whether the control has the menu cursor adjacent to it.
    /// </summary>
    property bool IsSelected
    {
        bool get()
        {
            return this->selected;
        }

        void set(bool value)
        {
            this->selected = value;
        }
    }

    /// <summary>
    /// Gets or sets the controls position in the control list.
    /// </summary>
    /// <remarks>
    /// Controls that descend from SelectableControl are sorted in ascending order
    /// by their selectedIndex value, which is used to determine which control
    /// should be selected next when the user presses the up or down keys.
    /// Technically, this value doesn't need to be unique to the control, but it's a good
    /// idea if you want to ensure the correct order.
    /// </remarks>
    property int SelectedIndex
    {
        int get()
        {
            return this->selectedIndex;
        }

        void set(int value)
        {
            this->selectedIndex = value;
        }
    }

    /// <summary>
    /// 
    /// </summary>
    property bool Enabled
    {
        bool get()
        {
            return this->enabled;
        }
        void set(bool value)
        {
            this->enabled = value;
        }
    }

    /// <summary>
    /// 
    /// </summary>
    property bool ShowMenuCursor
    {
        bool get()
        {
            return this->showMenuCursor;
        }

        void set(bool value)
        {
            this->showMenuCursor = value;
        }
    }
};