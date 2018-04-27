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
#include "scenefactory.h"
#include "xmlhelper.h"
#include "resourcemanager.h"

using namespace System::Collections::Generic;

Label ^SceneFactory::ParseLabel(XElement ^element)
{
    try
    {
        int x = XmlHelper::GetAttributeValueAsInt32(element, "x");
        int y = XmlHelper::GetAttributeValueAsInt32(element, "y");
        String ^font = XmlHelper::GetAttributeValue(element, "font");
        String ^text = XmlHelper::GetAttributeValue(element, "text");

        return gcnew Label(x, y, font, text);
    }
    catch(...)
    {
        throw;
    }
}

MenuItem ^SceneFactory::ParseMenuItem(XElement ^element, MenuItemContainer ^parentContainer, String ^sceneID)
{
    try
    {
        int x = XmlHelper::GetAttributeValueAsInt32(element, "x");
        int y = XmlHelper::GetAttributeValueAsInt32(element, "y");
        String ^font = XmlHelper::GetAttributeValue(element, "font");
        String ^text = XmlHelper::GetAttributeValue(element, "text");
        int selectedIndex = XmlHelper::GetAttributeValueAsInt32(element, "selectedIndex");
        String ^navigateTo = XmlHelper::GetAttributeValue(element, "navigateTo");

        String ^navigationArgs = nullptr;

        try
        {
            navigationArgs = XmlHelper::GetAttributeValue(element, "navigationArgs");
            if("self" == navigationArgs)
            {
                navigationArgs = sceneID;
            }
        }
        catch(...)
        {
            // navigationArgs is an optional attribute, so ignore errors.
        }

        MenuItem ^menuItem;

        if(nullptr == navigationArgs)
        {
            menuItem = gcnew MenuItem(x, y, font, text, selectedIndex, navigateTo, parentContainer);
        }
        else
        {
           menuItem = gcnew MenuItem(x, y, font, text, selectedIndex, navigateTo, navigationArgs, parentContainer);
        }

        return menuItem;
    }
    catch(...)
    {
        throw;
    }
}

Slider ^SceneFactory::ParseSlider(XElement ^element, MenuItemContainer ^parentContainer)
{
    if(nullptr == element)
    {
        throw gcnew ArgumentNullException("element");
    }
    else if(nullptr == parentContainer)
    {
        throw gcnew ArgumentNullException("parentContainer");
    }

    try
    {
        int x = XmlHelper::GetAttributeValueAsInt32(element, "x");
        int y = XmlHelper::GetAttributeValueAsInt32(element, "y");
        unsigned int length = XmlHelper::GetAttributeValueAsInt32(element, "length");
        int selectedIndex = XmlHelper::GetAttributeValueAsInt32(element, "selectedIndex");
        double minimum = XmlHelper::GetAttributeValueAsDouble(element, "minimum");
        double maximum = XmlHelper::GetAttributeValueAsDouble(element, "maximum");
        double stepValue = XmlHelper::GetAttributeValueAsDouble(element, "stepValue");;
        String ^optionsKey = XmlHelper::GetAttributeValue(element, "optionsKey");

        return gcnew Slider(x, y, length, selectedIndex, minimum, maximum, stepValue, optionsKey, parentContainer);
    }
    catch(...)
    {
        throw;
    }
}

array<Scene ^, 1> ^SceneFactory::Read(XElement ^sceneXML)
{
    List<Scene ^> ^sceneList = gcnew List<Scene ^>();

    try
    {
        // <scene>
        System::Collections::Generic::IEnumerable<XElement ^> ^sceneQuery = sceneXML->Elements((String ^)"scene");
        for each(XElement ^sceneElement in sceneQuery)
        {
            String ^id = XmlHelper::GetAttributeValue(sceneElement, "id");
            bool initialScene = XmlHelper::GetAttributeValueAsBoolean(sceneElement, "initialScene");

            Scene ^newScene = gcnew Scene(id, initialScene);

            // <control>
            System::Collections::Generic::IEnumerable<XElement ^> ^controlsQuery = sceneElement->Descendants((String ^)"control");
            for each(XElement ^controlElement in controlsQuery)
            {
                String ^typeAttribute = XmlHelper::GetAttributeValue(controlElement, "type");
                if(typeAttribute == "label")
                {
                    Label ^label = ParseLabel(controlElement);
                    newScene->GetControlList()->Add(label);
                }
                else if(typeAttribute == "menuItemContainer")
                {
                    System::Collections::Generic::IEnumerable<XElement ^> ^containerQuery = sceneElement->Descendants((String ^)"containerItem");
                    MenuItemContainer ^menuItemContainer = gcnew MenuItemContainer(ResourceManager::GetSurface("menu_cursor"));

                    for each(XElement ^containerItem in containerQuery)
                    {
                        String ^containerItemType = XmlHelper::GetAttributeValue(containerItem, "type");

                        if(containerItemType == "menuItem")
                        {
                            MenuItem ^menuItem = ParseMenuItem(containerItem, menuItemContainer, newScene->Name);
                            menuItemContainer->AddControl(menuItem);
                        }
                        else if(containerItemType == "slider")
                        {
                            Slider ^slider = ParseSlider(containerItem, menuItemContainer);
                            menuItemContainer->AddControl(slider);
                        }
                    }

                    newScene->GetControlList()->Add(menuItemContainer);
                }
            }

            sceneList->Add(newScene);
        }

        return sceneList->ToArray();
    }
    catch(...)
    {
        throw;
    }
}