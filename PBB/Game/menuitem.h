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
#include "menuitemcontainer.h"
#include "containercontrol.h"

using namespace System;

/// <summary>
/// an interactive label that's primarily used to transition between scenes.
/// </summary>
public ref class MenuItem : ContainerControl
{
    // Labels and MenuItems are the same in the sense that they both are a string of text
    // rendered to the screen, so an internal copy of a label is kept to handle this aspect
    // of the menu item.
    Label ^label;

    // the scene that should be displayed if the user hits the select key (currently the enter key)
    String ^navigateTo;

    // arguments that were read from the controls "navigationArgs" attribute in the XML file.
    // these are passed to the new scene when a scene transition is requested.
    String ^outboundArgs;

    // arguments that were passed to the scene and subsequently this control, when the scene
    // this control belongs to became visible to user.
    array<Object ^, 1> ^receivedArgs;
public:
    /// <summary>
    /// Initialises a new instance of MenuItem.
    /// </summary>
    /// <param name="x">the</param>
    /// <param name="y"></param>
    /// <param name="font"></param>
    /// <param name="text"></param>
    /// <param name="selectedIndex"></param>
    /// <param name="navigateTo"></param>
    /// <param name="parentContainer"></param>
    MenuItem(int x, int y, String ^font, String ^text, int selectedIndex, String ^navigateTo, MenuItemContainer ^parentContainer);

    /// <summary>
    /// 
    /// </summary>
    /// <param name="x"></param>
    /// <param name="y"></param>
    /// <param name="font"></param>
    /// <param name="text"></param>
    /// <param name="selectedIndex"></param>
    /// <param name="navigateTo"></param>
    /// <param name="navigationArgs"></param>
    /// <param name="parentContainer"></param>
    MenuItem(int x, int y, String ^font, String ^text, int selectedIndex, String ^navigateTo,
        String ^navigationArgs, MenuItemContainer ^parentContainer) : MenuItem(x, y, font, text, selectedIndex, navigateTo, parentContainer)
    {
        this->outboundArgs = navigationArgs;
    }

    
    /// <summary>
    /// 
    /// </summary>
    /// <param name="keyboardState"></param>
    /// <param name="mouseState"></param>
    void Update(Keys ^keyboardState, Mouse ^mouseState) override;

    /// <summary>
    /// 
    /// </summary>
    /// <param name="args"></param>
    void ReceiveSceneArgs(array<Object ^, 1> ^args) override
    {
        this->receivedArgs = args;
    }

    /// <summary>
    /// 
    /// </summary>
    void Render() override
    {
        try
        {
            this->label->Render();
        }
        catch(...)
        {
            throw;
        }
    }

    
};