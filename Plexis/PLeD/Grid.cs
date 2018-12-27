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
using System.Drawing;

namespace PLeD
{
    /// <summary>
    /// Represents the grid that can be used a guide when a drawing bricks.
    /// </summary>
    internal class Grid : IDisposable
    {
        // the width and height of each cell in the grid.
        // It (obviously) really represents the width and height of each brick.
        int cellWidth;
        int cellHeight;

        // the total width and height in pixels of the grids width and height
        int gridWidth;
        int gridHeight;

        Color gridColor;

        // used to draw the grid.
        Pen gridPen = new Pen(Color.Black);

        // true if the grid is visible and being rendered, otherwise false.
        bool visible;

        bool disposed = false;

        protected virtual void Dispose(bool disposing)
        {
            if(disposed)
            {
                return;
            }

            if(disposing && gridPen != null)
            {
                gridPen.Dispose();
            }

            disposed = true;
        }

        public void Dispose()
        {
            Dispose(true);

            GC.SuppressFinalize(this);
        }

        /// <summary>
        /// Draws the grid onto the specified graphics context. 
        /// </summary>
        /// <param name="renderTarget">the graphics context to draw the grid on to.</param>
        /// <exception cref="System.ArgumentNullException"><i>renderTarget</i> is <b>null</b>.</exception>
        public void Render(Graphics renderTarget)
        {
            // no point in rendering if the user can't see it.
            if(!this.visible)
            {
                return;
            }

            // horizontal lines
            for(int i = 0; i < this.gridHeight; i++)
            {
                renderTarget.DrawLine(this.gridPen, 0, i, this.gridWidth, i);
            }
        }

        /// <summary>
        /// Gets or sets the height of the cells within the grid.
        /// </summary>
        public int CellHeight
        {
            get
            {
                return this.cellHeight;
            }

            set
            {
                this.cellHeight = value;
            }
        }

        /// <summary>
        /// Gets or sets the width of the cells within the grid.
        /// </summary>
        public int CellWidth
        {
            get
            {
                return this.cellWidth;
            }
            set
            {
                this.cellWidth = value;
            }
        }

        /// <summary>
        /// Gets or sets the colour of the grid.
        /// </summary>
        public Color Colour
        {
            get
            {
                return this.gridColor;
            }
            set
            {
                this.gridColor = value;
            }
        }

        /// <summary>
        /// Gets or sets the total height of the grid in pixels.
        /// </summary>
        public int Height
        {
            get
            {
                return this.gridHeight;
            }
            set
            {
                this.gridHeight = value;
            }
        }

        /// <summary>
        /// Gets or sets the total width of the grid.
        /// </summary>
        public int Width
        {
            get
            {
                return this.gridWidth;
            }
            set
            {
                this.gridWidth = value;
            }
        }
        /// <summary>
        /// Gets or sets whether the grid is visible.
        /// </summary>
        public bool Visible
        {
            get
            {
                return this.visible;
            }
            set
            {
                this.visible = value;
            }
        }
    }
}
