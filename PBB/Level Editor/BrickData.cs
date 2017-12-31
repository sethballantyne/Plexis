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
