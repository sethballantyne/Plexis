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
using System.IO;

namespace Level_Editor
{
    class Level
    {
        ushort width;
        ushort height;

        int[,] level;

        public Level(ushort x, ushort y)
        {
            level = new int[x, y];

            width = x;
            height = y;

            Clear();
        }

        public int GetElement(ushort x, ushort y)
        {
            return level[x, y];
        }

        public void SetElement(ushort x, ushort y, int value)
        {
            level[x, y] = value;
        }

        public void Clear()
        {
            for (int i = 0; i < width; i++)
            {
                for (int j = 0; j < height; j++)
                {
                    level[i, j] = -1;
                }
            }
        }

        public void Read(string path)
        {
            if (String.IsNullOrEmpty(path))
            {
                throw new ArgumentNullException("path");
            }

            using (FileStream fs = new FileStream(path, FileMode.Open))
            {
                using (BinaryReader br = new BinaryReader(fs))
                {
                    char[] format = new char[6];

                    br.Read(format, 0, 6);
                    if (new string(format) != "PBBLVL")
                    {
                        // error
                    }

                    if (br.ReadInt32() != 1)
                    {
                        // error
                    }

                    for (int i = 0; i < width; i++)
                    {
                        for(int j = 0; j < height; j++)
                        {
                            level[i, j] = br.ReadInt32();
                        }
                    }

                    br.Close();
                }

                fs.Close();
            }
        }

        public void Save(string path)
        {
            if (String.IsNullOrEmpty(path))
            {
                throw new ArgumentNullException("path");
            }

            using (FileStream fs = new FileStream(path, FileMode.Create))
            {
                using (BinaryWriter bw = new BinaryWriter(fs))
                {
                    bw.Write(new char[] { 'P', 'B', 'B', 'L', 'V', 'L' });
                    bw.Write(1);
                    for (int i = 0; i < width; i++)
                    {
                        for (int j = 0; j < height; j++)
                        {
                            bw.Write(level[i, j]);
                        }
                    }

                    bw.Close();
                }

                fs.Close();
            }
        }

    }
}
