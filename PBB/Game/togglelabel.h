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
#include "containercontrol.h"
#include "gameoptions.h"
#include "label.h"

/// <summary>
///  A ToggleLabel is a label that acts like a toggle switch. When the user presses enter, the label alternates
/// between the two strings specified in its "trueCaption" and "falseCaption" XML attributes.
/// When the control alternates between these two values, it automatically updates the setting referenced
/// by its XML attribute "optionsKey". 
/// </summary>
public ref class ToggleLabel : ContainerControl
{
private:
    // specifies whether the control is currently displaying its falseCaption or trueCaption text.
    bool toggleState;

    // the strings used to denote when the control is in a false or true state.
    // fale is always the 0th element.
    array<String ^, 1> ^captions;

    // the option to update each time the control is toggled.
    String ^optionsKey;

    // used to render the controls text.
    Label ^label;

    void Toggle()
    {
        toggleState = !toggleState;

        GameOptions::SetValue(optionsKey, toggleState);
        GameOptions::Save();
        label->Text = captions[toggleState];
    }

public:
    /// <summary>
    /// Initialises a new instance of ToggleLabel.
    /// </summary>
    /// <param name="x">the controls screen coordinate on the x axis.</param>
    /// <param name="y">the controls screen coordinate on the y axis.</param>
    /// <param name="font">the name of the font that will be used to render the text.</param>
    /// <param name="trueCaption">the text to display indicate the control is in a true state.</param>
    /// <param name="falseCaption">the text to display to indicate the control is in a false state.</param>
    /// <param name="selectedIndex">the controls position in the control list.</param>
    /// <param name="optionsKey">the option in GameOptions that should be revised each time the control is toggled.</param>
    /// <param name="parentContaier">the MenuItemContainer instance the control belongs to.</param>
    ToggleLabel(int x, int y, String ^font, String ^trueCaption, String ^falseCaption,
        int selectedIndex, String ^optionsKey, MenuItemContainer ^parentContaier);

    /// <summary>
    /// handles arguments passed to the control via its parent scene.
    /// </summary>
    /// <param name="sceneArgs">the argments to pass to the control.</param>
    /// <remarks>
    /// a toggle label doesn't require or parse arguments so this does nothing.
    /// </remarks>
    void ReceiveSceneArgs(array<Object ^, 1> ^sceneArgs) override
    {
        // this control doesn't take any arguments.
    }

    /// <summary>
    /// Renders the controls text to the backbuffer.
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
            label->Render();
        }
        catch(...)
        {
            throw;
        }
    }

    /// <summary>
    /// Updates the control state, based on the users input.
    /// </summary>
    /// <param name="keyboardState"></param>
    /// <param name="mouseState"></param>
    void Update(Keys ^keyboardState, Mouse ^mouseState) override
    {
        if(nullptr == keyboardState)
        {
            throw gcnew ArgumentNullException("keyboardState");
        }

        if(keyboardState->KeyPressed(DIK_RETURN))
        {
            Toggle();
        }
        else if(keyboardState->KeyPressed(DIK_DOWN))
        {
            ParentContainer->SelectNextControl();
        }
        else if(keyboardState->KeyPressed(DIK_UP))
        {
            ParentContainer->SelectPreviousControl();
        }
    }
};