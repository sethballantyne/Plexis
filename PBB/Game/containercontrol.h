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
#include "menuitemcontainer.h"

/// <summary>
/// Base class for controls that are contained within a MenuItemContainer.
/// </summary>
public ref class ContainerControl abstract : public SelectableControl
{
private:
    MenuItemContainer ^parentContainer;

public:
    /// <summary>
    /// Initialises an instance of ContainerControl
    /// </summary>
    /// <param name="x">the controls x coordinate.</param>
    /// <param name="y">the controls y coordinate.</param>
    /// <param name="selectedIndex">the controls position in the control list.</param>
    /// <param name="parentContainer">the MenuItemContainer instance the control belongs to.</param>
    /// <exception cref="System::ArgumentNullException"><i>parentContainer</i> is <b>null</b>.</exception>
    ContainerControl(int x, int y, int selectedIndex, MenuItemContainer ^parentContainer) : SelectableControl(x, y, selectedIndex, parentContainer->CursorSize)
    {
        if(nullptr == parentContainer)
        {
            throw gcnew ArgumentNullException("parentContainer");
        }

        this->parentContainer = parentContainer;
    }

    /// <summary>
    /// Gets or sets the instance of MenuItemContainer the control belongs to.
    /// </summary>
    /// <exception cref="System::ArgumentNullException">attempted to assign a <b>null</b> value to the property.</exception>
    property MenuItemContainer ^ParentContainer
    {
        MenuItemContainer ^get()
        {
            return this->parentContainer;
        }

        void set(MenuItemContainer ^value)
        {
            if(nullptr == value)
            {
                throw gcnew ArgumentNullException("value", "assigning null to this property is not allowed.");
            }
            else
            {
                this->parentContainer = value;
                //this->parentContainer->GetControlList()->Add(this);
            }
        }
    }
};