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
#include "entitymanager.h"

using namespace System::Xml;

Frame ^EntityManager::ParseFrame(XElement ^frameElement)
{
    System::Drawing::Rectangle frame;
    System::Drawing::Rectangle boundingBox;

    try
    {
        frame.X = XmlHelper::GetAttributeValueAsInt32(frameElement, "x");
        frame.Y = XmlHelper::GetAttributeValueAsInt32(frameElement, "y");
        frame.Width = XmlHelper::GetAttributeValueAsInt32(frameElement, "width");
        frame.Height = XmlHelper::GetAttributeValueAsInt32(frameElement, "height");

        XElement ^boundingBoxElement = frameElement->Element((String ^) "boundingBox");
        if(nullptr != boundingBoxElement)
        {
            boundingBox.X = XmlHelper::GetAttributeValueAsInt32(boundingBoxElement, "x");
            boundingBox.Y = XmlHelper::GetAttributeValueAsInt32(boundingBoxElement, "y");
            boundingBox.Width = XmlHelper::GetAttributeValueAsInt32(boundingBoxElement, "width");
            boundingBox.Height = XmlHelper::GetAttributeValueAsInt32(boundingBoxElement, "height");
        }

        return gcnew Frame(frame, boundingBox);
    }
    catch(...)
    {
        throw;
    }
}

void EntityManager::ParseBrick(XElement ^brickElement)
{
    try
    {
        String ^name = XmlHelper::GetAttributeValue(brickElement, "name");
        String ^image = XmlHelper::GetAttributeValue(brickElement, "image");
        unsigned int hitCount = XmlHelper::GetAttributeValueAsUInt32(brickElement, "hitCount");
        unsigned int chanceOfPowerUp = XmlHelper::GetAttributeValueAsUInt32(brickElement, "chanceOfPowerUp");
        unsigned int points = XmlHelper::GetAttributeValueAsUInt32(brickElement, "points");

        System::Collections::Generic::IEnumerable<XElement ^> ^frameQuery = brickElement->Elements((String ^) "frame");
        if(0 == XmlHelper::Count(frameQuery))
        {
            throw gcnew XmlException(String::Format("brick {0} doesn't have any frames associated with it.", name));
        }

        List<Frame ^> ^frames = gcnew List<Frame ^>();
        for each(XElement ^frameElement in frameQuery)
        {
            frames->Add(ParseFrame(frameElement));
        }

        Sprite ^brickSprite = gcnew Sprite(0, 0, frames->ToArray(), image);
        Brick ^brick = gcnew Brick(brickSprite, hitCount, chanceOfPowerUp, points);

        parsedEntities[name] = brick;
    }
    catch(...)
    {
        throw;
    }
}

void EntityManager::Initialise(XElement ^entitiesFile)
{
    try
    {
        System::Collections::Generic::IEnumerable<XElement ^> ^entitiesQuery = entitiesFile->Elements((String ^)"entity");
        if(0 == XmlHelper::Count(entitiesQuery))
        {
            throw gcnew XmlException("the entities file doesn't contain any entity tags!");
        }

        for each(XElement ^entityElement in entitiesQuery)
        {
            String ^entityType = XmlHelper::GetAttributeValue(entityElement, "type");

            if("brick" == entityType)
            {
                ParseBrick(entityElement);
            }
        }
    }
    catch(...)
    {
        throw;
    }
}