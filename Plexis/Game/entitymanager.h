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
#include "brick.h"
#include "xmlhelper.h"

/// <summary>
/// 
/// </summary>
public ref class EntityManager abstract sealed
{
private:

    static System::Collections::Generic::Dictionary<String ^, Entity ^> parsedEntities;

    /// <summary>
    /// 
    /// </summary>
    /// <param name="frameElement"></param>
    /// <returns></returns>
    static Frame ^ParseFrame(XElement ^frameElement);
   
    /// <summary>
    /// 
    /// </summary>
    /// <param name="brickElement"></param>
    /// <returns></returns>
    static void ParseBrick(XElement ^brickElement);

public:
    /// <summary>
    /// 
    /// </summary>
    /// <param name="entitiesFile"></param>
    static void Initialise(XElement ^entitiesFile);

    /// <summary>
    /// 
    /// </summary>
    /// <param name="name"></param>
    /// <returns></returns>
    generic <class T> where T : ref class T static GetEntity(String ^name)
    {
        try
        {
            return safe_cast<T>(parsedEntities[name]->Clone());
        }
        catch(...)
        {
            // returning nullptr if it doesn't exist
            return T();
        }
    }
};
