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

#include "selectablecontrol.h"
#include "resourcemanager.h"
#include "surface.h"
#include "video.h"
#include "logmanager.h"

using namespace System::Collections::Generic;


/// <summary>
/// MenuItemContainers are in charge of selecting the appropriate MenuItem 
/// each time the user presses the down or up arrow keys. 
/// They also forward the input state to the currently selected control 
/// and handle the rendering of each control.
/// </summary>
public ref class MenuItemContainer : public SelectableControl
{
private:
    // controls contained within the container. Each control depends on the container 
    // to instruct it to render and pass on arguments received from the Scene they 
    // belong to each time the scene is displayed. This list is sorted by each controls
    // SelectedIndex value every time a control is added via MenuItemContainer::AddControl().
    List<SelectableControl ^> ^items;

    // the cursor to be displayed next to a child control, indicating it's the control that is selected.
    Surface ^menuItemCursor;

    // the arguments passed by the Scene object each time the scene is shown to the user.
    array<Object ^, 1> ^receivedArguments;

    // the index of the currently selected item in the items list. 
    // This is used to determine which control is to be selected next,
    // based on whether the user presses up (select previous control) or down (select the next control).
    int currentSelectedItem;
public:
    /// <summary>
    /// Initialises a new instance of MenuItemContainer with the specified cursor.
    /// </summary>
    /// <param name="cursor">the cursor to display next to controls contained within this menu item container.</param>
    /// <exception cref="System::ArgumentNullException"><i>cursor</i> is <b>null</b>.</exception>
    MenuItemContainer(Surface ^cursor) : SelectableControl(0, 0, 0, gcnew System::Drawing::Size())
    {
        if(nullptr == cursor)
        {
            throw gcnew ArgumentNullException("cursor");
        }

        this->items = gcnew List<SelectableControl ^>();
        this->IsSelected = true;
        this->menuItemCursor = cursor;
        this->CursorSize = cursor->Size;
    }

    /// <summary>
    /// Attaches a control to the container, so it can be rendered and manipulated. 
    /// </summary>
    /// <param name="control">the control to attach.</param>
    /// <remarks>controls also need to know which container the belong to. When Calling <i>MenuItemContainer::AddControl()</i>,
    /// you must also assign this instance of <i>MenuItemContainer</i> to the controls <i>ParentControl</i> property.</remarks>
    /// <exception cref="System::ArgumentNullException"><i>control</i> is <b>null</b>.</exception>
    void AddControl(SelectableControl ^control);

    /// <summary>
    /// Gets the controls assigned to the container.
    /// </summary>
    /// <returns>An array containing references to all the controls attached to the container.</returns>
    array<SelectableControl ^, 1> ^GetControls()
    {
        return items->ToArray();
    }

    /// <summary>
    /// Stores and forwards the arguments passed by the Scene to each control located within the container.
    /// </summary>
    /// <param name="args">the arguments to be passed to all controls within the container.</param>
    void ReceiveSceneArgs(array<Object ^, 1> ^args) override
    {
        this->receivedArguments = args;
		SelectFirstControl();

        for(int i = 0; i < this->items->Count; i++)
        {
            items[i]->ReceiveSceneArgs(args);
        }
    }

	/// <summary>
	/// Selects the first control in the control list.
	/// </summary>
	void SelectFirstControl();

    /// <summary>
    /// Selects the previous control in the control list.
    /// If the current control is the first in the list, the last is selected.
    /// </summary>
    /// <remarks>
    /// A controls position in the control list is determined by its SelectedIndex property.
    /// Controls are sorted in ascending order based on this value each time a control is added via
    /// MenuItemContainer::AddControl().
    /// </remarks>
    void SelectPreviousControl();

    /// <summary>
    /// Selects the next control in the control list. If the current control is the last control in the list,
    /// the first control is selected.
    /// </summary>
    /// <remarks>
    /// A controls position in the control list is determined by its SelectedIndex property.
    /// Controls are sorted in ascending order based on this value each time a control is added via
    /// MenuItemContainer::AddControl().
    /// </remarks>
    void SelectNextControl();

    /// <summary>
    /// Renders to the backbuffer each control that's attached to the container. A cursor 
    /// is rendered ajacent to the currently selected control.
    /// </summary>
    /// <remarks>
    /// When a control is created, it must define where it wants the cursor to appear when its selected.
    /// If this hasn't been set, it'll appear in the default location which is probably not where you want it.
    /// See SelectableControl::CursorPosition for more information.
    /// </remarks>
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
            for(int i = 0; i < this->items->Count; i++)
            {
                if(this->items[i]->IsSelected && items[i]->ShowMenuCursor)
                {
                    Video::Blit(items[i]->CursorPosition->X, items[i]->CursorPosition->Y, this->menuItemCursor);
                }

                this->items[i]->Render();
            }
        }
        catch(...)
        {
            throw;
        }
    }

    /// <summary>
    /// handles user input and for the currently selected control.
    /// </summary>
    /// <param name="keyboardState">current state of the keyboard.</param>
    /// <param name="mouseState">current state of the mouse.</param>
    void Update(Keys ^keyboardState, Mouse ^mouseState) override
    {
        if(nullptr == keyboardState)
        {
            throw gcnew ArgumentNullException("keyboardState");
        }

        this->items[this->currentSelectedItem]->Update(keyboardState, mouseState);
    }


    /*property System::Drawing::Size ^CursorSize
    {
        System::Drawing::Size ^get()
        {
            return gcnew System::Drawing::Size(menuItemCursor->Width, menuItemCursor->Height);
        }
    }*/
};
