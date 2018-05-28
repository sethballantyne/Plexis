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

Credits ^SceneFactory::ParseCredits(XElement ^element)
{
    try
    {
        String ^descFont = XmlHelper::GetAttributeValue(element, "descriptionFont");
        String ^nameFont = XmlHelper::GetAttributeValue(element, "nameFont");

        return gcnew Credits(nameFont, descFont);
    }
    catch(...)
    {
        throw;
    }
}

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

KeyConfigLabel ^SceneFactory::ParseKeyConfigLabel(XElement ^element, MenuItemContainer ^parentContainer)
{
    try
    {
        int x = XmlHelper::GetAttributeValueAsInt32(element, "x");
        int y = XmlHelper::GetAttributeValueAsInt32(element, "y");
        String ^font = XmlHelper::GetAttributeValue(element, "font");
        int selectedIndex = XmlHelper::GetAttributeValueAsInt32(element, "selectedIndex");
        String ^optionsKey = XmlHelper::GetAttributeValue(element, "optionsKey");

        return gcnew KeyConfigLabel(x, y, font, selectedIndex, optionsKey, parentContainer);
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
        unsigned int length = XmlHelper::GetAttributeValueAsUInt32(element, "length");
        int selectedIndex = XmlHelper::GetAttributeValueAsInt32(element, "selectedIndex");
        double minimum = XmlHelper::GetAttributeValueAsInt32(element, "minimum");
        double maximum = XmlHelper::GetAttributeValueAsInt32(element, "maximum");
        double stepValue = XmlHelper::GetAttributeValueAsInt32(element, "stepValue");;
        String ^optionsKey = XmlHelper::GetAttributeValue(element, "optionsKey");

        return gcnew Slider(x, y, length, selectedIndex, minimum, maximum, stepValue, optionsKey, parentContainer);
    }
    catch(...)
    {
        throw;
    }
}

ToggleLabel ^SceneFactory::ParseToggleLabel(XElement ^element, MenuItemContainer ^parentContainer)
{
    try
    {
        int x = XmlHelper::GetAttributeValueAsInt32(element, "x");
        int y = XmlHelper::GetAttributeValueAsInt32(element, "y");
        int selectedIndex = XmlHelper::GetAttributeValueAsInt32(element, "selectedIndex");
        String ^font = XmlHelper::GetAttributeValue(element, "font");
        String ^optionsKey = XmlHelper::GetAttributeValue(element, "optionsKey");
        String ^falseCaption = XmlHelper::GetAttributeValue(element, "falseCaption");
        String ^trueCaption = XmlHelper::GetAttributeValue(element, "trueCaption");

        return gcnew ToggleLabel(x, y, font, trueCaption, falseCaption, selectedIndex, optionsKey, parentContainer);
    }
    catch(...)
    {
        throw;
    }
}

HighScoreTable ^SceneFactory::ParseHighScoreTable(XElement ^element)
{
    try
    {
        // reading <control>
        int x = XmlHelper::GetAttributeValueAsInt32(element, "x");
        int y = XmlHelper::GetAttributeValueAsInt32(element, "y");
        int numberOfRows = XmlHelper::GetAttributeValueAsInt32(element, "rows");

        // reading <row>
        XElement ^rowElement = element->Element("rows");
        int verticalSpacing = XmlHelper::GetAttributeValueAsInt32(rowElement, "verticalSpacing");

        // reading <column>
        System::Collections::Generic::IEnumerable<XElement ^> ^columnQuery = rowElement->Elements("column");
        if(XmlHelper::Count(columnQuery) < 3)
        {
            throw gcnew XmlException("HighScoreTable: not enough <column> elements defined.");
        }

        int indexXPosition;
        int playerNameXPosition;
        int scoreXPosition;
        String ^indexFont;
        String ^playerNameFont;
        String ^scoreFont;

        for each(XElement ^element in columnQuery)
        {
            String ^id = XmlHelper::GetAttributeValue(element, "id");

            if("index" == id)
            {
                indexXPosition = XmlHelper::GetAttributeValueAsInt32(element, "x");
                indexFont = XmlHelper::GetAttributeValue(element, "font");
            }
            else if("playerName" == id)
            {
                playerNameXPosition = XmlHelper::GetAttributeValueAsInt32(element, "x");
                playerNameFont = XmlHelper::GetAttributeValue(element, "font");
            }
            else if("score" == id)
            {
                scoreXPosition = XmlHelper::GetAttributeValueAsInt32(element, "x");
                scoreFont = XmlHelper::GetAttributeValue(element, "font");
            }
            else
            {
                throw gcnew XmlException("HighScoreTable: id attribute in column element has an illegal value.");
            }
        }

        return gcnew HighScoreTable(x, y, numberOfRows, verticalSpacing, indexXPosition, indexFont,
            playerNameXPosition, playerNameFont, scoreXPosition, scoreFont);
    }
    catch(...)
    {
        throw;
    }
}

ImageControl ^SceneFactory::ParseImageControl(XElement ^element)
{
    try
    {
        int x = XmlHelper::GetAttributeValueAsInt32(element, "x");
        int y = XmlHelper::GetAttributeValueAsInt32(element, "y");
        String ^imageName = XmlHelper::GetAttributeValue(element, "image");

        return gcnew ImageControl(x, y, imageName);
    }
    catch(...)
    {
        throw;
    }
}

EditableLabel ^SceneFactory::ParseEditableLabel(XElement ^element, MenuItemContainer ^parentContainer)
{
    try
    {
        int x = XmlHelper::GetAttributeValueAsInt32(element, "x");
        int y = XmlHelper::GetAttributeValueAsInt32(element, "y");
        int selectedIndex = XmlHelper::GetAttributeValueAsInt32(element, "selectedIndex");
        int length = XmlHelper::GetAttributeValueAsInt32(element, "length");
        String ^font = XmlHelper::GetAttributeValue(element, "font");
        bool allowEmptyInput = XmlHelper::GetAttributeValueAsBoolean(element, "allowEmptyInput");

        String ^navigateTo;
        try
        {
            navigateTo = XmlHelper::GetAttributeValue(element, "navigateTo");
        }
        catch(...)
        {
            navigateTo = nullptr;
        }

        if(nullptr == navigateTo)
        {
            return gcnew EditableLabel(x, y, selectedIndex, font, length, allowEmptyInput, parentContainer);
        }
        else
        {
            return gcnew EditableLabel(x, y, selectedIndex, font, length, allowEmptyInput, navigateTo, parentContainer);
        }
    }
    catch(...)
    {
        throw;
    }
}

VolumeSlider ^SceneFactory::ParseVolumeSlider(XElement ^element, MenuItemContainer ^parentContainer)
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
        unsigned int length = XmlHelper::GetAttributeValueAsUInt32(element, "length");
        int selectedIndex = XmlHelper::GetAttributeValueAsInt32(element, "selectedIndex");
        double minimum = XmlHelper::GetAttributeValueAsInt32(element, "minimum");
        double maximum = XmlHelper::GetAttributeValueAsInt32(element, "maximum");
        double stepValue = XmlHelper::GetAttributeValueAsInt32(element, "stepValue");;
        String ^optionsKey = XmlHelper::GetAttributeValue(element, "optionsKey");

        return gcnew VolumeSlider(x, y, length, selectedIndex, minimum, maximum, stepValue, optionsKey, parentContainer);
    }
    catch(...)
    {
        throw;
    }
}

VersionLabel ^SceneFactory::ParseVersionLabel(XElement ^element)
{
    try
    {
        int x = XmlHelper::GetAttributeValueAsInt32(element, "x");
        int y = XmlHelper::GetAttributeValueAsInt32(element, "y");
        String ^font = XmlHelper::GetAttributeValue(element, "font");

        return gcnew VersionLabel(x, y, font);
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
            bool initialScene = false;
            try
            {
                initialScene = XmlHelper::GetAttributeValueAsBoolean(sceneElement, "initialScene");
            }
            catch(...)
            {
                // the initialScene attribute is optional; failure to parse this attribute just means we use
                // the initialScene variables default value.
            }

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
                else if(typeAttribute == "highScoreTable")
                {
                    HighScoreTable ^highScoreTable = ParseHighScoreTable(controlElement);
                    newScene->GetControlList()->Add(highScoreTable);
                }
                else if(typeAttribute == "imageControl")
                {
                    ImageControl ^imageControl = ParseImageControl(controlElement);
                    newScene->GetControlList()->Add(imageControl);
                }
                else if(typeAttribute == "versionLabel")
                {
                    VersionLabel ^versionLabel = ParseVersionLabel(controlElement);
                    newScene->GetControlList()->Add(versionLabel);
                }
                else if(typeAttribute == "credits")
                {
                    Credits ^credits = ParseCredits(controlElement);
                    newScene->GetControlList()->Add(credits);
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
                        else if(containerItemType == "toggleLabel")
                        {
                            ToggleLabel ^toggleLabel = ParseToggleLabel(containerItem, menuItemContainer);
                            menuItemContainer->AddControl(toggleLabel);
                        }
                        else if(containerItemType == "keyConfigLabel")
                        {
                            KeyConfigLabel ^keyConfigLabel = ParseKeyConfigLabel(containerItem, menuItemContainer);
                            menuItemContainer->AddControl(keyConfigLabel);
                        }
                        else if(containerItemType == "editableLabel")
                        {
                            EditableLabel ^editableLabel = ParseEditableLabel(containerItem, menuItemContainer);
                            menuItemContainer->AddControl(editableLabel);
                        }
                        else if(containerItemType == "volumeSlider")
                        {
                            VolumeSlider ^volumeSlider = ParseVolumeSlider(containerItem, menuItemContainer);
                            menuItemContainer->AddControl(volumeSlider);
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