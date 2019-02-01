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
#include "keyconfiglabel.h"
#include "gameoptions.h"


bool KeyConfigLabel::KeyInUse(unsigned char key)
{
    array<SelectableControl ^, 1> ^controls = ParentContainer->GetControls();
    for(int i = 0; i < controls->Length; i++)
    {
        if(controls[i]->GetType() == KeyConfigLabel::typeid && !ReferenceEquals(this, controls[i]))
        {
            KeyConfigLabel ^keyConfigLabel = dynamic_cast<KeyConfigLabel ^>(controls[i]);
            String ^keyText = Input::GetDIKAsString(key);

            // is the key in use?
            if(nullptr == keyText || keyConfigLabel->label->Text == keyText)
            {
                // if keyText evaluates to nullptr, the user has pressed a key that's not in the 
                // lookup table used by GetDIKAsString() to retrieve the string representation of the key.
                // If it's not in the table, it's an invalid key for our purposes, so pretend
                // it's already in use; this'll force them to bind another key and play the error
                // sound effect.
                return true;
            }
        }
    }

    return false;
}

KeyConfigLabel::KeyConfigLabel(int x, int y, String ^font, int selectedIndex, String ^optionsKey, MenuItemContainer ^parentContainer)
    : ContainerControl(x, y, selectedIndex, parentContainer)
{
    if(nullptr == optionsKey)
    {
        throw gcnew ArgumentNullException("optionsKey");
    }
    else if(nullptr == font)
    {
        throw gcnew ArgumentNullException("font");
    }
    else if(nullptr == parentContainer)
    {
        throw gcnew ArgumentNullException("parentContainer");
    }

    if(String::Empty == optionsKey)
    {
        throw gcnew ArgumentException("optionsKey evaluates to String::Empty.");
    }
    else if(String::Empty == font)
    {
        throw gcnew ArgumentException("font evaluates to String::Empty.");
    }

    this->optionsKey = optionsKey;

    try
    {
        label = gcnew Label(x, y, font, nullptr);
    }
    catch(...)
    {
        throw;
    }
}

void KeyConfigLabel::ReceiveSceneArgs(array<Object ^, 1> ^sceneArgs)
{
    if(firstRun)
    {
        firstRun = false;

        int keyValue = GameOptions::GetValue(optionsKey, -1);
        String ^caption = Input::GetDIKAsString(keyValue);

        if(nullptr == caption)
        {
            caption = "UNDEFINED";
			LogManager::WriteLine(LogType::Debug, "Key assigned to {0} is an illegal value.", optionsKey);
        }

        label->Text = caption;
    }
}

void KeyConfigLabel::Update(Keys ^keyboardState, Mouse ^mouseState)
{
    if(nullptr == keyboardState)
    {
        throw gcnew ArgumentNullException("keyboardState");
    }
	else if(nullptr == mouseState)
	{
		throw gcnew ArgumentNullException("keyboardState");
	}

    if(changingKey != true)
    {
        if(keyboardState->KeyPressed(DIK_RETURN))
        {
            changingKey = true;
            label->Text = "PRESS A KEY";
        }
        else if(keyboardState->KeyPressed(DIK_UP))
        {
            ParentContainer->SelectPreviousControl();
        }
        else if(keyboardState->KeyPressed(DIK_DOWN))
        {
            ParentContainer->SelectNextControl();
        }
    }
    else
    {
		if(mouseState->ButtonDown(0))
		{
			LogManager::WriteLine(LogType::Debug, "mouseState");
			UpdateConfig(0);
			changingKey = false;
		}
		else if(mouseState->ButtonDown(1))
		{
			UpdateConfig(1);
			changingKey = false;
		}
		else
		{
			unsigned int pressedKey = keyboardState->PressedKey;
			// in the off chance that the user presses keys and mouse buttons(s) at the same time,
			// the keyboard takes precendence.
			if(pressedKey != -1) // -1 means a key hasn't been pressed.
			{
				if(!KeyInUse(pressedKey))
				{
					UpdateConfig(pressedKey);
					changingKey = false;
				}
				else
				{
					// attempted to assign a key that's already in bound
					// to another option. Play a prompt indicating the error.
					ResourceManager::GetSoundBuffer("error3")->Play();
				}
			}
		}
    }
}