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
using System.Drawing;

namespace Level_Editor
{
    /// <summary>
    /// Represents the editors grid and its properties.
    /// </summary>
    class Grid
    {
        // represents whether the grid is visible to the user or not. 
        bool visible = false;

        Color gridColor = Color.Green;

        /// <summary>
        /// Gets or sets a value indicating whether the grid is visible or not. 
        /// </summary>
        /// <remarks>
        /// This property doesn't toggle rendering of the grid itself. It's meant to be a way of determining elsewhere
        /// in the program wether the grid is visible or not. 
        /// </remarks>
        public bool Visible
        {
            get { return visible; }
            set { visible = value; }
        }

        /// <summary>
        /// Gets or sets the colour of the grid in the form of a Color struct. 
        /// </summary>
        public Color Colour
        {
            get { return gridColor; }
            set { gridColor = value; }
        }

        /// <summary>
        /// Renders the grid to the specified surface.
        /// </summary>
        /// <param name="renderTo">The GDI+ surface to draw to.</param>
        public void Draw(Graphics renderTo)
        {
            if (!visible)
            {
                return;
            }

            using(Pen pen = new Pen(gridColor))
            {
                for (int i = 0; i < 480; i += 20)
                {
                    // draw horizontal lines
                    renderTo.DrawLine(pen, 0, i, 1024, i);
                }

                for (int i = 0; i < 1024; i += 41)
                {
                    // vertical lines
                    renderTo.DrawLine(pen, i, 0, i, 460);
                }

                // 1024/41 doesn't divide evenly, the right most cells 
                // are 40 pixels wide. It's not really noticeable in the game
                // but it also means the grid doesn't quite render as desired.
                // This ensures the right side of the cells are drawn. 
                renderTo.DrawLine(pen, 1023, 0, 1023, 460); 
            }
        }
    }
}
