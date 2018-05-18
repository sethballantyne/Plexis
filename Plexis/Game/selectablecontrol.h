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

    // if true, the menu cursor is shown when the item is selected; otherwise it's hidden.
    bool showMenuCursor;

    static int sizeOfGapBetweenCursorAndMenuItems = 12;

    // where the cursor should be rendered on the screen when this control is selected.
    System::Drawing::Point ^cursorPosition;

    // cursor dimensions required for calculating the position above.
    System::Drawing::Size ^cursorSize;

public:
    /// <summary>
    /// Initialises SelectableControl.
    /// </summary>
    /// <param name="x">the controls screen coordinate on the x axis.</param>
    /// <param name="y">the controls screen coordinate on the y axis.</param>
    /// <param name="selectedIndex">the controls position in the control list.</param>
    /// <param name="cursorSize">the width and height of the cursor that'll be displayed when the control is selected.</param>
    SelectableControl(int x, int y, int selectedIndex, System::Drawing::Size ^cursorSize) : Control(x, y)
    {
        this->selectedIndex = selectedIndex;
        this->enabled = true;
        this->showMenuCursor = true;

        this->cursorSize = cursorSize;

        this->cursorPosition = gcnew System::Drawing::Point();

        // default position
        this->cursorPosition->X = this->Position.X - cursorSize->Width - sizeOfGapBetweenCursorAndMenuItems;
        this->cursorPosition->Y = this->Position.Y;
    }

    /// <summary>
    /// compares the current instance of SelectableControl against another.
    /// </summary>
    /// <param name="obj">the object to compare against.</param>
    /// <returns><b>-1</b> if <i>obj</i> is not a instance of SelectableControl or <i>obj</i> evaluates to <b>null</b>, or 
    /// the selected index of the calling object is less than that of <i>obj</i>.
    /// <b>0</b> if the value returned by the SelectedIndex property of <i>obj</i> matches the selected index of the calling object. 
    /// <b>1</b> if the selected index is greater than that of <i>obj</i>.</returns>
    /// <exception cref="System::ArgumentException"><i>obj</i> isn't of a type that inherits from SelectableControl.</exception>
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
    /// Specifies whether the control is enabled.
    /// </summary>
    /// <remarks>
    /// a control that's not enabled cannot be selected and is ignored by the MenuItemContainer 
    /// it belongs to when determining which control to select next.
    /// </remarks>
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
    /// Determines whether the cursor should be displayed when the controls is selected.
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

    /// <summary>
    /// Gets or sets the position the cursor should be rendered at when the control is selected.
    /// </summary>
    property System::Drawing::Point ^CursorPosition
    {
        System::Drawing::Point ^get()
        {
            return this->cursorPosition;
        }
        void set(System::Drawing::Point ^value)
        {
            this->cursorPosition = value;
        }
    }

    /// <summary>
    /// Gets or sets the cursors width and height.
    /// </summary>
    /// <exception cref="System::ArgumentNullException">attempting to assign a <b>null</b> value.</exception>
    property System::Drawing::Size ^CursorSize
    {
        System::Drawing::Size ^get()
        {
            return this->cursorSize;
        }

        void set(System::Drawing::Size ^value)
        {
            if(nullptr == value)
            {
                throw gcnew ArgumentNullException("value");
            }

            this->cursorSize = value;
        }
    }
};