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
    /// <summary>
    /// Represents a single parsed &lt;brick&gt; element in assets.xml and its associated &lt;attribute&gt; child element(s). 
    /// </summary>
    public class BrickData
    {
        string description;
        string filename;
        string label;
        string id;
        string[] attributes;

        /// <summary>
        /// Creates a new instance of BrickData using the parsed settings from assets.xml
        /// </summary>
        /// <param name="filename">The sprites filename, including extension.</param>
        /// <param name="paletteLabel">The text to use as the images caption when copied to the ListView on the main form.</param>
        /// <param name="id"></param>
        /// <param name="attributes">Array of parsed &lt;attribute&gt;'s belonging to the brick.</param>
        /// <param name="description">The bricks tooltip text when it's made in to a ListView item.</param>
        /// <exception cref="System.ArgumentNullException">The specified variable is either null or an empty string.</exception>
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

        /// <summary>
        /// Gets or sets the filename of the bricks sprite. 
        /// </summary>
        public string Filename
        {
            get { return filename; }
            set { filename = value; }
        }

        /// <summary>
        /// Gets or sets the label to be used by the ListView when the brick is converted into a ListViewItem.
        /// </summary>
        public string Label
        {
            get { return label; }
            set { label = value; }
        }

        /// <summary>
        /// 
        /// </summary>
        public string ID
        {
            get { return id; }
            set { id = value; }
        }

        /// <summary>
        /// Gets or sets the text to be used as the items tooltip when it's made into a ListViewItem. 
        /// </summary>
        public string Description
        {
            get { return description; }
            set { description = value; }
        }

        /// <summary>
        /// Returns an array of the bricks attributes.
        /// </summary>
        /// <returns></returns>
        public string[] GetAttributes()
        {
            return attributes;
        }
    }
}
