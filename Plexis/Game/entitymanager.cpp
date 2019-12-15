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
#include "paddle.h"
#include "ball.h"
#include "laserpowerup.h"
#include "laser.h"

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

void EntityManager::ParseBall(XElement ^ballElement)
{
	try
	{
		String ^name = XmlHelper::GetAttributeValue(ballElement, "name");
		String ^image = XmlHelper::GetAttributeValue(ballElement, "image");
		if(parsedEntities.ContainsKey(name))
		{
			LogManager::WriteLine(LogType::Debug, "A ball with the name {0} already exists in the entities collection; skipping.", name);
		}

		//---------------------------------------------------------------------------------------------
		// load the child <frame> elements; this will also load the bounding box data
		//---------------------------------------------------------------------------------------------
		System::Collections::Generic::IEnumerable<XElement ^> ^frameElements = ballElement->Elements((String ^) "frame");
		if(XmlHelper::Count(frameElements) == 0)
		{
			// if there's no frame data, the game doesn't know what to draw.
			throw gcnew XmlException(String::Format("No frames are associated with the ball named {0}.", name));
		}

		List<Frame ^> ^frames = gcnew List<Frame ^>();
		for each(XElement ^frameElement in frameElements)
		{
			frames->Add(ParseFrame(frameElement));
		}

		Sprite ^ballSprite = gcnew Sprite(0, 0, frames->ToArray(), image);
		Ball ^ball = gcnew Ball(ballSprite, name);

		parsedEntities[name] = ball;
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
        Brick ^brick = gcnew Brick(brickSprite, hitCount, chanceOfPowerUp, points, name);

        parsedEntities[name] = brick;
    }
    catch(...)
    {
        throw;
    }
}

void EntityManager::ParseLaserPowerUp(XElement ^powerupElement, String ^name)
{
	try
	{
		String ^image = XmlHelper::GetAttributeValue(powerupElement, "image");
		unsigned int duration = XmlHelper::GetAttributeValueAsUInt32(powerupElement, "duration");
		int laserVelocity = XmlHelper::GetAttributeValueAsInt32(powerupElement, "laserVelocity");
		unsigned int laserDamage = XmlHelper::GetAttributeValueAsUInt32(powerupElement, "laserDamage");

		System::Collections::Generic::IEnumerable<XElement ^> ^frameQuery = powerupElement->Elements((String ^) "frame");
		if(0 == XmlHelper::Count(frameQuery))
		{
			throw gcnew XmlException(String::Format("powerup {0} doesn't have any frames associated with it.", name));
		}

		List<Frame ^> ^frames = gcnew List<Frame ^>();
		for each(XElement ^frameElement in frameQuery)
		{
			frames->Add(ParseFrame(frameElement));
		}

		Sprite ^powerupSprite = gcnew Sprite(0, 0, frames->ToArray(), image);
		LaserPowerUp ^laserPowerUp = gcnew LaserPowerUp(powerupSprite, duration, laserVelocity, laserDamage, name);

		parsedEntities[name] = laserPowerUp;
	}
	catch(...)
	{
		throw;
	}
}

void EntityManager::ParsePowerup(XElement ^powerupElement)
{
	try
	{
		String ^name = XmlHelper::GetAttributeValue(powerupElement, "name");
		String ^lowercasename = name->ToLower();

		if("laser_powerup" == lowercasename)
		{
			ParseLaserPowerUp(powerupElement, lowercasename);
			NumberOfPowerUps++;
		}
	}
	catch(...)
	{
		throw;
	}
}

void EntityManager::ParsePaddle(XElement ^paddleElement)
{
	try
	{
		String ^name = XmlHelper::GetAttributeValue(paddleElement, "name");
		String ^image = XmlHelper::GetAttributeValue(paddleElement, "image");
		if (parsedEntities.ContainsKey(name))
		{
			LogManager::WriteLine(LogType::Debug, "A paddle with the name {0} already exists in the entities collection; skipping.", name);
		}
		
		//---------------------------------------------------------------------------------------------
		// load the child <frame> elements; this will also load the bounding box data
		//---------------------------------------------------------------------------------------------
		System::Collections::Generic::IEnumerable<XElement ^> ^frameElements = paddleElement->Elements((String ^) "frame");
		if (XmlHelper::Count(frameElements) == 0)
		{
			// if there's no frame data, the game doesn't know what to draw.
			throw gcnew XmlException(String::Format("No frames are associated with the paddle named {0}.", name));
		}

		List<Frame ^> ^frames = gcnew List<Frame ^>();
		for each(XElement ^frameElement in frameElements)
		{
			frames->Add(ParseFrame(frameElement));
		}

		Sprite ^paddleSprite = gcnew Sprite(0, 0, frames->ToArray(), image);
		Paddle ^paddle = gcnew Paddle(paddleSprite, name);

		parsedEntities[name] = paddle;
	}
	catch(...)
	{
		throw;
	}
}

void EntityManager::ParsePowerUpAsset(XElement ^entityElement)
{
	try
	{
		String ^name = XmlHelper::GetAttributeValue(entityElement, "name");
		String ^image = XmlHelper::GetAttributeValue(entityElement, "image");

		//---------------------------------------------------------------------------------------------
		// load the child <frame> elements; this will also load the bounding box data
		//---------------------------------------------------------------------------------------------
		System::Collections::Generic::IEnumerable<XElement ^> ^frameElements = entityElement->Elements((String ^) "frame");
		if(XmlHelper::Count(frameElements) == 0)
		{
			// if there's no frame data, the game doesn't know what to draw.
			throw gcnew XmlException(String::Format("No frames are associated with the powerup asset named {0}.", name));
		}

		List<Frame ^> ^frames = gcnew List<Frame ^>();
		for each(XElement ^frameElement in frameElements)
		{
			frames->Add(ParseFrame(frameElement));
		}

		Sprite ^laserSprite = gcnew Sprite(0, 0, frames->ToArray(), image);
		Laser ^laser = gcnew Laser(laserSprite, name);

		parsedEntities[name] = laser;

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
			else if("paddle" == entityType)
			{
				ParsePaddle(entityElement);
			}
			else if("ball" == entityType)
			{
				ParseBall(entityElement);
			}
			else if("powerup" == entityType)
			{
				ParsePowerup(entityElement);
			}
			else if("powerup_asset" == entityType)
			{
				ParsePowerUpAsset(entityElement);
			}
        }
    }
    catch(...)
    {
        throw;
    }
}