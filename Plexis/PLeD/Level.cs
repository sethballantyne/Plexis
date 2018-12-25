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
    /// Represents the level being edited by the user
    /// </summary>
    class Level
    {
        int[,] levelData;

        // the maximum width and height of the level in bricks.
        uint width;
        uint height;

        /// <summary>
        /// Initialises a new instance of Level of the specified dimension in bricks.
        /// </summary>
        /// <param name="width">the maximum number of bricks wide the level is.</param>
        /// <param name="height">the maximum number of bricks high the level is.</param>
        public Level(uint width, uint height)
        {
            this.levelData = new int[width, height];
            this.width = width;
            this.height = height;

            this.Clear();
        }

        /// <summary>
        /// Clears the level of all bricks. This doesn't render the changes, that has to be
        /// done manually.
        /// </summary>
        public void Clear()
        {
            for(int i = 0; i < this.width; i++)
            {
                for(int j = 0; j < this.height; j++)
                {
                    this.levelData[i, j] = -1;
                }
            }
        }

        /// <summary>
        /// Gets or sets the brick at the specified indice.
        /// </summary>
        /// <param name="x">the bricks row coordinate</param>
        /// <param name="y">the bricks column coordinate</param>
        /// <returns>-1 if the there's no brick at the specified index, otherwise an index value corresponding
        /// to the bricks index in the ImageList.
        /// </returns>
        public int this[int x, int y]
        {
            get
            {
                return this.levelData[x, y];
            }
            set
            {
                this.levelData[x, y] = value;
            }
        }

        /// <summary>
        /// formats the current level so XML::WriteLevel can export it to XML.
        /// the level has to be formatted so all the brick coordinates are grouped by brick
        /// in order to be written out in the correct format. You could export it as is, but
        /// the resulting XML would be suboptimal. (it's XML, it's not optimal anyway..)
        /// </summary>
        /// <param name="bricks">An array of the bricks parsed from entities.xml</param>
        /// <returns>The coordinates of each brick in the level grouped by brick name.</returns>
        public Dictionary<string, List<Point>> BuildDictionary(Brick[] bricks)
        {
            Dictionary<string, List<Point>> output = new Dictionary<string,List<Point>>();

            for(int i = 0; i < bricks.Length; i++)
            {
                output[bricks[i].Name] = new List<Point>();
            }

            for(int x = 0; x < this.width; x++)
            {
                for(int y = 0; y < this.height; y++)
                {
                    int index = levelData[x, y];

                    // don't add blanks.
                    if(index != -1)
                    {
                        string name = bricks[index].Name;
                        output[name].Add(new Point(x, y));
                    }
                }
            }
  
            return output;
        }
    }
}
