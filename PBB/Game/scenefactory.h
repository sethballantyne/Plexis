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
#include "scene.h"
#include "label.h"
#include "menuitem.h"
#include "menuitemcontainer.h"
#include "keyconfiglabel.h"
#include "slider.h"
#include "togglelabel.h"
#include "logmanager.h"
#include "highscoretable.h"
#include "editablelabel.h"

using namespace System::Xml::Linq;

/// <summary>
/// Produces scenes based on the data stored within scenes.xml.
/// </summary>
public ref class SceneFactory
{
private:
    
    void Log(String ^message, ...array<Object ^> ^args)
    {
        LogManager::WriteLine(LogType::Debug, message, args);
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="element"></param>
    /// <returns></returns>
    EditableLabel ^ParseEditableLabel(XElement ^element, MenuItemContainer ^parentContainer);

    /// <summary>
    /// Parses a control element whose value of its <i>type</i> attribute is "highScoreTable".
    /// </summary>
    /// <param name="element">the element to parse.</param>
    /// <returns>an instance of <i>HighScoreTable</i> which has been initialised with the parsed values.</returns>
    /// <exception cref="System::ArgumentNullException"><i>element</i> is <b>null</b>.</exception>
    /// <exception cref="System::FormatException">the value  of the element's <i>x</i> or <i>y</i>
    ///  attributes contain non-integer values.</exception>
    /// <exception cref="System::Xml::XmlException">the element or one of its children is missing a required 
    /// attribute or the value of an attribute evaluates to String::Empty.</exception>
    /// <exception cref="ResourceNotFoundException">the resource manager doesn't have the specified font.</exception>
    /// <exception cref="System::OverflowException">one of the attributes that takes a numberic value contains a value that's 
    /// less than Int32::MinValue or greater than Int32::MaxValue.</exception>
    /// <exception cref="System::FormatException">the value of an attribute that takes a numeric argument 
    /// contains a non-integer value.</exception>
    HighScoreTable ^ParseHighScoreTable(XElement ^element);

    /// <summary>
    /// Parses a control element whose value of its <i>type</i> attribute is "label".
    /// that was created using the parsed settings.
    /// </summary>
    /// <param name="element">the element to parse.</param>
    /// <returns>an instance of <i>Label</i> which has been initialised using the parsed values.</returns>
    /// <exception cref="System::ArgumentNullException"><i>element</i> is <b>null</b>.</exception>
    /// <exception cref="System::FormatException">the value  of the element's <i>x</i> or <i>y</i>
    ///  attributes contain non-integer values.</exception>
    /// <exception cref="System::OverflowException">either the <i>x</i> or <i>y</i> attribute contains a value 
    /// that's less than Int32::MinValue or greater than Int32::MaxValue.</exception>
    /// <exception cref="ResourceNotFoundException">the resource manager doesn't have the specified font.</exception>
    /// <exception cref="System::Xml::XmlException">the element is missing a required 
    /// attribute or the value of an attribute evaluates to String::Empty.</exception>
    Label ^ParseLabel(XElement ^element);

    /// <summary>
    /// Parses a containerItemElement element whose value of its <i>type</i> attribute is "menuItem".
    /// </summary>
    /// <param name="element">the element to parse.</param>
    /// <param name="parentContainer">the menu item container the control will be attached to.</param>
    /// <param name="sceneID">the name of the current scene being parsed.</param>
    /// <returns>an instance of <i>MenuItem</i> which has been initialised using the parsed values.</returns>
    /// <exception cref="System::ArgumentNullException">either <i>element</i> or <i>parentContainer</i> is <b>null</b>.</exception>
    /// <exception cref="System::FormatException">the value  of the element's <i>x</i> or <i>y</i> attributes contain non-integer values.</exception>
    /// <exception cref="System::OverflowException">either the <i>x</i> or <i>y</i> attribute contains a value that's less than Int32::MinValue or greater than Int32::MaxValue.</exception>
    /// <exception cref="ResourceNotFoundException">the resource manager doesn't have the specified font.</exception>
    /// <exception cref="System::Xml::XmlException">the element is missing a required attribute or the value of an attribute evaluates to String::Empty.</exception>
    MenuItem ^ParseMenuItem(XElement ^element, MenuItemContainer ^parentContainer, String ^sceneID);

    /// <summary>
    /// Parses a <i>containerItem</i> element whose <i>type</i> attribute evaluates to <b>slider</b>.
    /// </summary>
    /// <param name="element">the element to parse.</param>
    /// <param name="parentContainer">the menu item container the control will be attached to.</param>
    /// <returns>an instance of Slider created from the parsed XML.</returns>
    /// <exception cref="System::ArgumentNullException">either <i>element</i> or <i>parentContainer</i> is <b>null</b>.</exception>
    /// <exception cref="System::FormatException">one of the attributes that takes a numeric argument contains a non-integer value.</exception>
    /// <exception cref="System::OverflowException">one of the attributes that takes a numeric argument contains a value that's less than Int32::MinValue or greater than Int32::MaxValue.</exception>
    /// <exception cref="System::Xml::XmlException">the element is missing a required attribute or the value of an attribute evaluates to String::Empty.</exception>
    Slider ^ParseSlider(XElement ^element, MenuItemContainer ^parentContainer);

    /// <summary>
    /// Parses a </i>containerItem</i> element whose <i>type</i> attribute evaluates to <b>keyConfigLabel</b>.
    /// </summary>
    /// <param name="element">the element to parse.</param>
    /// <param name="parentContainer">the menu item container the control will be attached to.</param>
    /// <returns>an instance of KeyConfigLabel created from the parsed XML.</returns>
    /// <exception cref="System::ArgumentNullException">
    /// either <i>element</i> or <i>parentContainer</i> is <b>null</b>.
    /// </exception>
    /// <exception cref="System::ArgumentNullException">either <i>element</i> or <i>parentContainer</i> is <b>null</b>.</exception>
    /// <exception cref="System::FormatException">one of the attributes that takes a numeric argument contains a non-integer value.</exception>
    /// <exception cref="System::OverflowException">one of the attributes that takes a numeric argument contains a value that's less than Int32::MinValue or greater than Int32::MaxValue.</exception>
    /// <exception cref="ResourceNotFoundException">the resource manager doesn't have the specified font.</exception>
    /// <exception cref="System::Xml::XmlException">the element is missing a required attribute or the value of an attribute evaluates to String::Empty.</exception>
    KeyConfigLabel ^ParseKeyConfigLabel(XElement ^element, MenuItemContainer ^parentContainer);

    /// <summary>
    /// Parses a </i>&lt;containerItem&gt;</i> element whose <i>type</i> attribute evaluates to <b>toggleLabel.</b>.
    /// </summary>
    /// <param name="element">the element to parse.</param>
    /// <param name="parentContainer">the menu item container the control will be attached to.</param>
    /// <returns>an instance of keyConfigLabel created from the parsed XML.</returns>
    /// <exception cref="System::ArgumentNullException">either <i>element</i> or <i>parentContainer</i> is <b>null</b>.</exception>
    /// <exception cref="System::FormatException">one of the attributes that takes a numeric argument contains a non-integer value.</exception>
    /// <exception cref="System::OverflowException">one of the attributes that takes a numeric argument contains a value that's less than Int32::MinValue or greater than Int32::MaxValue.</exception>
    /// <exception cref="ResourceNotFoundException">the resource manager doesn't have the specified font.</exception>
    /// <exception cref="System::Xml::XmlException">the element is missing a required attribute or the value of an attribute evaluates to String::Empty.</exception>
    ToggleLabel ^ParseToggleLabel(XElement ^element, MenuItemContainer ^parentContainer);
public:
    /// <summary>
    /// 
    /// </summary>
    /// <param name="sceneXML"></param>
    /// <returns></returns>
    /// <exception cref=""></exception>
    array<Scene ^, 1> ^Read(XElement ^sceneXML);
};