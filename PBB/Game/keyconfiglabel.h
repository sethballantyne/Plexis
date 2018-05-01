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
#include "input.h"
#include "gameoptions.h"
#include "label.h"

using namespace System::Collections::Generic;

/// <summary>
/// KeyConfigLabel is used to map a particular key to a game option; typically player movement and the like.
/// When the label is selected and the user presses enter, a prompt to press a key is displayed.
/// when the user presses a key within a specified range, the option is set to that key and the name of the key
/// is displayed by the label until the user binds another key.
/// </summary>
public ref class KeyConfigLabel : ContainerControl
{
private:
    // used to display the controls text
    Label ^label;
  
    // the option to update whenever the key is changed. 
    String ^optionsKey;

    // when true, the control is listening for a keypress that will be mapped to the option assigned
    // in the constructor via optionsKey.
    bool changingKey;
    
    bool firstRun = true;

    /// <summary>
    /// Checks to see if the specified key is currently assigned to any other KeyConfigLabel instances within the
    /// current scene.
    /// </summary>
    /// <param name="key">the key to check for; should be a DIK_* key code.</param>
    /// <returns><b>true</b> if the key is assigned to another instance of KeyConfigLabel, otherwise <b>false</b>.</returns>
    bool KeyInUse(unsigned char key);

    /// <summary>
    /// Updates the control and GameOptions when a new key is assigned.
    /// </summary>
    /// <param name="key">the new key associated with the control, in the form of a DIK_* key code.</param>
    void UpdateConfig(unsigned char key)
    {
        try
        {
            String ^keyText = Input::GetDIKAsString(key);
            if(keyText != nullptr)
            {
                GameOptions::SetValue(optionsKey, key);
                GameOptions::Save();

                label->Text = keyText;
                
            }
        }
        catch(KeyNotFoundException ^)
        {
            // ignore it; invalid key pressed.
            // stick a sound prompt here.
        }
        catch(...)
        {
            throw;
        }
    }

public:
    /// <summary>
    /// Creates a new instance of KeyConfigLabel.
    /// </summary>
    /// <param name="x">the controls screen coordinate on the x axis.</param>
    /// <param name="y">the controls screen coordinate on the y axis.</param>
    /// <param name="font">the name of the font that will be used to render the text.</param>
    /// <param name="selectedIndex">the controls position in the control list.</param>
    /// <param name="optionsKey">the option in GameOptions that should be revised each time the control is toggled.</param>
    /// <param name="parentContainer">the MenuItemContainer instance the control belongs to.</param>
    KeyConfigLabel(int x, int y, String ^font, int selectedIndex, String ^optionsKey, MenuItemContainer ^parentContainer);

    /// <summary>
    /// handles arguments passed to the control via its parent scene.
    /// </summary>
    /// <param name="args">the arguments to interpret.</param>
    void ReceiveSceneArgs(array<Object ^, 1> ^args) override;

    /// <summary>
    /// Renders the controls text to the backbuffer.
    /// </summary>
    ///<exception cref = "System::Runtime::InteropServices::COMException">An unspecified COM error was returned.< / exception>
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
    /// Handles user input related to the state of the control.
    /// </summary>
    /// <param name="keyboardState">current state of the keyboard.</param>
    /// <param name="mouseState">current state of the mouse.</param>
    /// <exception cref="System::ArgumentNullException"><i>keyboardState</i> is <b>null</b>.</exception>
    void Update(Keys ^keyboardState, Mouse ^mouseState) override;
};