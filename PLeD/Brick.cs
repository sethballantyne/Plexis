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
    /// Represents a brick parsed from entities.xml.
    /// </summary>
    internal struct Brick
    {
        /// <summary>
        /// The X position of the first frame in the sprite sheet.
        /// </summary>
        public int FrameX;

        /// <summary>
        /// The Y position of the first frame in the sprite sheet.
        /// </summary>
        public int FrameY;

        /// <summary>
        /// The width of a frame. It's assumed all frames are the same width.
        /// </summary>
        public int FrameWidth;

        /// <summary>
        /// The height of a frame. It's assumed all frames are the same height.
        /// </summary>
        public int FrameHeight;

        /// <summary>
        /// The name of the brick as specified in the <c>name</c> attribute in entities.xml. Must be unique.
        /// </summary>
        public string Name;

        /// <summary>
        /// The filename of the bricks spritesheet, minus file extension.
        /// </summary>
        public string ImageName;
    }
}
