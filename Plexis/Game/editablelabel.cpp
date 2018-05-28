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
#include "editablelabel.h"
#include "input.h"
#include "scenemanager.h"
#include "highscores.h"

EditableLabel::EditableLabel(int x, int y, int selectedIndex, String ^font, unsigned int length, bool allowEmptyInput, MenuItemContainer ^parentContainer)
    : ContainerControl(x, y, selectedIndex, parentContainer)
{
    if(nullptr == font)
    {
        throw gcnew ArgumentNullException("font");
    }
    else if(nullptr == parentContainer)
    {
        throw gcnew ArgumentNullException("parentContainer");
    }

    if(String::Empty == font)
    {
        throw gcnew ArgumentException("font evaluates to String::Empty.");
    }

    maxLength = length;
    text = gcnew array<unsigned char, 1>(maxLength);
    cursorPosition = 0;
    editMode = true;
    this->allowEmptyInput = allowEmptyInput;

    try
    {
        this->font = ResourceManager::GetFont(font);
    }
    catch(...)
    {
        throw;
    }
}

void EditableLabel::Update(Keys ^keyboardState, Mouse ^mouseState)
{
    if(nullptr == keyboardState)
    {
        throw gcnew ArgumentNullException("keyboardState");
    }

    if(editMode)
    {
        int dikCode = keyboardState->PressedKey;
        if(dikCode != -1)
        {
            unsigned int asciiCode = Input::DIKToASCII(dikCode);
            if(asciiCode >= KEY_SPACE && asciiCode <= KEY_TILDE)
            {
                // is the buffer filled?
                if(cursorPosition < (text->Length - 1))
                {
                    // nope, stick the key that was pressed into the cursors position
                    text[cursorPosition] = asciiCode;
                    
                    // and move the cursor.
                    cursorPosition++;
                }
            }
            else
            {
                // pressed a key we don't want to print.
                // certain keys have special actions, others we just ignore.
                switch(dikCode)
                {
                    case DIK_BACKSPACE:
                        if(cursorPosition != 0)
                        {
                            cursorPosition--;
                            text[cursorPosition] = 0;
                        }
                        break;

                    case DIK_RETURN:
                        if(text[0] == 0 && !allowEmptyInput)
                        {
                            ResourceManager::GetSoundBuffer("error3")->Play();
                        }
                        else
                        {
                            editMode = false;
                            if(nullptr != navigateTo)
                            {
                                // TEST CODE FOR PASSING PLAYER NAME AND A SCORE
                                // TO THE HIGHSCORE TABLE.
                                // VERY MUCH TEMPORARY.
                                int newScoreRank = 0;
                                HighScores::Update(0, text, 99999);
                                array<Object ^, 1> ^args = gcnew array<Object ^, 1>(1);
                                args[0] = newScoreRank;
                                // END TEST CODE.
                                SceneManager::SetActiveScene(navigateTo, args);
                            }
                        }
                        break;
                    default:
                        break;
                }
            }
        }
    }
}

void EditableLabel::Render()
{
    try
    {
        font->Render(position->X, position->Y, text);
        
        int x = position->X + (cursorPosition * 17);
        if(editMode)
        {
            font->Render(x, position->Y, "_");
        }
    }
    catch(...)
    {
        throw;
    }
}