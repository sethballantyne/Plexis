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
    /// <param name="x">the controls screen coordinate on the x axis.</param>
    /// <param name="y">the controls screen coordinate on the y axis.</param>
    /// <param name="font">the name of the font that will be used to render the text.</param>
    /// <param name="text">the caption to display to the user.</param>
    /// <param name="selectedIndex">the controls position in the control list.</param>
    /// <param name="navigateTo">the name of the Scene instance that should be shown to the user when the control is selected and enter is pressed.</param>
    /// <param name="parentContainer">the MenuItemContainer instance the control belongs to.</param>
    /// <exception cref="System::ArgumentException">either <i>font</i>, <i>text</i> or <i>navigateTo</i> evaluates to String::Empty.</exception>
    /// <exception cref="System::ArgumentNullException">either <i>font</i>, <i>text</i>, 
    /// <i>navigateTo</i> or <i>parentContainer</i> is <b>null</b>.</exception>
    /// <exception cref="ResourceNotFoundException">the specified font doesn't exist within the resource manager.</exception>
    MenuItem(int x, int y, String ^font, String ^text, int selectedIndex, String ^navigateTo, MenuItemContainer ^parentContainer);

    /// <summary>
    /// Initialises a new instance of MenuItem.
    /// </summary>
    /// <param name="x">the controls screen coordinate on the x axis.</param>
    /// <param name="y">the controls screen coordinate on the y axis.</param>
    /// <param name="font">the name of the font that will be used to render the text.</param>
    /// <param name="text">the caption to display to the user.</param>
    /// <param name="selectedIndex">the controls position in the control list.</param>
    /// <param name="navigateTo">the name of the Scene instance that should be shown to the user when the control is selected and enter is pressed.</param>
    /// <param name="navigationArgs">the arguments to pass to the new scene when a transition occurs.</param>
    /// <param name="parentContainer">the MenuItemContainer instance the control belongs to.</param>
    /// <exception cref="System::ArgumentException">either <i>font</i>, <i>text</i>, <i>navigateTo</i> or <i>navigationArgs</i> evaluates to String::Empty.</exception>
    /// <exception cref="System::ArgumentNullException">either <i>font</i>, 
    /// <i>text</i>, <i>navigateTo</i>, <i>navigationArgs</i> or <i>parentContainer</i> is <b>null</b>.</exception>
    /// <exception cref="ResourceNotFoundException">the specified font doesn't exist within the resource manager.</exception>
    MenuItem(int x, int y, String ^font, String ^text, int selectedIndex, String ^navigateTo,
        String ^navigationArgs, MenuItemContainer ^parentContainer) : MenuItem(x, y, font, text, selectedIndex, navigateTo, parentContainer)
    {
        if(String::Empty == navigationArgs)
        {
            throw gcnew System::ArgumentException("navigationArgs evaluates to String::Empty.");
        }

        this->outboundArgs = navigationArgs;
    }

    
    /// <summary>
    /// Handles user input related to the state of the control.
    /// </summary>
    /// <param name="keyboardState">current state of the keyboard.</param>
    /// <param name="mouseState">current state of the mouse.</param>
    void Update(Keys ^keyboardState, Mouse ^mouseState) override;

    /// <summary>
    /// handles arguments passed to the control via its parent scene.
    /// </summary>
    /// <param name="args">the arguments to interpret.</param>
    void ReceiveSceneArgs(array<Object ^, 1> ^args) override
    {
        this->receivedArgs = args;
    }

    /// <summary>
    /// Renders the control to the backbuffer.
    /// </summary>
    /// <exception cref="System::Runtime::InteropServices::COMException">An unspecified COM error was returned.</exception>
    /// <exception cref="DirectDrawGenericException">DirectDraw returned an unspecified error condition.</exception>
    /// <exception cref="DirectDrawInvalidClipListException">DirectDraw does not support the provided clip list.</exception>
    /// <exception cref="DirectDrawInvalidObjectException">DirectDraw received a pointer that was an invalid DirectDraw object.</exception>
    /// <exception cref="DirectDrawInvalidParametersException">one or more of the parameters passed to the method are incorrect.</exception>
    /// <exception cref="DirectDrawInvalidRectException">the rectangle coordinates used by the surface were invalid.</exception>
    /// <exception cref="DirectDrawNoAlphaHardwareException">no alpha acceleration hardware is present or available.</exception>
    /// <exception cref="DirectDrawNoBlitHardwareException">no blitter hardware is present.</exception>
    /// <exception cref="DirectDrawNoClipListException">no clip list is available.</exception>
    /// <exception cref="DirectDrawNoDDRasterOperationHardwareException">no DirectDraw raster operation (ROP) hardware is available.</exception>
    /// <exception cref="DirectDrawNoMirrorHardwareException">the operation cannot be carried out because no mirroring hardware is present or available.</exception>
    /// <exception cref="DirectDrawNoRasterOperationHardwareException">the operation cannot be carried out because no appropriate raster operation hardware is present or available.</exception>
    /// <exception cref="DirectDrawNoRotationHardwareException">the operation cannot be carried out because no rotation hardware is present or available.</exception>
    /// <exception cref="DirectDrawNoStretchHardwareException">the operation cannot be carried out because there is no hardware support for stretching.</exception>
    /// <exception cref="DirectDrawNoZBufferHardwareException">the operation cannot be carried out because there is no hardware support for Z-buffers.</exception>
    /// <exception cref="DirectDrawSurfaceBusyException">access to the surface is refused because the surface is locked by another thread.</exception>
    /// <exception cref="DirectDrawSurfaceLostException">access to the surface is refused because the surface memory is gone.</exception>
    /// <exception cref="DirectDrawUnsupportedException">the operation is not supported.</exception>
    /// <exception cref="DirectDrawWasStillDrawingException">the previous blit operation is incomplete.</exception>
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