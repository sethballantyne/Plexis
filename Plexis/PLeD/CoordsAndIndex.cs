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

namespace PLeD
{
    /// <summary>
    /// Used by editor to keep tracking of which bricks have been painted over and which have been painted and where.
    /// A single instance represents a single brick.
    /// </summary>
    struct CoordsAndIndex
    {
        /// <summary>
        /// The X coordinate of the brick, in pixels.
        /// </summary>
        public int X;

        /// <summary>
        /// the Y coordinate of the brick, in pixels.
        /// </summary>
        public int Y;

        /// <summary>
        /// -1 if it represents a blank space, otherwise the selected index value of the brick in the list view.
        /// </summary>
        public int Index;

        /// <summary>
        /// Initialises a new instance with the specified screen coordinates and index.
        /// </summary>
        /// <param name="x">The X screen coordinate.</param>
        /// <param name="y">The Y screen coordainte.</param>
        /// <param name="index">1 if it represents a blank space, otherwise the selected index value of the brick in the list view.</param>
        public CoordsAndIndex(int x, int y, int index)
        {
            this.X = x;
            this.Y = y;
            this.Index = index;
        }
    }
}
