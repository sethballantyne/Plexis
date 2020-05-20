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
#include "levelmanager.h"

using namespace System::Xml;

Level ^LevelManager::ReadLevel(String ^levelFile, bool testLevel)
{
    try
    {
        // <level width="30" height="20">
        //     <brick name="redBrick">
        //         <position x="3" y="3" /> 
        //         ...
        //     </brick>
        // </level>

		XElement ^levelXml;
		if (!testLevel)
		{
			levelXml = ResourceManager::GetXML(levelFile);
		}
		else
		{
			levelXml = XElement::Load(levelFile);
		}

        /*System::Collections::Generic::IEnumerable<XElement ^> ^levelQuery = levelXml->Elements((String ^)"level");
        if(0 == XmlHelper::Count(levelQuery))
        {
            throw gcnew XmlException(String::Format("level file {0} is missing its <level> tag.", levelFile));
        }*/

        // Get the width and height of the level in bricks.
        // In reality this will be the same for every level but it's implemented incase
        // different resolutions somehow make it in the game via magic truffle gnomes named Sexy Jenna-Knickers.
        //XElement ^levelElement = levelQuery->GetEnumerator()->Current;
        unsigned int levelBricksWide = XmlHelper::GetAttributeValueAsUInt32(levelXml, "width");
        unsigned int levelBricksHigh = XmlHelper::GetAttributeValueAsUInt32(levelXml, "height");

        Level ^newLevel = gcnew Level(levelBricksWide, levelBricksHigh);
		
        System::Collections::Generic::IEnumerable<XElement ^> ^brickQuery = levelXml->Elements((String ^)"brick");
        if(0 == XmlHelper::Count(brickQuery))
        {
			// returning nullptr instead of throwing an exception because I don't want the game to end just
			// because the level is devoid of bricks; better to ignore the level, which is what nullptr indicates.
            LogManager::WriteLine(LogType::Debug, "no <brick> tags found in the level {0}'s xml.", levelFile);
			return nullptr;
        }

        // read each different brick type within the level
        // you *can* specify the same brick in two different elements but why would you?
        for each(XElement ^brickElement in brickQuery)
        {
            String ^name = XmlHelper::GetAttributeValue(brickElement, "name");

            System::Collections::Generic::IEnumerable<XElement ^> ^brickPositionQuery = brickElement->Elements((String ^)"position");

            // read all the coordinates where this particular brick has to be placed.
            for each(XElement ^brickPosition in brickPositionQuery)
            {
                // tile coordinates, not screen coordinates.
                int x = XmlHelper::GetAttributeValueAsInt32(brickPosition, "x");
                int y = XmlHelper::GetAttributeValueAsInt32(brickPosition, "y");

                if(x < 0 || x >= levelBricksWide)
                {
                    throw gcnew XmlException(String::Format("X coordinate {0} is out of bounds for the level {1}.", x, levelFile));
                }
                
                if(y < 0 || y >= levelBricksHigh)
                {
                    throw gcnew XmlException(String::Format("Y coordinate {0} is out of bounds for the level {1}.", y, levelFile));
                }
                
                // put a new instance of this brick at the specified coordinates.
                newLevel->PutBrickAt(x, y, name);

				Brick ^brick = EntityManager::GetEntity<Brick^>(name);
				if(brick->Tally)
				{
					newLevel->BrickCount++;
				}
            }
        }

        return newLevel;
    }
    catch(...)
    {
        throw;
    }
}

void LevelManager::Initialise(XElement ^xmlLevelList)
{
    try
    {
        System::Collections::Generic::IEnumerable<XElement ^> ^levelsQuery = xmlLevelList->Elements((String ^)"level");
        if(0 == XmlHelper::Count(levelsQuery))
        {
            throw gcnew XmlException(String::Format("no <level> tags present in {0}.xml", xmlLevelList));
        }

        List<String ^> ^temp = gcnew List<String ^>();
        for each(XElement ^levelElement in levelsQuery)
        {
            if(nullptr != levelElement->Value)
            {
                temp->Add(levelElement->Value);
            }
        }

        levels = temp->ToArray();

        // when GetNextLevel() is called, currentLevel will be set to 0
        // and the first level will be loaded.
        currentLevel = levels->Length;
    }
    catch(...)
    {
        throw;
    }
}