/*Copyright (c) Seth Ballantyne <seth.ballantyne@gmail.com>
*
*Permission is hereby granted, free of charge, to any person obtaining a copy
*of this software and associated documentation files (the "Software"), to deal
*in the Software without restriction, including without limitation the rights
*to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*copies of the Software, and to permit persons to whom the Software is
*furnished to do so, subject to the following conditions:
*
*The above copyright notice and this permission notice shall be included in
*all copies or substantial portions of the Software.
*
*THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
*THE SOFTWARE.
*/

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml;
using System.Xml.Linq;

namespace Level_Editor
{
    static class XMLParser
    {
        public static BrickData[] Load(string filename)
        {
            List<BrickData> parsedBrickData = new List<BrickData>();

            XElement xmlFile = XElement.Load("assets.xml");
            IEnumerable<XElement> xmlBrickData = from q in xmlFile.Elements("brick") select q;

            //string tempFilename;
            //string tempID;
            foreach (var node in xmlBrickData)
            {
                string tempFilename = (string)node.Attribute("filename");
                string tempID = (string)node.Attribute("id");
                string tempPaletteLabel = (string)node.Attribute("label");

                List<string> tempAttributeList = new List<string>();
                foreach (var attribute in node.Elements("attribute"))
                {
                    string attributeName = (string) attribute.Attribute("name");
                    string attributeLabel = (string)attribute.Attribute("label");
                    string attributeValue = (string)attribute.Attribute("value");

                    string format;
                    if (attributeName == "powerupSpawnChance")
                    {
                        format = "{0}: {1}%";
                    }
                    else
                    {
                        format = "{0}: {1}";
                    }
                    tempAttributeList.Add(String.Format(format, attributeLabel, attributeValue));
                }

                string tempDescription = (string)node.Element("description");

                parsedBrickData.Add(new BrickData(tempFilename, tempPaletteLabel, tempID, 
                    tempAttributeList.ToArray(), tempDescription));

                tempAttributeList.Clear();
            }

            return parsedBrickData.ToArray();
        }
    }
}
