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

            string tempFilename;
            string tempID;
            foreach (var node in xmlBrickData)
            {
                tempFilename = (string)node.Attribute("filename");
                tempID = (string)node.Attribute("id");

                List<string> tempAttributeList = new List<string>();
                foreach (var attribute in node.Elements("attribute"))
                {
                    string attributeLabel = (string)attribute.Attribute("label");
                    string attributeValue = (string)attribute.Attribute("value");

                    tempAttributeList.Add(String.Format("{0}: {1}", attributeLabel, attributeValue));
                }

                string tempDescription = (string)node.Element("description");

                parsedBrickData.Add(new BrickData(tempFilename, tempID, tempAttributeList.ToArray(), tempDescription));

                tempAttributeList.Clear();
            }

            return parsedBrickData.ToArray();
        }
    }
}
