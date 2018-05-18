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

using namespace System;
using namespace System::Collections::Generic;

/// <summary>
/// GameOptions is queried by the other classes whenever they need information
/// on one of the games settings. Its job is to load the settings in from disk, 
/// make them available to any other class that needs them and write them back out
/// to disk when they're updated.
/// </summary>
public ref class GameOptions abstract sealed
{
private:
    static Dictionary<String ^, String ^> ^gameOptions;
    static String ^path;

public:
    static GameOptions()
    {
        gameOptions = gcnew Dictionary<String ^, String ^>();
    }

    /// <summary>
    /// Reads the game settings from the specified file.
    /// </summary>
    /// <param name="path">the XML file that contains the games settings.</param>
    /// <exception cref="System::ArgumentException"><i>path</i> evaluates to String::Empty.</exception>
    /// <exception cref="System::ArgumentNullException"><i>path</i> is <b>null</b>.</exception>
    /// <exception cref="System::IO::FileNotFoundException">the file specified in <i>path</i> doesn't exist.</exception>
    /// <exception cref="System::Security::SecurityException">the application doesn't have the correct permissions to acccess the file specified in <i>path</i>.</exception>
    /// <exception cref="System::Xml::XmlException">an error occured while parsing the XML file.</exception>
    static void Initialise(String ^path);

    /// <summary>
    /// Gets the value of a specified option as a string.
    /// </summary>
    /// <param name="id">the option whose value should be returned.</param>
    /// <param name="default">the value to return if the specified option doesn't exist.</param>
    /// <returns>the value associated with the setting <i>id</i>. If the specified option
    /// doesn't exist, <i>default</i> is returned.</returns>
    static String ^GetValue(String ^id, String ^default)
    {
        String ^value;

        try
        {
            value = GameOptions::gameOptions[id];
        }
        catch(...)
        {
            value = default;
        }

        return value;
    }

    /// <summary>
    /// Gets the value of a specified option as an Int32 value.
    /// </summary>
    /// <param name="id">the option whose value should be returned.</param>
    /// <param name="default">the value to return if the specified option doesn't exist, or it can't be converted to an Int32 value.</param>
    /// <returns>the value associated with the setting <i>id</i> as an Int32. If the specified option
    /// doesn't exist or the associated value coulnd't be converted to a Int32, <i>default</i> is returned.</returns>
    static int GetValue(String ^id, int default)
    {
        int value;
        try
        {
            value = Convert::ToInt32(GameOptions::gameOptions[id]);
        }
        catch(...)
        {
            value = default;
        }

        return value;
    }

    /// <summary>
    /// Gets the value of a specified option as a double.
    /// </summary>
    /// <param name="id">the option whose value should be returned.</param>
    /// <param name="default">the value to return if the specified option doesn't exist, or it can't be converted to a double.</param>
    /// <returns>the value associated with the setting <i>id</i> as a double. If the specified option
    /// doesn't exist or the associated value coulnd't be converted to a double, <i>default</i> is returned.</returns>
    static double GetValue(String ^id, double default)
    {
        double value;
        try
        {
            value = Convert::ToDouble(GameOptions::gameOptions[id]);
        }
        catch(...)
        {
            value = default;
        }

        return value;
    }

    /// <summary>
    /// Writes the game settings back out to the XML file the GameOptions was initialised with.
    /// </summary>
    /// <exception cref="System::UnauthorizedAccessException">the application doesn't have permission to write to the XML file.</exception>
    static void Save();

    /// <summary>
    /// Sets the specified the value of the specified setting.
    /// </summary>
    /// <param name="id">the setting to update.</param>
    /// <param name="value">the value of the setting.</param>
    /// <remarks>if the setting doesn't currently exist, it'll be created with the value of <i>value</i>.</remarks>
    static void SetValue(String ^id, String ^value)
    {
        // key and value is added to the dictionary if id doesn't exist.
        GameOptions::gameOptions[id] = value;
    }

    /// <summary>
    /// Sets the specified the value of the specified setting.
    /// </summary>
    /// <param name="id">the setting to update.</param>
    /// <param name="value">the value of the setting.</param>
    /// <remarks>if the setting doesn't currently exist, it'll be created with the value of <i>value</i>.</remarks>
    static void SetValue(String ^id, int value)
    {
        GameOptions::gameOptions[id] = Convert::ToString(value);
    }

    /// <summary>
    /// Sets the specified the value of the specified setting.
    /// </summary>
    /// <param name="id">the setting to update.</param>
    /// <param name="value">the value of the setting.</param>
    /// <remarks>if the setting doesn't currently exist, it'll be created with the value of <i>value</i>.</remarks>
    static void SetValue(String ^id, double value)
    {
        GameOptions::gameOptions[id] = Convert::ToString(value);
    }
};