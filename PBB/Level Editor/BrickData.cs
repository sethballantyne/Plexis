using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Level_Editor
{
    public class BrickData
    {
        string description;
        string filename;
        string label;
        string id;
        string[] attributes;

        public BrickData(string filename, string paletteLabel, string id, string[] attributes, string description)
        {
            if (String.IsNullOrEmpty(filename))
            {
                throw new ArgumentNullException("filename");
            }
            else if (String.IsNullOrEmpty(id))
            {
                throw new ArgumentNullException("id");
            }
            else if (String.IsNullOrEmpty(paletteLabel))
            {
                throw new ArgumentNullException("paletteLabel");
            }
            else if (attributes == null)
            {
                throw new ArgumentNullException("attributes");
            }
            else if (String.IsNullOrEmpty(description))
            {
                throw new ArgumentNullException("description");
            }

            this.filename = filename;
            this.label = paletteLabel;
            this.id = id;
            this.description = description;
            this.attributes = attributes;
        }

        public string Filename
        {
            get { return filename; }
            set { filename = value; }
        }

        public string Label
        {
            get { return label; }
            set { label = value; }
        }

        public string ID
        {
            get { return id; }
            set { id = value; }
        }

        public string Description
        {
            get { return description; }
            set { description = value; }
        }

        public string[] GetAttributes()
        {
            return attributes;
        }
    }
}
