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
