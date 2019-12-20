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
#include "selectablecontrol.h"
#include "exceptions.h"

using namespace System::Collections::Generic;

/// <summary>
/// Encapsulates what the user is currently seeing on the screen. 
/// A scene can be the actual game itself, or one of the menu screens.
/// </summary>
public ref class Scene
{
private:
    // the scenes name as specified by its id attribute in the XML file.
    String ^name;

    // contains each control present in the scene.
    List<Control ^> ^controls;

    bool initialScene;

public:
    Scene()
    {
        this->name = name;
        this->controls = gcnew List<Control ^>();
        initialScene = false;
    }

    /// <summary>
    /// Initialises a new instance of Scene with the specified name.
    /// </summary>
    /// <param name="name">the scenes name; should be unique.</param>
    /// <exception cref="System::ArgumentException"><i>name</i> evaluates to String::Empty.</exception>
    /// <exception cref="System::ArgumentNullException"><i>name</i> evaluats to <b>null</b>.</exception>
    Scene(String ^name) : Scene()
    {
        if(nullptr == name)
        {
            throw gcnew ArgumentNullException("name");
        }
        else if(String::Empty == name)
        {
            throw gcnew ArgumentException("the scenes name evaluates to String::Empty.");
        }

        this->name = name;
    }

    /// <summary>
    /// Initialises the scene with the specified name and whether it's the first scene.
    /// </summary>
    /// <param name="name">the scenes name; should be unique.</param>
    /// <param name="initialScene"><b>true</b> specifies the scene is the first scene that should
    /// be shown to the user when the application starts (such as a main menu screen), 
    /// <b>false</b> specifies that it's not.</param>
    /// <exception cref="System::ArgumentException"><i>name</i> evaluates to String::Empty.</exception>
    /// <exception cref="System::ArgumentNullException"><i>name</i> evaluats to <b>null</b>.</exception>
    Scene(String ^name, bool initialScene) : Scene(name)
    {
        this->initialScene = initialScene;
    }

    /// <summary>
    /// Passes the arguments received via the SceneManager to the scenes controls.
    /// </summary>
    /// <param name="navigationArgs">the arguments received from the scene manager.</param>
    void BroadcastArguments(array<Object ^> ^navigationArgs)
    {
        for(int i = 0; i < controls->Count; i++)
        {
            if(controls[i]->GetType()->IsSubclassOf(SelectableControl::typeid))
            {
                SelectableControl ^temp = dynamic_cast<SelectableControl ^>(controls[i]);
                temp->ReceiveSceneArgs(navigationArgs);
            }
        }
    }

    /// <summary>
    /// returns the list containing the controls present within the scene.
    /// </summary>
    /// <returns></returns>
    List<Control ^> ^GetControlList()
    {
        return this->controls;
    }

    /// <summary>
    /// Forwards the state of the keyboard and mouse on to the currently selected control.
    /// keyboardState must not be <b>null</b>, mouseState may be.
    /// </summary>
    /// <param name="keyboardState"></param>
    /// <param name="mouseState"></param>
    /// <exception cref="System::ArgumentNullException"><i>keyboardState</i> is <b>null</b>.</exception>
    void Update(Keys ^keyboardState, Mouse ^mouseState);

    /// <summary>
    /// Instructs each control in the scene to draw itself to the backbuffer.
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
    void Render()
    {
        try
        {
            for(int i = 0; i < controls->Count; i++)
            {
                controls[i]->Render();
            }
        }
        catch(...)
        {
            throw;
        }
    }

    /// <summary>
    /// Gets or sets a boolean value specifying whether the scene is the initial
    /// scene to be displayed to the user when the application starts.
    /// </summary>
    property bool IsInitialScene
    {
        bool get()
        {
            return this->initialScene;
        }

        void set(bool value)
        {
            this->initialScene = value;
        }
    }

    /// <summary>
    /// Gets the name of the scene.
    /// </summary>
    property String ^Name
    {
        String ^get()
        {
            return this->name;
        }
    }
};