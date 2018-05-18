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
#include "font.h"
#include "containercontrol.h"

/// <summary>
/// essentially behaves like a textbox with a single line. When the control is in edit mode, 
/// the user can type to add or remove text from the control in real-time. When not in edit mode,
/// the control functions like a regular label.
/// </summary>
public ref class EditableLabel : ContainerControl
{
private:
    // used as an index for the text array.
    unsigned int cursorPosition;

    // the maximum size the labels string can be.
    unsigned int maxLength;

    // the text to be rendered to the screen.
    array<unsigned char, 1> ^text;

    // determines whether the label is in edit mode (true) or "read only" mode (false).
    bool editMode;

    const unsigned int KEY_SPACE = 32;
    const unsigned int KEY_TILDE = 126;

    ::Font ^font;

    String ^navigateTo;

    bool allowEmptyInput;
public:
    /// <summary>
    /// Creates a new instance of EditableLabel.
    /// </summary>
    /// <param name="x">the controls screen coordinate on the x axis.</param>
    /// <param name="y">the controls screen coordinate on the y axis.</param>
    /// <param name="selectedIndex">the controls position in the control list.</param>
    /// <param name="font">the name of the font that will be used to render the text.</param>
    /// <param name="length">the maximum number of characters the label can contain.</param>
    /// <param name="parentContainer">the MenuItemContainer instance the control belongs to.</param>
    /// <exception cref="System::ArgumentException"><i>font</i> evaluates to String::Empty.</exception>
    /// <exception cref="System::ArgumentNullException"><i>font</i> or <i>parentContainer</i> evaluate to <b>null</b></exception>
    /// <exception cref="ResourceNotFoundException">the font specified in <i>font</i> doesn't exist within the resource manager.</exception>
    EditableLabel(int x, int y, int selectedIndex, String ^font, unsigned int length, bool allowEmptyInput, MenuItemContainer ^parentContainer);

    /// <summary>
    /// Creates a new instance of EditableLabel.
    /// </summary>
    /// <param name="x">the controls screen coordinate on the x axis.</param>
    /// <param name="y">the controls screen coordinate on the y axis.</param>
    /// <param name="selectedIndex">the controls position in the control list.</param>
    /// <param name="font">the name of the font that will be used to render the text.</param>
    /// <param name="length">the maximum number of characters the label can contain.</param>
    /// <param name="navigateTo">the scene that will displayed when the control is selected and the user presses enter.</param>
    /// <param name="parentContainer">the MenuItemContainer instance the control belongs to.</param>
    /// <exception cref="System::ArgumentException"><i>font</i> evaluates to String::Empty.</exception>
    /// <exception cref="System::ArgumentNullException"><i>font</i>, <i>parentContainer</i> or <i>navigateTo</i> evaluate to <b>null</b></exception>
    /// <exception cref="ResourceNotFoundException">the font specified in <i>font</i> doesn't exist within the resource manager.</exception>
    EditableLabel(int x, int y, int selectedIndex, String ^font, unsigned int length, bool allowEmptyInput, String ^navigateTo, MenuItemContainer ^parentContainer) :
        EditableLabel(x, y, selectedIndex, font, length, allowEmptyInput, parentContainer)
    {
        if(nullptr == navigateTo)
        {
            throw gcnew ArgumentNullException("navigateTo");
        }
        
        this->navigateTo = navigateTo;
    }

    /// <summary>
    /// handles arguments passed to the control via its parent scene when the Scene is displayed.
    /// </summary>
    /// <param name="args">the arguments to received from the Scene.</param>
    /// <remarks>this isn't used by this control.</remarks>
    void ReceiveSceneArgs(array<Object ^, 1> ^args) override
    {

    }

    /// <summary>
    /// Handles user input related to the state of the control.
    /// </summary>
    /// <param name="keyboardState">current state of the keyboard.</param>
    /// <param name="mouseState">current state of the mouse.</param>
    void Update(Keys ^keyboardState, Mouse ^mouseState) override;

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
    void Render() override;
};