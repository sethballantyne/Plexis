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
  
    String ^optionsKey;

    // when true, the control is listening for a keypress that will be mapped to the option assigned
    // in the constructor via optionsKey.
    bool changingKey;

    bool firstRun = true;

    bool KeyInUse(unsigned char key);

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
    /// 
    /// </summary>
    /// <param name="x"></param>
    /// <param name="y"></param>
    /// <param name="font"></param>
    /// <param name="selectedIndex"></param>
    /// <param name="optionsKey"></param>
    /// <param name="parentContainer"></param>
    KeyConfigLabel(int x, int y, String ^font, int selectedIndex, String ^optionsKey, MenuItemContainer ^parentContainer);

    /// <summary>
    /// 
    /// </summary>
    /// <param name="args"></param>
    void ReceiveSceneArgs(array<Object ^, 1> ^args) override;

    /// <summary>
    /// 
    /// </summary>
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
    /// 
    /// </summary>
    /// <param name="keyboardState"></param>
    /// <param name="mouseState"></param>
    void Update(Keys ^keyboardState, Mouse ^mouseState) override;
};