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

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime;
using System.Xml;
using System.Xml.Linq;
using System.Drawing;

namespace PLeD
{
    /// <summary>
    /// Handles all the XML processing required by the editor. If it involves reading/writing
    /// XML, it's here.
    /// </summary>
    internal static class XML
    {
        /// <summary>
        /// 
        /// </summary>
        /// <param name="element"></param>
        /// <param name="attribute"></param>
        /// <returns></returns>
        private static string GetAttributeValue(XElement element, string attribute)
        {
            if(element == null)
            {
                throw new ArgumentNullException("element");
            }
            else if(attribute == null)
            {
                throw new ArgumentNullException("attribute");
            }

            if(attribute == String.Empty)
            {
                throw new ArgumentException("attribute evaluates to String::Empty.");
            }

            XAttribute xAttribute = element.Attribute(attribute);
            if(xAttribute == null)
            {
                throw new XmlException(String.Format("the {0} attribute is missing.", attribute));
            }
    
            if(xAttribute.Value == String.Empty)
            {
                throw new XmlException(String.Format("the value of the {0} attribute evaluates to String::Empty."));
            }
            else
            {
                return xAttribute.Value;
            }
        }

        private static int GetAttributeValueAsInt32(XElement element, string attribute)
        {
            try
            {
                string initialValue = GetAttributeValue(element, attribute);
                int number = Convert.ToInt32(initialValue);

                return number;
            }
            catch(FormatException)
            {
                throw new FormatException(String.Format("the {0} attribute contains a non-numeric value.", attribute));
            }
            catch(OverflowException)
            {
                throw new OverflowException(String.Format("the {0} attribute contains a number that's less than {1} or greater than {2}", attribute, Int32.MinValue, Int32.MaxValue));
            }
            catch
            {
                throw;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="element"></param>
        /// <param name="attribute"></param>
        /// <returns></returns>
        private static uint GetAttributeValueAsUInt32(XElement element, String attribute)
        {
            try
            {
                string initialValue = GetAttributeValue(element, attribute);
                return Convert.ToUInt32(initialValue);
            }
            catch(FormatException)
            {
                throw new FormatException(String.Format("the {0} attribute contains a non-numeric value.", attribute));
            }
            catch(OverflowException)
            {
                throw new OverflowException(String.Format("the {0} attribute contains a value less than {1} and greater than {2}", attribute, UInt32.MinValue, UInt32.MaxValue));
            }
            catch
            {
                throw;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="element"></param>
        /// <param name="attribute"></param>
        /// <returns></returns>
        private static bool GetAttributeValueAsBoolean(XElement element, string attribute)
        {
            try
            {
                string temp = GetAttributeValue(element, attribute);
                return Convert.ToBoolean(temp);
            }
            catch
            {
                throw;
            }
        }

        /// <summary>
        /// Reads the specified XML and extracts the locations being used to house the
        /// specified resource type.
        /// </summary>
        /// <param name="type">the resource to search for within the XML.</param>
        /// <param name="elements">The XML to search.</param>
        /// <returns>An array of the locations being used to store the specified resource type.
        /// Each element will contain a member boolean specifying whether subdirectories within
        /// the specified path also contain the specified type of resource.</returns>
        /// <exception cref="System.Xml.XmlException">The specified XML is missing an <i>includeSubDirectories</i> attribute
        /// or the value of the attribute evaluates to String.Empty.</exception>
        /// <exception cref="System.FormatException">The value of the <i>includeSubDirectories</i> element 
        /// is not a boolean value, nor is it the strings <i>true</i> or <i>false</i>.</exception>
        private static Path[] LoadPaths(string type, IEnumerable<XElement> elements)
        {
            List<Path> pathList = new List<Path>();

            try
            {
                foreach (var element in elements)
                {
                    Path resourcePath = new Path();
                    resourcePath.IncludeSubDirectories = GetAttributeValueAsBoolean(element, "includeSubDirectories");
                    resourcePath.ResourcePath = element.Value;
                    pathList.Add(resourcePath);
                }

                return pathList.ToArray();
            }
            catch
            {
                throw;
            }

            
        }

        /// <summary>
        /// Loads the locations of the XML and Bitmap resources from the specified file.
        /// </summary>
        /// <param name="filename">the path of the XML file containing path information.</param>
        /// <returns>The locations of the bitmap and XML resources as a <i>Path</i> object.</returns>
        /// <remarks>in practice it's going to load from paths.xml, but it's been kept generic
        /// as possible incase this ever changes.</remarks>
        /// <exception cref="System.ArgumentNullException"><i>path</i> evaluates to <b>null</b>.</exception>
        /// <exception cref="System.ArgumentExeption"><i>path</i> evaluates to String.Empty.</exception>
        /// <exception cref="System.FileNotFoundException">the file specified in <i>path</i> does not exist.</exception>
        /// <exception cref="System.XmlException">The specified XML file is missing data or is incorrectly formatted.</exception>
        /// <exception cref="System.FormatException">the <c>includeSubDirectories</c> contains an invalid (non-boolean) value.</exception>
        public static Paths LoadPaths(string path)
        {
            if(path == null)
            {
                throw new ArgumentNullException("path");
            }
            else if(path == String.Empty)
            {
                throw new ArgumentException("path evaluates to String.Empty.");
            }

            try
            {
                List<Path> xmlPaths = new List<Path>();
                List<Path> bitmapPaths = new List<Path>();

                XElement xmlFile = XElement.Load(path);

                IEnumerable<XElement> pathData = xmlFile.Descendants();
                foreach (XElement node in pathData)
                {
                    // read the type attribute of each <resource> element.
                    XAttribute typeAttribute = node.Attribute("type");
                    if (typeAttribute != null && (typeAttribute.Value == "xml" || typeAttribute.Value == "bitmaps"))
                    {
                        IEnumerable<XElement> desiredPathData = node.Descendants();

                        Path[] paths;

                        // if we're parsing a xml or bitmap resource element, read the 
                        // child elements. 
                        // <path includeSubDirectories="false">/data</path> etc 
                        if (typeAttribute.Value == "xml")
                        {
                            paths = LoadPaths("xml", desiredPathData);
                            if (paths != null)
                            {
                                xmlPaths.AddRange(paths);
                            }
                        }
                        else
                        {
                            paths = LoadPaths("bitmaps", desiredPathData);
                            if (paths != null)
                            {
                                bitmapPaths.AddRange(paths);
                            }
                        }
                    }
                }

                Paths parsedPaths = new Paths();
                parsedPaths.XMLPaths = xmlPaths.ToArray();
                parsedPaths.BitmapPaths = bitmapPaths.ToArray();
                return parsedPaths;
            }
            catch
            {
                throw;
            }
            
        }

        /// <summary>
        /// Reads the required brick information from the specified XML file.
        /// </summary>
        /// <param name="path">location of the XML file containing entity information.</param>
        /// <returns>An array of Brick objects containing the names, filenames and sprite coords of each brick.</returns>
        /// <remarks>Like XML.ReadPaths(), this has been kept generic as possible though in reality
        /// you'll be reading from entities.xml.</remarks>
        /// <exception cref="System.ArgumentNullException"><i>path</i> evaluates to <b>null</b>.</exception>
        /// <exception cref="System.ArgumentException"><i>path</i> evaluates to String.Empty.</exception>
        /// <exception cref="System.FileNotFoundException">the location specified in <i>path</i> does not exist.</exception>
        /// <exception cref="System.XmlException">The specified XML file is missing data or is incorrectly formatted.</exception>
        /// <exception cref="System.FormatException">A required attribute in the XML was supposed to contain a numeric value but doesn't.</exception>
        public static Brick[] ReadBrickData(string path)
        {
            if(path == null)
            {
                throw new ArgumentNullException("path");
            }
            else if(path == String.Empty)
            {
                throw new ArgumentException("path evaluates to String.Empty");
            }

            try
            {
                XElement entitiesFile = XElement.Load(path);
                IEnumerable<XElement> entitiesData = entitiesFile.Descendants();

                List<Brick> bricks = new List<Brick>();
                foreach (XElement element in entitiesData)
                {
                    XAttribute typeAttribute = element.Attribute("type");
                    if (typeAttribute != null && typeAttribute.Value == "brick")
                    {
                        Brick parsedBrick = new Brick();

                        parsedBrick.Name = GetAttributeValue(element, "name");
                        parsedBrick.ImageName = GetAttributeValue(element, "image");

                        XElement firstFrameElement = element.Element("frame");
                        if (firstFrameElement != null)
                        {
                            parsedBrick.FrameX = GetAttributeValueAsInt32(firstFrameElement, "x");
                            parsedBrick.FrameY = GetAttributeValueAsInt32(firstFrameElement, "y");
                            parsedBrick.FrameWidth = GetAttributeValueAsInt32(firstFrameElement, "width");
                            parsedBrick.FrameHeight = GetAttributeValueAsInt32(firstFrameElement, "height");

                            bricks.Add(parsedBrick);
                        }
                        else
                        {
                            return null;
                        }
                    }
                }

                return bricks.ToArray();
            }
            catch
            {
                throw;
            }
        }

        /// <summary>
        /// Reads the specified level and returns it as a Level object.
        /// </summary>
        /// <param name="path">the path of the desired level.</param>
        /// <param name="bricks">the brick data parsed from entities.xml.</param>
        /// <returns>the level formatted as a level object. Duh.</returns>
        /// <exception cref="System.ArgumentNullException"><i>path</i> or <i>bricks</i> evaluate to <b>null</b>.</exception>
        /// <exception cref="System.ArgumentException"><i>path</i> evaluates to String.Empty.</exception>
        /// <exception cref="System.IO.FileNotFoundException">the location specified in <i>path</i> does not exist.</exception>
        /// <exception cref="System.FormatException">A required attribute in the XML was supposed to contain a numeric value but doesn't.</exception>
        /// <exception cref="System.Xml.XmlException">The specified XML file is missing data or is incorrectly formatted.</exception>
        public static Level ReadLevel(string path, Brick[] bricks)
        {
            if(path == null)
            {
                throw new ArgumentNullException("path");
            }
            else if (bricks == null)
            {
                throw new ArgumentNullException("bricks");
            }
            else if (path == String.Empty)
            {
                throw new ArgumentException("path evaluates to String.Empty.");
            }

            try
            {
                XElement levelFile = XElement.Load(path);

                // reading the height and width attributes of the level element.
                uint width = GetAttributeValueAsUInt32(levelFile, "width");
                uint height = GetAttributeValueAsUInt32(levelFile, "height");

                Level level = new Level(width, height);

                IEnumerable<XElement> brickNodes = levelFile.Descendants("brick");
                foreach (XElement brickElement in brickNodes)
                {
                    string brickName = GetAttributeValue(brickElement, "name");

                    int index = Array.FindIndex(bricks, w => w.Name == brickName);
                    if (index != -1)
                    {
                        IEnumerable<XElement> brickPositions = brickElement.Elements("position");
                        foreach (XElement position in brickPositions)
                        {
                            int x = GetAttributeValueAsInt32(position, "x");
                            int y = GetAttributeValueAsInt32(position, "y");

                            level[x, y] = index;
                        }
                    }
                    else
                    {
                        // the specified brick doesn't exist. 
                        throw new XmlException(String.Format("The level wants to use a brick with the name {0} but I have no idea what the hell that is.", brickName));
                    }
                }
                return level;
            }
            catch
            {
                throw;
            }
            
        }

        /// <summary>
        /// Writes a level to disk.
        /// </summary>
        /// <param name="path">the destination path for the level, including filename.</param>
        /// <param name="level">the level to write to XML.</param>
        /// <param name="bricks">the brick data parsed from entities.xml at startup.</param>
        /// <exception cref="System.ArgumentNullException"><i>path</i>, <i>level</i>, or <i>bricks</i> evaluate to <b>null</b>.</exception>
        /// <exception cref="System.ArgumentException"><i>path</i> evaluates to String.Empty.</exception>
        internal static void WriteLevel(string path, Level level, Brick[] bricks)
        {
            if(path == null)
            {
                throw new ArgumentNullException("path");
            }
            else if(level == null)
            {
                throw new ArgumentNullException("level");
            }
            else if(bricks == null)
            {
                throw new ArgumentNullException("bricks");
            }
            else if(path == String.Empty)
            {
                throw new ArgumentException("path evaluates to String.Empty.");
            }

            XmlWriterSettings xmlWriterSettings = new XmlWriterSettings();
            xmlWriterSettings.Indent = true;
            xmlWriterSettings.CloseOutput = true;
            
            try
            {
                using(XmlWriter xmlWriter = XmlWriter.Create(path, xmlWriterSettings))
                {
                    // <level width="xx" height="xx">
                    xmlWriter.WriteStartElement("level");
                    xmlWriter.WriteAttributeString("width", Convert.ToString(level.Width));
                    xmlWriter.WriteAttributeString("height", Convert.ToString(level.Height));

                    Dictionary<string, List<Point>> formattedLevelData = level.BuildDictionary(bricks);
                    foreach(KeyValuePair<string, List<Point>> kvp in formattedLevelData)
                    {
                        if(kvp.Value.Count > 0)
                        {
                            // <brick name="foo">
                            xmlWriter.WriteStartElement("brick");
                            xmlWriter.WriteAttributeString("name", kvp.Key);

                            for(int i = 0; i < kvp.Value.Count; i++)
                            {
                                // <position x="666" y="999">
                                xmlWriter.WriteStartElement("position");
                                xmlWriter.WriteAttributeString("x", Convert.ToString(kvp.Value[i].X));
                                xmlWriter.WriteAttributeString("y", Convert.ToString(kvp.Value[i].Y));
                                xmlWriter.WriteEndElement();
                            }
                        }

                        // </brick>
                        xmlWriter.WriteEndElement();
                    }

                    //</level>
                    xmlWriter.WriteEndElement();
                    xmlWriter.Close();
                }
            }
            catch
            {
                throw;
            }

        }
    }
}
