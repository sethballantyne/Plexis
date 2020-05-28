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
/// Acts a pool for each entity parsed from entities.xml. 
/// Calling GetEntity() performs a deep clone of the specified entity for use during gameplay.
/// </summary>
public ref class EntityManager abstract sealed
{
private:

    static System::Collections::Generic::Dictionary<String ^, Entity ^> parsedEntities;

    /// <summary>
    /// Parses a &lt;frame&gt; element.
    /// </summary>
    /// <param name="frameElement">The element to parse.</param>
    /// <returns>An instance of <see cref="Frame"/> based on the elements parsed attributes.</returns>
    /// <exception cref="System::FormatException">one of the elements attributes contains an invalid value.</exception>
    /// <exception cref="System::OverflowException">an attribute that contains a numeric value is outside its range.</exception>
    /// <exception cref="System::Xml::XmlException">A required attribute is missing from the element, or the value of the element evaluates to String::Empty.</exception>
    static Frame ^ParseFrame(XElement ^frameElement);
   
	/// <summary>
	/// Parses all the &lt;frame&gt; elements for a specified &lt;entity&gt; element.
	/// </summary>
	static array<Frame ^, 1>^ ParseFrames(XElement ^element);

	/// <summary>
	/// Parses an &lt;entity&gt; element whose <c>name</c> attribute is <i>ball</i>.
	/// </summary>
	/// <param name="ballElement">The element to parse.</param>
	/// <exception cref="System::Xml::XmlException">A required attribute is missing from the element, or the value of the element evaluates to String::Empty.</exception>
	/// <exception cref="System::FormatException">one of the elements attributes contains an invalid value.</exception>
	/// <exception cref="System::OverflowException">an attribute that contains a numeric value is outside its range.</exception>
	static void ParseBall(XElement ^ballElement);

    /// <summary>
    /// Parses an &lt;entity&gt; element whose <c>name</c> attribute is <i>brick</i>.
    /// </summary>
    /// <param name="brickElement">The element to parse.</param>
    /// <exception cref="System::FormatException">one of the elements attributes contains an invalid value.</exception>
    /// <exception cref="System::OverflowException">an attribute that contains a numeric value is outside its range.</exception>
    /// <exception cref="System::Xml::XmlException">A required attribute is missing from the element, or the value of the element evaluates to String::Empty.</exception>
    static void ParseBrick(XElement ^brickElement);

	/// <summary>
	/// Parses an &lt;entity&gt; element whose <c>name</c> attribute is <i>paddle</i>.
	/// </summary>
	/// <param name="paddleElement">the element to parse.</param>
	/// <exception cref="System::FormatException">one of the elements attributes contains an invalid value.</exception>
	/// <exception cref="System::OverflowException">an attribute that contains a numeric value is outside its range.</exception>
	/// <exception cref="System::Xml::XmlException">A required attribute is missing from the element, or the value of the element evaluates to String::Empty.</exception>
	static void ParsePaddle(XElement ^paddleElement);

	/// <summary>
	/// Parses an entity element who's name attribute is <i>powerup_asset</i>.
	/// Powerup assets are assets used by a powerup when it's in effect (lasers, etc).
	/// </summary>
	/// <exception cref="System::FormatException">one of the elements attributes contains an invalid value.</exception>
	/// <exception cref="System::OverflowException">an attribute that contains a numeric value is outside its range.</exception>
	/// <exception cref="System::Xml::XmlException">A required attribute is missing from the element, or the value of the element evaluates to String::Empty.</exception>
	static void ParsePowerUpAsset(XElement ^assetElement);

	/// <summary>
	/// Parses the XML that describes laser power up.
	/// </summary>
	/// <exception cref="System::FormatException">one of the elements attributes contains an invalid value.</exception>
	/// <exception cref="System::OverflowException">an attribute that contains a numeric value is outside its range.</exception>
	/// <exception cref="System::Xml::XmlException">A required attribute is missing from the element, or the value of the element evaluates to String::Empty.</exception>
	static void ParseLaserPowerUp(XElement ^powerupElement, String ^name);

	static void ParsePowerUp(XElement ^powerupElement, String^ name);

	/// <summary>
	/// Parses all the powerups within the entities file.
	/// </summary>
	/// <exception cref="System::FormatException">one of the elements attributes contains an invalid value.</exception>
	/// <exception cref="System::OverflowException">an attribute that contains a numeric value is outside its range.</exception>
	/// <exception cref="System::Xml::XmlException">A required attribute is missing from the element, or the value of the element evaluates to String::Empty.</exception>
	static void ReadPowerup(XElement ^brickElement);

	/// <summary>
	/// Parses the XML that describes instadeath power up that isn't really a power up, I just didn't know what to call it. 
	/// Fuck-me-up? I dunno. "the instadeath fuck-me-up". I kinda like that.
	/// I'm talking to myself in the documentation tags. Awesome. 
	/// </summary>
	/// <exception cref="System::FormatException">one of the elements attributes contains an invalid value.</exception>
	/// <exception cref="System::OverflowException">an attribute that contains a numeric value is outside its range.</exception>
	/// <exception cref="System::Xml::XmlException">A required attribute is missing from the element, or the value of the element evaluates to String::Empty.</exception>
	static void ParseInstaDeathPowerUp(XElement ^powerupElement, String ^name);

	/// <summary>
	/// Parses the XML that describes the power ups that give bonus points.
	/// </summary>
	/// <exception cref="System::FormatException">one of the elements attributes contains an invalid value.</exception>
	/// <exception cref="System::OverflowException">an attribute that contains a numeric value is outside its range.</exception>
	/// <exception cref="System::Xml::XmlException">A required attribute is missing from the element, or the value of the element evaluates to String::Empty.</exception>
	static void ParseBonusPointsPowerUp(XElement ^powerupElement, String ^lowercasename);

	/// <summary>
	/// Parses the XML that describes bonus life power up.
	/// </summary>
	/// <exception cref="System::FormatException">one of the elements attributes contains an invalid value.</exception>
	/// <exception cref="System::OverflowException">an attribute that contains a numeric value is outside its range.</exception>
	/// <exception cref="System::Xml::XmlException">A required attribute is missing from the element, or the value of the element evaluates to String::Empty.</exception>
	static void ParseExtraLifePowerUp(XElement ^powerupElement, String ^name);

	/// <summary>
	/// Parses the XML that describes a timed powerup.
	///</summary>
	/// <exception cref="System::FormatException">one of the elements attributes contains an invalid value.</exception>
	/// <exception cref="System::OverflowException">an attribute that contains a numeric value is outside its range.</exception>
	/// <exception cref="System::Xml::XmlException">A required attribute is missing from the element, or the value of the element evaluates to String::Empty.</exception>
	static void ParseTimedPowerUp(XElement^ powerupElement, String^ name);

	static void ParseFireBallPowerUp(XElement^ powerupElement, String^ name);

	static void ParseExtraBallPowerUp(XElement^ powerupElement, String^ name);

public:
	/// <summary>
	/// The number of different powerups within the game.
	/// </summary>
	static property unsigned int NumberOfPowerUps;

    /// <summary>
    /// Parses the XML containing entity information, creating instances of each entity which can 
    /// then be requested during gameplay.
    /// </summary>
    /// <param name="entitiesFile">The XML contents of the file containing entity information.</param>
    /// <exception cref="System::FormatException">An attribute belonging to an element contains an invalid value.</exception>
    /// <exception cref="System::OverflowException">an attribute that contains a numeric value is outside its range.</exception>
    /// <exception cref="System::Xml::XmlException">A required attribute is missing from an element, or the value of the attribute evaluates to String::Empty.</exception>
    static void Initialise(XElement ^entitiesFile);

    /// <summary>
    /// Returns the specified entity as a deep clone of the copy stored in the entity pool.
    /// </summary>
    /// <param name="name">The name of the entity to return.</param>
    /// <returns>A deep cloned instance of the entity with the specified name, 
    /// or <b>nullptr</b> if the desired entity doesn't exist within the entity pool.</returns>
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

	/// <summary>
	/// Returns a shallow copy of the specified entity, or <b>nullptr</b> if the 
	/// entity doesn't exist.
	///</summary>
	generic <class T> where T : ref class T static GetShallowEntity(String ^name)
	{
		try
		{
			return safe_cast<T>(parsedEntities[name]);
		}
		catch(...)
		{
			// returning nullptr if it doesn't exist
			return T();
		}
	}
};
